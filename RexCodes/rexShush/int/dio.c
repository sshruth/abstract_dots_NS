/*
 *-----------------------------------------------------------------------*
 * NOTICE:  This code was developed by the US Government.  The original
 * versions, REX 1.0-3.12, were developed for the pdp11 architecture and
 * distributed without restrictions.  This version, REX 4.0, is a port of
 * the original version to the Intel 80x86 architecture.  This version is
 * distributed only under license agreement from the National Institutes 
 * of Health, Laboratory of Sensorimotor Research, Bldg 10 Rm 10C101, 
 * 9000 Rockville Pike, Bethesda, MD, 20892, (301) 496-9375.
 *-----------------------------------------------------------------------*
 */

/*
 * Digital I/O, d/a converter support.
 */
#include <stdio.h>
#include <x86/inline.h>
#include <sys/types.h>
#include "rexhdrs.h"
#include "int.h"

/*
 * Array of pointers to DIO_DEV array.  Indexed by device id.
 * This array is initialized by dio_init() when int process is first
 * run.
 */
DIO_DEV *dio_index[DIO_DEV_DEVMAX]= {0};
int port_status[DIO_PORT_PORTMAX]= {0};


void
dio_init()
{
    DIO_DEV *dp;
    int dev_search, i, dio_port_base_tmp;
    u_char *cp;

    for(dp= dio_dev; dp->dio_devid != 0; dp++) {
		dio_index[dp->dio_devid]= dp;
    }

    /*
     * Setup port inits.
     */
#ifdef PCI_DIO
    dev_search= PCI_DIO_PRIM;
#else
    dev_search= PCDIO_DIO_ISA;
#endif

    for(dp= dio_dev; dp->dio_devid != 0; dp++) {
		if(dp->dio_devid == dev_search) break;
    }
    if(dp->dio_devid == 0) return; /* no dio */

#ifdef MEXWEX_UNITS
    if(MEXWEX_PORT < 2 || MEXWEX_PORT > 4) {
		rxerr("MEXWEX_PORT must be either 2, 3, or 4");
		/*		goto next0; */
    }
    else if(port_status[MEXWEX_PORT]) {
		rxerr("MEXWEX_PORT and either PCMESSAGE_PORT or PLEX_PORT overlap");
		/* 		goto next0; */
    }
	else {
		port_status[MEXWEX_PORT]= 1;
		cp= &dp->dio_pinit[MEXWEX_PORT*4];

#ifdef MEX62_UNITS
		*cp++= D_IN;
		*cp++= D_IN;
		*cp++= D_OUT_IN;
		*cp++= D_CTRL;
#else
		*cp++= D_IN;
		*cp++= D_IN;
		*cp++= D_IN;
		*cp++= D_CTRL;
#endif

#ifdef PCI_DIO
		dio_port_base_tmp= i_b->i_PciDev[pciUse[dp->dio_pciRexUse]].pcidev_iobase;
#else
		dio_port_base_tmp= PRIM_ISA_DIO_BASE;
#endif
		for(i= 0; i < MEX2_BYTE_NUM; i++) {
			mex2[i].mex2_port= dio_port_base_tmp + (MEXWEX_PORT*4) + i;
		}
	}
	/*  next0: */
#endif	// MEXWEX_UNITS

#ifdef PCMESSAGE
    if(PCMESSAGE_PORT < 2 || PCMESSAGE_PORT > 4) {
		rxerr("PCMESSAGE_PORT must be either 2, 3, or 4");
		/* 		goto next1; */
    }
    else if(port_status[PCMESSAGE_PORT]) {
		rxerr("PCMESSAGE_PORT and either MEXWEX_PORT or PLEX_PORT overlap");
		/* 		goto next1; */
    }
	else {
		port_status[PCMESSAGE_PORT]= 1;
	}
	/*  next1: */
#endif

#ifdef PLEX_ECODES
    if(PLEX_PORT < 2 || PLEX_PORT > 4) {
		rxerr("PLEX_PORT must be either 2, 3, or 4");
		/*		goto next2; */
    }
    else if(port_status[PLEX_PORT]) {
		rxerr("PLEX_PORT and either PCMESSAGE_PORT or MEXWEX_PORT overlap");
		/*		goto next2; */
    }
	else {
		port_status[PLEX_PORT]= 1;
	}
	/* next2: */
#endif
}

/*
 * Output to port.
 *
 * Note: this routine is written so that it can be safely reentered
 * (for example, it might be called from the lower level when also
 * being executed from upper level).  It therefore modifies static data
 * only from inside protected regions.
 */
int
dio_set(DIO_ID id, int value, int flag)
{
    DIO_DEV *dp;
    int port, data;
    u_char pinit;
    unsigned eflags;

    if((dp= dio_index[Dio_dev(id)]) == 0) {
		rxerr("dio_set(): nonexistant device");
		return(0);
    }

    /*
     * Init device if it hasn't been initialized.
     */
    if((eflags = pswget()) & INTR_IF) InterruptDisable();
    if((dp->dio_flags & D_INIT) == 0) {
		dp->dio_flags |= D_INIT;
		if(eflags & INTR_IF) InterruptEnable();
		if(dp->dio_init_f) (*dp->dio_init_f)(dp);
    }
	else if(eflags & INTR_IF) InterruptEnable();
    port= Dio_port(id);
    pinit= dp->dio_pinit[port];
    if(Dio_type(pinit) != D_OUT) {
		rxerr("dio_set(): illegal operation on port, or port out of range.");
		return(0);
    }
    if((eflags = pswget()) & INTR_IF) InterruptDisable();
    switch(flag) {
	case D_ON:
	    data= (dp->dio_state[port] |= Dio_data(id));
	    break;
	case D_OFF:
	    data= (dp->dio_state[port] &= ~Dio_data(id));
	    break;
	case D_VAL:
	    data= value & Dio_data(id);
	    break;
	default:
	    if(eflags & INTR_IF) InterruptEnable();
	    rxerr("dio_set(): illegal flag passed to function");
	    return(0);
    }
    if(pinit & D_REV) data= ~data;
    if(pinit & D_WORD) out16(dp->dio_port_base + port, data);
    else out8(dp->dio_port_base + port, (data & 0xff));
    if(eflags & INTR_IF) InterruptEnable();

#ifdef NIH_ACUC
	i_b->inthdr.i_lastCorrect = i_b->i_time;
#endif

    return(0);
}

/*
 * Input from port.
 */
int
dio_in(DIO_ID id)
{
    DIO_DEV *dp;
    int port, data;
    u_char pinit;
    unsigned eflags;

    if((dp= dio_index[Dio_dev(id)]) == 0) {
		rxerr("dio_set(): nonexistant device");
		return(0);
    }

    /*
     * Init device if it hasn't been initialized.
     */
    if((eflags = pswget()) & INTR_IF) InterruptDisable();
    if((dp->dio_flags & D_INIT) == 0) {
		dp->dio_flags |= D_INIT;
		if(eflags & INTR_IF) InterruptEnable();
		if(dp->dio_init_f) (*dp->dio_init_f)(dp);
    }
	else if(eflags & INTR_IF) InterruptEnable();
    port= Dio_port(id);
    pinit= dp->dio_pinit[port];
    if(Dio_type(pinit) != D_IN) {
		rxerr("dio_in(): illegal operation on port, or port out of range");
		return(0);
    }
    if(pinit & D_WORD) {
		data= in16(dp->dio_port_base + port);
		if(pinit & D_REV) data= ~data;
		return(data & Dio_data(id));
    } else {
		data= in8(dp->dio_port_base + port);
		if(pinit & D_REV) data= ~data & 0xff;
		return(data & Dio_data(id));
    }
}

/*
 * Initialization routines.
 * 
 * CAREFUL!  This routine can be called from interrupt level!!!
 * 
 */
void
i8255_init(DIO_DEV *dp)
{
    int i, apinit, bpinit, cpinit, aclr, bclr, cclr;
    u_int control;

    /*
     * Pick up base address.
     */
#ifdef PCI_DIO
    prim_dio_base= i_b->i_PciDev[pciUse[dp->dio_pciRexUse]].pcidev_iobase;
#else
    prim_dio_base= PRIM_ISA_DIO_BASE;
#endif
    dp->dio_port_base= prim_dio_base;
    i_b->i_prim_dio_base= prim_dio_base; /* save a copy for procs other than int */
   
   
    /*
     * 8255 has 24 dio bits, with 4 ports per chip.  The first
     * three ports are 8 bit dio ports, the fourth is a control
     * register.  A device may have more than one chip.
     * Loop looking for control registers.  Use index addressing
     * instead of pointers for readability (this routine is
     * exectued once, and is not time-critical).
     */
    for(i = 3; Dio_type(dp->dio_pinit[i]) != D_NUL; i += 4) {

		if(Dio_type(dp->dio_pinit[i]) == D_RES) continue;
		if(Dio_type(dp->dio_pinit[i]) != D_CTRL) {
			rxerr("8255_init(): pinit[] array improperly inited");
			return;
		}
	
		/*
		 * Assemble control word based on whether ports are in/out.
		 * Set unused ports as inputs (note that an input port driving
		 * an output I/O module is safe, but an output port driving an
		 * input I/O module is not).
		 *
		 * If D_NOCLR is not set, turn output ports to off.  This will
		 * turn off any output I/O modules such as current sinks or
		 * relays.  This is important for output I/O modules, since
		 * they are on when driven with a logic 0.  The 8255 comes up
		 * with output ports all 0s.
		 */
		control= 0x80;

		apinit= dp->dio_pinit[i-3];
		if(Dio_type(apinit) == D_IN) control |= 0x10;	    /* port A */
		aclr= (apinit & D_REV) ? 0xff : 0x0;

		bpinit= dp->dio_pinit[i-2];
		if(Dio_type(bpinit) == D_IN) control |= 0x2;	    /* port B */
		bclr= (bpinit & D_REV) ? 0xff : 0x0;

		cpinit= dp->dio_pinit[i-1];
		if(Dio_type(cpinit) == D_IN) control |= 0x9;	    /* port C */
		else if(Dio_type(cpinit) == D_IN_OUT) control |= 0x8;	/* port C-hi in, port C-low out */
		else if(Dio_type(cpinit) == D_OUT_IN) control |= 0x1;	/* port C-hi out, port C-low in */
		cclr= (cpinit & D_REV) ? 0xff : 0x0;

		/*
		 * Try to clear outputs as quickly as possible after setting
		 * control word.
		 */
		out8(dp->dio_port_base + i, control);
		if( ! (apinit & D_NOCLR)) out8(dp->dio_port_base + (i-3), aclr);
		if( ! (bpinit & D_NOCLR)) out8(dp->dio_port_base + (i-2), bclr);
		if( ! (cpinit & D_NOCLR)) out8(dp->dio_port_base + (i-1), cclr);
    }
    dp->dio_flags |= D_INIT;
}

/*
 * Output to d/a converters.  This is a low level routine that should
 * not be called directly, but rather use 'da_set()' actions.
 * This routine has no knowledge of the DA struct.
 *
 * Routine is coded to output to multiple d/a's as quickly as possible
 * without allowing intervening interrupts.  This is to guarantee
 * that d/a's update as nearly simultaneously as possible.
 *
 * Note: this routine is written so that it can be safely reentered
 * (for example, it might be called from the lower level when also
 * being executed from upper level).  It therefore modifies static data
 * only from inside protected regions.
 */
int
dio_da(int cnt, DIO_DA_ARG *ap)
{
	DIO_DEV *diop;
	int danum, datmp;
	u_char pinit;
	char *err= NP;
	unsigned eflags;

	/*
	 * D/A output code.  D/A specific info is known here.  The code
	 * takes a value specified in the standard REX internal
	 * calibration of 40 steps/deg, 2s complement
	 * and performs necessary conversions for specific D/A.
	 *
	 * This loop is performed with interrupts locked out so that
	 * multiple d/a's will be updated as simultaneously as possible.
	 */
	if((eflags = pswget()) & INTR_IF) InterruptDisable();
	for(; cnt > 0; cnt--, ap++) {

		if((diop= dio_index[Dio_dev(ap->dio_da_id)]) == 0) {
			err= "dio_da(): nonexistant DIO_ID for d/a";
			goto done;
		}
		if((diop->dio_flags & D_INIT) == 0) {
			diop->dio_flags |= D_INIT;
			if(diop->dio_init_f) {
				if(eflags & INTR_IF) InterruptEnable();
				(*diop->dio_init_f)(diop);
				InterruptDisable();
			}
		}
		danum= Dio_port(ap->dio_da_id);   /* physical d/a number */
		pinit= diop->dio_pinit[danum];
		if(Dio_type(pinit) != D_DA) {
			err= "dio_da(): illegal operation on d/a, or d/a out of range.";
			goto done;
		}

		switch(diop->dio_devid) {

#ifdef PCI_DtoA
		case PCI_DtoA_PRIM:
       
			/*
			 * Find out which PCI d/a we have.
			 */
			switch(diop->dio_vend) {
			case KONTRON_VID:

				switch(diop->dio_device) {
				case AOB8_12_DID:

#ifdef GOO
					fprintf(stderr, "In dio_da() AOB8_12, dio_vend %x, dio_device %x, dio_port_base %x\n",
							diop->dio_vend, diop->dio_device, diop->dio_port_base);
#endif
	     
					if(danum >= 12) {
						break; /* we dont output, but also dont give error */
					}
					if(ap->dio_da_flag & D_NOMAP) datmp= ap->dio_da_value;
					else {
						datmp= -(ap->dio_da_value - 2047);
						if(datmp > 4095) datmp= 4095;
						if(datmp < 0) datmp= 0;
					}
					out16(diop->dio_port_base + (danum << 1), datmp);
					if(ap->dio_da_curvalp != NP) *ap->dio_da_curvalp= ap->dio_da_value;
					break;
				default:

					/*
					 * No d/a- this helps examples run (like tstramp)
					 */
					if(ap->dio_da_curvalp != NP) *ap->dio_da_curvalp= ap->dio_da_value;
					goto done;
				}
				break;

			default:	     

				/*
				 * No d/a- this helps examples run (like tstramp)
				 */
				if(ap->dio_da_curvalp != NP) *ap->dio_da_curvalp= ap->dio_da_value;
				goto done;
			}
			break;
#endif       
#ifdef CIO_DA
		case CIO_DA_ISA:

			/*
			 * Computer Boards d/a cards have 6, 8, or 16 d/a converters.
			 * There are two 8-bit registers/converter, starting at the base
			 * address.  The first register is the lower 8 bits and should
			 * be written first.  The next register is the higher 4 bits.
			 * A write to this register updates the d/a.  Offset binary.
			 */
			if(ap->dio_da_flag & D_NOMAP) datmp= ap->dio_da_value;
			else {
				datmp= -(ap->dio_da_value - 2047);
				if(datmp > 4095) datmp= 4095;
				if(datmp < 0) datmp= 0;
			}
			out8(diop->dio_port_base+(danum << 1), shortlo_(datmp));
			out8(diop->dio_port_base+(danum << 1)+1, shorthi_(datmp));
			if(ap->dio_da_curvalp != NP) *ap->dio_da_curvalp= ap->dio_da_value;
			break;
#endif	
#ifdef ANDAS12
		case ANDAS12_DA:
	
			/*
			 * Analogics DAS12.  Offset binary.  NOTE! This d/a can only drive
			 * +,- 1ma.
			 */
			if(ap->dio_da_flag & D_NOMAP) datmp= ap->dio_da_value;
			else {
				datmp= -(ap->dio_da_value - 2047);
				if(datmp > 4095) datmp= 4095;
				if(datmp < 0) datmp= 0;
			}
			switch(danum) {
			case 0:
				out16(AN1_DAC0_WO, datmp);
				break;
			case 1:
				out16(AN1_DAC1_WO, datmp);
				break;
			default:
				err= "dio_da(): illegal da #";
				goto done;
			}
			if(ap->dio_da_curvalp != NP) *ap->dio_da_curvalp= ap->dio_da_value;
			break;
#endif
		default:
			err= "dio_da(): unsupported da device id";
			goto done;

		}	/* end switch */
	}	/* end for */

 done:
	if(eflags & INTR_IF) InterruptEnable();
	if(err != NP) rxerr(err);
	return(0);
}

/*
 * PCI D/A init routine.
 */
void
pciPrimDtoAInit(DIO_DEV *dp)
{

    /*
     * Pick up base address.
     */
    dp->dio_port_base= i_b->i_PciDev[pciUse[dp->dio_pciRexUse]].pcidev_iobase;

    /*
     * Pick up vendor ID and device ID.
     */
    dp->dio_vend= i_b->i_PciDev[pciUse[dp->dio_pciRexUse]].pcidev_info.VendorId;
    dp->dio_device= i_b->i_PciDev[pciUse[dp->dio_pciRexUse]].pcidev_info.DeviceId;
}
   

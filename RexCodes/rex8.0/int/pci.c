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
 * PCI bus support.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <hw/inout.h>
#include "rexhdrs.h"
#include "ecode.h"
#include "sac.h"
#include "ramp.h"
#include "int.h"
#include <sys/neutrino.h>
#include <sys/mman.h>

#undef DEBUG

int NI_Init(int);
int DIO_Init(int);
int AOB_Init(int);

/*
 * These arrays should be initialized in order to discover PCI devices.  See comments
 * in hdr/pci.h.
 */
PCI Pci[] = {
     {
	PCI_PRIMARY_AtoD,	/* rexuse: primary a/d */
	PCI_SCAN_VEND,  	/* search method */
	NAT_INST_VID,		/* vendor id: National Instruments */
	0,			/* device id */
	0,			/* bus number */
	0,			/* dev number */
	0,			/* function number */
	NI_Init			/* process init function */
     },

     {
	PCI_PRIMARY_DIO,	/* rexuse: primary dio */
	PCI_SCAN_VENDEV,	/* search method */
	KONTRON_VID,		/* vendor id: Kontron */
	PCIDIO120_DID,		/* device id: PCI-DIO120 */
	0,			/* bus number */
	0,			/* dev number */
	0,			/* function number */
	DIO_Init		/* process init function */
     },

     {
	PCI_PRIMARY_DtoA,	/* rexuse: primary d/a */
	PCI_SCAN_VENDEV,	/* search method */
	KONTRON_VID,		/* vendor id: Kontron */
	AOB8_12_DID,		/* device id: PCI-AOB8/12 */
	0,			/* bus number */
	0,			/* dev number */
	0,			/* function number */
	AOB_Init		/* process init function */
     },

     {
	0			/* end of array */
     },
};

int pciUse[PCI_REXUSE_MAX];
struct pci_dev_info pciScanTmp[PCI_DEV_MAX]= {0};


int
PciPrint(struct pci_dev_info *dp, int count)
{
   int i;
   
   for(; count; count--, dp++) {
      printf("Vendor ID %x, Device ID %x\n", dp->VendorId, dp->DeviceId);
      printf("Bus %d, device number %d, function number %d, irq %d\n",
	     dp->BusNumber, (dp->DevFunc & 0xf8) >> 3, (dp->DevFunc & 0x7), dp->Irq);
      for(i= 0; i < 6; i++) {
	 if(!dp->PciBaseAddress[i]) break;
	 if(dp->PciBaseAddress[i] & 01) {
	    printf("Base address register %d, PCI IO address %llx, size %u\n",
		   i, dp->PciBaseAddress[i] & ~0x1, dp->BaseAddressSize[i]);
	 } else {
	    printf("Base address register %d, PCI Memory address %llx, size %u\n",
		   i, dp->PciBaseAddress[i] & ~0xf, dp->BaseAddressSize[i]);
	 }
      }
   }
}

int
PciScan(void)
{
	PCI *pci_p;
	PCI_DEV *dev_p, PciDevClr= {0};
	struct pci_dev_info *di_p, diClr= {0};
	int pciHand, index, scanCount;
	void *attachHand;

#ifdef DEBUG
	fprintf(stderr, "PciScan() entered\n");
#endif
   
	/*
	 * First see if we need to do PCI bus discovery.  This is only
	 * done once when the first 'int' process is started.
	 */
	if(!i_b->i_PciDiscoveryDone) {
		for(index= 0; index < PCI_REXUSE_MAX; index++) pciUse[index]= -1;
		for(index= 0; index < PCI_DEV_MAX; index++) i_b->i_PciDev[index]= PciDevClr; /* clear shared mem */
		if((pciHand= pci_attach(0)) == -1) {	/* attach to RTP PCI server */
			rxerr("PciScan(): cannot pci_attach()\n");
			return(-1);
		}
		for(pci_p= Pci, dev_p= &i_b->i_PciDev[0], index= 0; pci_p->pci_rexuse; pci_p++, dev_p++, index++) {

#ifdef DEBUG
			fprintf(stderr, "Main for, index %d\n", index);
#endif

			di_p= pciScanTmp;
			*di_p= diClr;
	    
			attachHand= (void *)NULL;
			switch(pci_p->pci_search) {
			case PCI_SCAN_VEND:
				if((di_p->VendorId= pci_p->pci_vendor_id) == 0) {
					rxerr("PciScan(): no vendor ID specified for scan, pci_search= PCI_SCAN_VEND");
					printf("PciScan(): PCI array number %d, pci_rexuse= %d\n", index, pci_p->pci_rexuse);
					return(-1);
				}
				for(scanCount= 0; ; di_p++, scanCount++) {
					if((attachHand = pci_attach_device(NULL, PCI_SEARCH_VEND|PCI_INIT_ALL, scanCount, di_p)) == NULL) {
						break;
					}
					if(pci_detach_device(attachHand) != PCI_SUCCESS) {
						rxerr("PciScan(): cannot pci_detach_device");
						return(-1);
					}
					if(scanCount == PCI_DEV_MAX-1) break;
					*(di_p+1)= diClr;
					(di_p+1)->VendorId= di_p->VendorId;
				}
				if(scanCount > 1) {	/* found more than one device using these search specifications */
					rxerr("PciScan(): Found more than one device meeting same search specifications:");
					printf("PciScan(): pci_search= PCI_SCAN_VEND, PCI array number %d, pci_rexuse= %d\n", 
						   index, pci_p->pci_rexuse);
					PciPrint(pciScanTmp, scanCount);
					rxerr("PciScan(): please modify PCI struct initializations in int/pci.c to correct");
					return(-1);
				}
				break;
			case PCI_SCAN_VENDEV:
				if((di_p->VendorId= pci_p->pci_vendor_id) == 0) {
					rxerr("PciScan(): no vendor ID specified for scan, pci_search= PCI_SCAN_VENDEV");
					printf("PciScan(): PCI array number %d, pci_rexuse= %d\n", index, pci_p->pci_rexuse);
					return(-1);
				}
				di_p->DeviceId= pci_p->pci_device_id;
				for(scanCount= 0; ; di_p++, scanCount++) {
					if((attachHand= pci_attach_device(NULL, PCI_SEARCH_VENDEV|PCI_INIT_ALL, scanCount, di_p)) == NULL) {
						break;
					}
					if(pci_detach_device(attachHand) != PCI_SUCCESS) {
						rxerr("PciScan(): cannot pci_detach_device");
						return(-1);
					}
					if(scanCount == PCI_DEV_MAX-1) break;
					*(di_p+1)= diClr;
					(di_p+1)->VendorId= di_p->VendorId;
					(di_p+1)->DeviceId= di_p->DeviceId;
				}
				if(scanCount > 1) {	/* found more than one device using these search specifications */
					rxerr("PciScan(): Found more than one device meeting same search specifications:");
					printf("PciScan(): pci_search= PCI_SCAN_VENDEV, PCI array number %d, pci_rexuse= %d\n", 
						   index, pci_p->pci_rexuse);
					PciPrint(pciScanTmp, scanCount);
					rxerr("PciScan(): please modify PCI struct initializations in int/pci.c to correct");
					return(-1);
				}
				break;
			case PCI_SCAN_BUSDEV:	/* no need to look for multiple devices when scaning with
									 * PCI_SCAN_BUSDEV- there is only one possible device with the same
									 * bus number, device number, and function number */
				di_p->BusNumber= pci_p->pci_busnum;
				di_p->DevFunc= ((pci_p->pci_devnum & 0x1f) << 3) | (pci_p->pci_funcnum & 0x3);
				scanCount= 0;
				if((attachHand= pci_attach_device(NULL, PCI_SEARCH_BUSDEV|PCI_INIT_ALL, NULL, di_p)) != NULL) {
					if(pci_detach_device(attachHand) != PCI_SUCCESS) {
						rxerr("PciScan(): cannot pci_detach_device");
						return(-1);
					}
					scanCount= 1;
				}
				break;
			default:
				rxerr("PciScan(): bad search specifier");
				return(-1);
			} /* END SWITCH */
			if(scanCount == 1) {	/* found a device */
				if(pciUse[pci_p->pci_rexuse] >= 0) {
					rxerr("PciScan(): found two devices assigned to same use (see hdr/pci.h)");
					printf("PciScan(): pci_rexuse: %d\n", pci_p->pci_rexuse);
					pciScanTmp[1]= i_b->i_PciDev[pciUse[pci_p->pci_rexuse]].pcidev_info;
					PciPrint(pciScanTmp, 2);
					rxerr("To correct this please change search methods or rexuse in int/pci.c");
					rxerr("to distinguish between two devices");
					return(-1);
				}
				dev_p->pcidev_rexuse= pci_p->pci_rexuse;
				dev_p->pcidev_info= pciScanTmp[0];
				pciUse[pci_p->pci_rexuse]= index;
			}
		} /* END FOR */
		if(pci_detach(pciHand) != PCI_SUCCESS) {
			rxerr("PciScan(): cannot pci_detach()");
			return(-1);
		}
	} /* END if(!i_b->i_PciDiscoveryDone) */

#ifdef DEBUG
	fprintf(stderr, "After main for\n");
#endif
   
	/*
	 * Pci discovery has already been done.  Call initialization functions.
	 */
	for(index= 0; index < PCI_REXUSE_MAX; index++) pciUse[index]= -1;
	for(dev_p= i_b->i_PciDev, index= 0; index < PCI_DEV_MAX; index++, dev_p++) {
		if(dev_p->pcidev_rexuse) {		/* device found */
			pciUse[dev_p->pcidev_rexuse]= index;
			if((Pci[index].pciInit)(index) == -1) {
				rxerr("PciScan(): error on call to device init function");
				printf("PCI array number %d, pci_rexuse= %d\n", index, dev_p->pcidev_rexuse);
				PciPrint(&dev_p->pcidev_info, 1);
				return(-1);
			}
#ifdef DEBUG
			fprintf(stderr, "Device array index: %d, rexuse %d, search %d\n",
					index, Pci[index].pci_rexuse, Pci[index].pci_search);
			PciPrint(&dev_p->pcidev_info, 1);
#endif DEBUG
		}
	}
	i_b->i_PciDiscoveryDone= 1; /* NOT MULTIPLE CPU SAFE: if multiple REX processes are started together
								 * on multiple cpu's they may run init routines simultaneously thinking
								 * it's the first time they are called.
								 * 
								 * Do this here because some initialization functions check this
								 * to see it is the first time they are called */
}

/*
 * Clear software copies of registers.
 */
void
NI_ClearSoftRegisters(NI_ATOD *adp)
{
   int i;
   
   for(i= 0; i < NI_DIR_SCOPY_NUM; i++) adp->ni_DirScopy[i]= 0;
   for(i= 0; i < NI_WIN_SCOPY_NUM; i++) adp->ni_WinScopy[i]= 0;
}

NI_ATOD NiAtoD[NI_MAX_NUMBER];
int NiNumAtoD= 0;		/* number of NI a/d's found */
NI_ATOD *ni_adp;

/*
 * Init NI E-series PCI a/d boards.
 */
int 
NI_Init(int index)
{
   PCI_DEV *dp;
   NI_ATOD *adp;
   unsigned window_data_val;
   
#ifdef DEBUG
   fprintf(stderr, "NI_init called, index %d\n", index);
#endif
   if(NiNumAtoD >= NI_MAX_NUMBER) {
      rxerr("Too many NI a/d cards found");
      return(-1);
   }
   adp= &NiAtoD[NiNumAtoD];
   dp= &i_b->i_PciDev[index];
   dp->pcidev_index= NiNumAtoD++;
   adp->ni_irq= dp->pcidev_info.Irq;
   adp->ni_MiteBar= dp->pcidev_info.PciBaseAddress[0] & ~0xf;	/* Mite is BAR0 */
   adp->ni_DaqBar= dp->pcidev_info.PciBaseAddress[1] & ~0xf;	/* Daq is BAR1 */
   
   /*
    * Map E-series registers into process address space.
    */
   if((adp->ni_MiteVar= mmap_device_memory(0, 4096, PROT_READ|PROT_WRITE|PROT_NOCACHE, 0, adp->ni_MiteBar)) == MAP_FAILED)
   {
      rxerr("Ni_Init(): Mite base map failed\n");
      return(-1);
   }
   if((adp->ni_DaqVar= mmap_device_memory(0, 4096, PROT_READ|PROT_WRITE|PROT_NOCACHE, 0, adp->ni_DaqBar)) == MAP_FAILED)
   {
      rxerr("Ni_Init(): Daq base map failed");
      return(-1);
   }

#ifdef DEBUG   
   fprintf(stderr, "Mite bar %x, vad %x, Daq bar %x, vad %x, irq %d\n",
	   adp->ni_MiteBar, adp->ni_MiteVar, adp->ni_DaqBar, adp->ni_DaqVar, adp->ni_irq);
#endif   

   /*
    * Turn on device window of MITE (whatever that means).
    */
   if(!i_b->i_PciDiscoveryDone) {
      window_data_val= ((0xffffff00 & adp->ni_DaqBar) | 0x80); /* magic from the NI examples */
      *((unsigned *)(adp->ni_MiteVar+0xc0))= window_data_val; /* more magic from the NI examples */
   }
   NI_ClearSoftRegisters(adp); /* clear software registers */
   ni_adp= adp;
#ifdef DEBUG
   fprintf(stderr, "ni_adp %x, irq %d\n", ni_adp, ni_adp->ni_irq);
#endif   
   return(0);
}

/*
 * Init Kontron PCI-DIO series digital i/o boards.
 */
int 
DIO_Init(int index)
{
   int i;
   PCI_DEV *dp;
   
#ifdef DEBUG
   fprintf(stderr, "DIO_init called, index %d\n", index);
   dp= &i_b->i_PciDev[index];
   fprintf(stderr, "DIO_init() PRE: base %x, length %u\n", dp->pcidev_iobase, dp->pcidev_iolen);
   fprintf(stderr, "base from i_b: %x\n", i_b->i_PciDev[pciUse[PCI_PRIMARY_DIO]].pcidev_iobase);
#endif
   if(i_b->i_PciDiscoveryDone) return(0); /* only need to do once */
   dp= &i_b->i_PciDev[index];
   dp->pcidev_driver= 0;
   for(i= 0;; i++) {
      if(!dp->pcidev_info.PciBaseAddress[i]) break;
      if(!(dp->pcidev_info.PciBaseAddress[i] & 01)) continue; /* mem address */

      /*
       * The PCIDIO cards have two IO registers.  The one with length 32 is
       * the correct base address.
       */
      if(dp->pcidev_info.BaseAddressSize[i] != 32) continue; /* wrong size */
      dp->pcidev_iobase= dp->pcidev_info.PciBaseAddress[i] & ~0x1;
      dp->pcidev_iolen= dp->pcidev_info.BaseAddressSize[i];
      break;
   }
#ifdef DEBUG
   fprintf(stderr, "DIO_init() base %x, length %u\n", dp->pcidev_iobase, dp->pcidev_iolen);
#endif
   return(0);
}

/*
 * Init Kontron PCI-AOB type d/a boards.
 */
int 
AOB_Init(int index)
{
   int i;
   PCI_DEV *dp;
   
#ifdef DEBUG
   fprintf(stderr, "AOB_init called, index %d\n", index);
   dp= &i_b->i_PciDev[index];
   fprintf(stderr, "AOB_init() PRE: base %x, length %u\n", dp->pcidev_iobase, dp->pcidev_iolen);
   fprintf(stderr, "base from i_b: %x\n", i_b->i_PciDev[pciUse[PCI_PRIMARY_DIO]].pcidev_iobase);
#endif
   if(i_b->i_PciDiscoveryDone) return(0); /* only need to do once */
   dp= &i_b->i_PciDev[index];
   dp->pcidev_driver= 0;
   for(i= 0;; i++) {
      if(!dp->pcidev_info.PciBaseAddress[i]) break;
      if(!(dp->pcidev_info.PciBaseAddress[i] & 01)) continue; /* mem address */

      /*
       * The AOB cards have three IO registers.  The one with length 64 is
       * the correct base address.
       */
      if(dp->pcidev_info.BaseAddressSize[i] != 64) continue; /* wrong size */
      dp->pcidev_iobase= dp->pcidev_info.PciBaseAddress[i] & ~0x1;
      dp->pcidev_iolen= dp->pcidev_info.BaseAddressSize[i];
      break;
   }
   switch(dp->pcidev_info.DeviceId) {
    case AOB8_12_DID:
      
#ifdef DEBUG
      fprintf(stderr, "AOB_Init: initing AOB8/12\n");
#endif
      
      /*
       * 12 d/a converters.  Clear and set to automatic update mode.
       */
      for(i= 0; i < 12; i++) out16(dp->pcidev_iobase + (i << 1), 2048); /* offset binary */
      in8(dp->pcidev_iobase + 0xa); /* update all outputs and release from simultaneous update mode */
      in8(dp->pcidev_iobase + 0xf); /* clear restrict-output-voltage mode */
      break;

    default:
      rxerr("Bad deviceid in AOB_Init()");
   }
      
#ifdef DEBUG
   fprintf(stderr, "AOB_init() base %x, length %u\n", dp->pcidev_iobase, dp->pcidev_iolen);
#endif
   return(0);
}

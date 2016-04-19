/*
 * pci.h: Header for support of devices on Plug-n-Play PCI bus.
 */

#ifndef __PCI_H_INCLUDED
#include <hw/pci.h>
#endif

#define PCI_DEV_MAX 30 /* max number of PCI devices that can be specified to search for
			* during PCI discovery */

typedef unsigned short int usint, *usint_p;
typedef unsigned char uchar, *uchar_p;
typedef unsigned int uint;

/*
 * Vendor and Device ID defines.
 */
#define NAT_INST_VID 0x1093
	#define NI6023E_DID 0x2a60
#define KONTRON_VID 0x494f 
	#define PCIDIO120_DID 0xc78
	#define AOB8_12_DID 0x6ca8

/*
 * Rex needs to have a way to identify how PCI realtime cards are used, and
 * what addresses have been assigned to them.  Each PCI card has an associated
 * vendor ID and device ID.  It also has a bus number and device/function number
 * depending on which slot it is inserted.  Rex can search for cards using 
 * various combinations of these values (the slot number could also be used, 
 * but this info is not readily obtainable using the RTP pci* routines.)  
 * Scans for devices can be done in three ways: by vendor id match only, by vendor id
 * and device id match, and by bus number, device number, and function number match.
 * The user must specify how to perform the scan, and initialize appropriate fields.
 * Fields not needed for scan are ignored.  These structs are initialized in pci.c.
 * This complexity is needed if there are cards with same vendor id
 * and device id used for different purposes.  For example, a system may have three
 * identical a/d converters- analog eye movement related, antidromic stimulation, and WEX.
 * In this case one would need bus, device, function info to distinguish among them 
 * and provide true Plug and Play.  This info can be obtained from the 'pci' program
 * under RTP.
 * 
 * There are PCI_DEV_MAX numbers of this struct.  Each one cross references the same
 * member of the PCI_DEV struct in shared memory.
 * 
 * When the first 'int' process runs it checks the 'i_PciDiscoveryDone' flag in shared memory.
 * If it is false, PCI discovery is done.  The discovery code steps thru the PCI array below
 * looking for the specified card installed in the system.  If it is found the corresponding member of
 * the PCI_DEV array in shared memory is initialized.  Then the 'pciOnceInit()' function is
 * called for this card.  The 'i_PciDiscoveryDone' flag is then set to true.
 * 
 * If the 'i_PciDiscoveryDone' flag is true, the code steps thru the array below checking the
 * corresponding member of the PCI_DEV array in shared memory.  If the PCI_DEV array is initialized
 * then the card was previously found on the system when PCI discovery was done.  The code then
 * calls the 'pciProcessInit()' function for the card.  This function also fills in the global
 * index variable for this card (e.g. 'PciPrimAtoD' below) so that drivers can reference the
 * card.
 */
typedef struct {
   int		pci_rexuse;			/* how used by Rex */
   int		pci_search;			/* how to search */
   int		pci_vendor_id;			/* vendor id */
   int		pci_device_id;			/* device id */
   int		pci_busnum;			/* specifies which bus card is on */
   int		pci_devnum;	        	/* device on bus */
   int		pci_funcnum;			/* function number */
   int  	(*pciInit)(int PciIndex);	/* pointer to init function, called when process is
						 * first run */
} PCI;

/*
 * Defines for pci_rexuse above.
 * 
 * These numbers describe the use of each PCI card.  They are stored in 'pci_rexuse'
 * above.  One cannot have more than one installed card assigned to the same function.
 * As Rex does PCI bus discovery it will complain if it finds more than one card assigned
 * to the same function.
 * 
 * ANY NEW CARD ADDED TO REX MUST HAVE A CORRESPONDING ENTRY BELOW DESCRIBING IT'S FUNCTION,
 * 
 * Note: numbers must start with 1.  Number 0 is reserved to signfy unused entry.
 */
#define PCI_PRIMARY_AtoD	1	/* primary a/d for data acquisition */
#define PCI_ANTIDROMIC_AtoD	2	/* secondary a/d, e.g. for MEX/WEX */
#define PCI_WEX_AtoD		3	/* a/d for WEX */
#define PCI_PRIMARY_DIO		4	/* primary digital i/o card */
#define PCI_TCUNIT_CNTR0	5	/* Counter timer card for detecting units */
#define PCI_TCUNIT_CNTR1	6	/* Counter timer card for detecting units */
#define PCI_TCUNIT_CNTR2	7	/* Counter timer card for detecting units */
#define PCI_PRIMARY_DtoA	8	/* primary d/a card */
#define PCI_SYNC_CNTR		9	/* Counter timer card for multiple cpu sync */
#define PCI_REXUSE_MAX		50	/* ABOVE NUMBERS CANNOT BE GREATER THAN THIS!!! */

/*
 * This array is indexed by the pci_rexuse defines above.  It contains the index of
 * the PCI and PCI_DEV arrays associated with each function.  Value is -1 when not
 * used.
 */
extern int pciUse[PCI_REXUSE_MAX];
  
/*
 * Initialized by user, instructs how to scan PCI bus.  Stored in 'pci_search' above.
 */
#define PCI_SCAN_VEND		0	/* scan based on vendor id only */
#define PCI_SCAN_VENDEV		1	/* scan based on vendor id and device id */
#define PCI_SCAN_BUSDEV		2	/* scan based on bus, devnum, and funcnum */

/*
 * One of these structs describes each realtime PCI device.  Stored in shared
 * memory.  There are PCI_DEV_MAX numbers of this array, equal to the number of
 * PCI structs in each int process.  They cross reference each other.  For example,
 * the third PCI struct indexes the third PCI_DEV struct in shared memory.
 */
typedef struct {
   int		pcidev_rexuse;		/* Rex function of this card; null if not used */
   struct pci_dev_info pcidev_info;	/* pci dev info struct for this device returned during discovery */
   uint		pcidev_driver;		/* index to select driver if applicable for this card; inited by
					 * one_time_init function */
   uint		pcidev_index;		/* index to other structures defined for this device;  device
					 * specific field that is used in various ways by drivers */
   uint		pcidev_iobase;		/* pci i/o space base address- this is inited by the
					 * one_time_init function to contain the I/O base address
					 * most likely to be used with this card
					 */
   uint		pcidev_iolen;		/* size of i/o space */
} PCI_DEV;

extern struct pci_dev_info pciScanTmp[PCI_DEV_MAX];	/* temp holds results of scans */

/* Define header for application - AppBuilder 2.03  */

/* Internal Module Links */
extern const int ABI_rexErrorDialog;
#define ABM_rexErrorDialog                   &AbInternalLinks[ABI_rexErrorDialog]

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_fileMenuButton;
#define ABW_fileMenuButton                   AbGetABW( ABN_fileMenuButton )
extern const int ABN_displayMenuButton;
#define ABW_displayMenuButton                AbGetABW( ABN_displayMenuButton )
extern const int ABN_cntrlBMenuButton;
#define ABW_cntrlBMenuButton                 AbGetABW( ABN_cntrlBMenuButton )
extern const int ABN_dataKeepBothToggle;
#define ABW_dataKeepBothToggle               AbGetABW( ABN_dataKeepBothToggle )
extern const int ABN_dataKeepEventsToggle;
#define ABW_dataKeepEventsToggle             AbGetABW( ABN_dataKeepEventsToggle )
extern const int ABN_procWaitTimer;
#define ABW_procWaitTimer                    AbGetABW( ABN_procWaitTimer )

/* 'processSelectorDialog' Dialog link */
extern const int ABN_processSelectorDialog;
#define ABW_processSelectorDialog            AbGetABW( ABN_processSelectorDialog )
extern const int ABN_procFileDirText;
#define ABW_procFileDirText                  AbGetABW( ABN_procFileDirText )
extern const int ABN_procFileSelList;
#define ABW_procFileSelList                  AbGetABW( ABN_procFileSelList )
extern const int ABN_procFileName;
#define ABW_procFileName                     AbGetABW( ABN_procFileName )
extern const int ABN_procSelChangeBtn;
#define ABW_procSelChangeBtn                 AbGetABW( ABN_procSelChangeBtn )
extern const int ABN_procSelLoadBtn;
#define ABW_procSelLoadBtn                   AbGetABW( ABN_procSelLoadBtn )

/* 'rootSelectorDialog' Dialog link */
extern const int ABN_rootSelectorDialog;
#define ABW_rootSelectorDialog               AbGetABW( ABN_rootSelectorDialog )
extern const int ABN_rootFileDirText;
#define ABW_rootFileDirText                  AbGetABW( ABN_rootFileDirText )
extern const int ABN_rootFileSelList;
#define ABW_rootFileSelList                  AbGetABW( ABN_rootFileSelList )
extern const int ABN_rootFileName;
#define ABW_rootFileName                     AbGetABW( ABN_rootFileName )
extern const int ABN_rootSelReadBtn;
#define ABW_rootSelReadBtn                   AbGetABW( ABN_rootSelReadBtn )
extern const int ABN_rootSelAllBtn;
#define ABW_rootSelAllBtn                    AbGetABW( ABN_rootSelAllBtn )
extern const int ABN_rootSelSysUsrBtn;
#define ABW_rootSelSysUsrBtn                 AbGetABW( ABN_rootSelSysUsrBtn )
extern const int ABN_rootSelSysBtn;
#define ABW_rootSelSysBtn                    AbGetABW( ABN_rootSelSysBtn )
extern const int ABN_rootSelUsrBtn;
#define ABW_rootSelUsrBtn                    AbGetABW( ABN_rootSelUsrBtn )
extern const int ABN_rootSelStatesBtn;
#define ABW_rootSelStatesBtn                 AbGetABW( ABN_rootSelStatesBtn )

/* 'dataSelectorDialog' Dialog link */
extern const int ABN_dataSelectorDialog;
#define ABW_dataSelectorDialog               AbGetABW( ABN_dataSelectorDialog )
extern const int ABN_dataFileName;
#define ABW_dataFileName                     AbGetABW( ABN_dataFileName )
extern const int ABN_dataSelLoadBtn;
#define ABW_dataSelLoadBtn                   AbGetABW( ABN_dataSelLoadBtn )
extern const int ABN_dataFileDirText;
#define ABW_dataFileDirText                  AbGetABW( ABN_dataFileDirText )
extern const int ABN_dataFileSelList;
#define ABW_dataFileSelList                  AbGetABW( ABN_dataFileSelList )

/* 'rexErrorDialog' Dialog link */
extern const int ABN_rexErrorDialog;
#define ABW_rexErrorDialog                   AbGetABW( ABN_rexErrorDialog )
extern const int ABN_rexErrorText;
#define ABW_rexErrorText                     AbGetABW( ABN_rexErrorText )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

/* 'displayMenu' Menu link */
extern const int ABN_displayMenu;
extern const int ABN_displayWindow;
extern const int ABN_displayRaster;
extern const int ABN_displayRunLine;
extern const int ABN_xcorrrDisplay;

/* 'filesMenu' Menu link */
extern const int ABN_filesMenu;
extern const int ABN_filesMenuProcesses;
extern const int ABN_fileRootsRead;
extern const int ABN_filesDataOpen;
extern const int ABN_filesDataClose;

#define AB_OPTIONS "s:x:y:h:w:S:"

/* Define header for application - AppBuilder 2.03  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_oscopePane;
#define ABW_oscopePane                       AbGetABW( ABN_oscopePane )
extern const int ABN_runLineWindow;
#define ABW_runLineWindow                    AbGetABW( ABN_runLineWindow )
extern const int ABN_cntrlBtnPane;
#define ABW_cntrlBtnPane                     AbGetABW( ABN_cntrlBtnPane )
extern const int ABN_triggerLabel;
#define ABW_triggerLabel                     AbGetABW( ABN_triggerLabel )
extern const int ABN_rlTrigButton;
#define ABW_rlTrigButton                     AbGetABW( ABN_rlTrigButton )
extern const int ABN_displayLabel;
#define ABW_displayLabel                     AbGetABW( ABN_displayLabel )
extern const int ABN_rlRepeatButton;
#define ABW_rlRepeatButton                   AbGetABW( ABN_rlRepeatButton )
extern const int ABN_rlModeButton;
#define ABW_rlModeButton                     AbGetABW( ABN_rlModeButton )
extern const int ABN_levelLabel;
#define ABW_levelLabel                       AbGetABW( ABN_levelLabel )
extern const int ABN_rlTrigLevel;
#define ABW_rlTrigLevel                      AbGetABW( ABN_rlTrigLevel )
extern const int ABN_chan0Toggle;
#define ABW_chan0Toggle                      AbGetABW( ABN_chan0Toggle )
extern const int ABN_chan1Toggle;
#define ABW_chan1Toggle                      AbGetABW( ABN_chan1Toggle )
extern const int ABN_chan2Toggle;
#define ABW_chan2Toggle                      AbGetABW( ABN_chan2Toggle )
extern const int ABN_chan3Toggle;
#define ABW_chan3Toggle                      AbGetABW( ABN_chan3Toggle )
extern const int ABN_unitsToggle;
#define ABW_unitsToggle                      AbGetABW( ABN_unitsToggle )
extern const int ABN_chan4Toggle;
#define ABW_chan4Toggle                      AbGetABW( ABN_chan4Toggle )
extern const int ABN_chan5Toggle;
#define ABW_chan5Toggle                      AbGetABW( ABN_chan5Toggle )
extern const int ABN_chan6Toggle;
#define ABW_chan6Toggle                      AbGetABW( ABN_chan6Toggle )
extern const int ABN_chan7Toggle;
#define ABW_chan7Toggle                      AbGetABW( ABN_chan7Toggle )
extern const int ABN_rlineToggle;
#define ABW_rlineToggle                      AbGetABW( ABN_rlineToggle )
extern const int ABN_runLineMenuBar;
#define ABW_runLineMenuBar                   AbGetABW( ABN_runLineMenuBar )
extern const int ABN_runLineOptions;
#define ABW_runLineOptions                   AbGetABW( ABN_runLineOptions )
extern const int ABN_dispResetButton;
#define ABW_dispResetButton                  AbGetABW( ABN_dispResetButton )
extern const int ABN_rlTimer;
#define ABW_rlTimer                          AbGetABW( ABN_rlTimer )

/* 'runLineDialog' Dialog link */
extern const int ABN_runLineDialog;
#define ABW_runLineDialog                    AbGetABW( ABN_runLineDialog )
extern const int ABN_runDispBarOffScroll;
#define ABW_runDispBarOffScroll              AbGetABW( ABN_runDispBarOffScroll )
extern const int ABN_runDispRateScroll;
#define ABW_runDispRateScroll                AbGetABW( ABN_runDispRateScroll )
extern const int ABN_sclBtnGrp;
#define ABW_sclBtnGrp                        AbGetABW( ABN_sclBtnGrp )
extern const int ABN_scale0Btn;
#define ABW_scale0Btn                        AbGetABW( ABN_scale0Btn )
extern const int ABN_scale1Btn;
#define ABW_scale1Btn                        AbGetABW( ABN_scale1Btn )
extern const int ABN_scale2Btn;
#define ABW_scale2Btn                        AbGetABW( ABN_scale2Btn )
extern const int ABN_scale3Btn;
#define ABW_scale3Btn                        AbGetABW( ABN_scale3Btn )
extern const int ABN_scale4Btn;
#define ABW_scale4Btn                        AbGetABW( ABN_scale4Btn )
extern const int ABN_scale5Btn;
#define ABW_scale5Btn                        AbGetABW( ABN_scale5Btn )
extern const int ABN_wrapBtn;
#define ABW_wrapBtn                          AbGetABW( ABN_wrapBtn )
extern const int ABN_runChanLabel;
#define ABW_runChanLabel                     AbGetABW( ABN_runChanLabel )
extern const int ABN_runChanList;
#define ABW_runChanList                      AbGetABW( ABN_runChanList )
extern const int ABN_runChanSigNumb;
#define ABW_runChanSigNumb                   AbGetABW( ABN_runChanSigNumb )
extern const int ABN_runChanSigTitle;
#define ABW_runChanSigTitle                  AbGetABW( ABN_runChanSigTitle )
extern const int ABN_runChanOffsetScroll;
#define ABW_runChanOffsetScroll              AbGetABW( ABN_runChanOffsetScroll )
extern const int ABN_runChanScaleScroll;
#define ABW_runChanScaleScroll               AbGetABW( ABN_runChanScaleScroll )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"

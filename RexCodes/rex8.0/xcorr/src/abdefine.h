/* Define header for application - AppBuilder 2.03  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_xcorrTimer;
#define ABW_xcorrTimer                       AbGetABW( ABN_xcorrTimer )
extern const int ABN_xcorrToolbar;
#define ABW_xcorrToolbar                     AbGetABW( ABN_xcorrToolbar )
extern const int ABN_sampleToggleButton;
#define ABW_sampleToggleButton               AbGetABW( ABN_sampleToggleButton )
extern const int ABN_clearButton;
#define ABW_clearButton                      AbGetABW( ABN_clearButton )
extern const int ABN_trigUnitInteger;
#define ABW_trigUnitInteger                  AbGetABW( ABN_trigUnitInteger )
extern const int ABN_xcorrUnitInteger;
#define ABW_xcorrUnitInteger                 AbGetABW( ABN_xcorrUnitInteger )
extern const int ABN_preInteger;
#define ABW_preInteger                       AbGetABW( ABN_preInteger )
extern const int ABN_intervalInteger;
#define ABW_intervalInteger                  AbGetABW( ABN_intervalInteger )
extern const int ABN_sigmaFloat;
#define ABW_sigmaFloat                       AbGetABW( ABN_sigmaFloat )
extern const int ABN_tigWindow;
#define ABW_tigWindow                        AbGetABW( ABN_tigWindow )
extern const int ABN_gainScrollbar;
#define ABW_gainScrollbar                    AbGetABW( ABN_gainScrollbar )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"

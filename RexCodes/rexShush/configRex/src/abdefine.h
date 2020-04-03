/* Define header for application - AppBuilder 2.01  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_configureButton;
#define ABW_configureButton                  AbGetABW( ABN_configureButton )
extern const int ABN_a2dGroup;
#define ABW_a2dGroup                         AbGetABW( ABN_a2dGroup )
extern const int ABN_NIATMIOXToggle;
#define ABW_NIATMIOXToggle                   AbGetABW( ABN_NIATMIOXToggle )
extern const int ABN_ANDAS12Toggle;
#define ABW_ANDAS12Toggle                    AbGetABW( ABN_ANDAS12Toggle )
extern const int ABN_NIPCIToggle;
#define ABW_NIPCIToggle                      AbGetABW( ABN_NIPCIToggle )
extern const int ABN_vectorGroup;
#define ABW_vectorGroup                      AbGetABW( ABN_vectorGroup )
extern const int ABN_vect3Toggle;
#define ABW_vect3Toggle                      AbGetABW( ABN_vect3Toggle )
extern const int ABN_vect4Toggle;
#define ABW_vect4Toggle                      AbGetABW( ABN_vect4Toggle )
extern const int ABN_vect5Toggle;
#define ABW_vect5Toggle                      AbGetABW( ABN_vect5Toggle )
extern const int ABN_vect7Toggle;
#define ABW_vect7Toggle                      AbGetABW( ABN_vect7Toggle )
extern const int ABN_vect10Toggle;
#define ABW_vect10Toggle                     AbGetABW( ABN_vect10Toggle )
extern const int ABN_vect11Toggle;
#define ABW_vect11Toggle                     AbGetABW( ABN_vect11Toggle )
extern const int ABN_vect12Toggle;
#define ABW_vect12Toggle                     AbGetABW( ABN_vect12Toggle )
extern const int ABN_vect15Toggle;
#define ABW_vect15Toggle                     AbGetABW( ABN_vect15Toggle )
extern const int ABN_adBasePortInteger;
#define ABW_adBasePortInteger                AbGetABW( ABN_adBasePortInteger )
extern const int ABN_smiYesToggle;
#define ABW_smiYesToggle                     AbGetABW( ABN_smiYesToggle )
extern const int ABN_smiNoToggle;
#define ABW_smiNoToggle                      AbGetABW( ABN_smiNoToggle )
extern const int ABN_pcdioBasePortInteger;
#define ABW_pcdioBasePortInteger             AbGetABW( ABN_pcdioBasePortInteger )
extern const int ABN_isaDioToggle;
#define ABW_isaDioToggle                     AbGetABW( ABN_isaDioToggle )
extern const int ABN_pciDioToggle;
#define ABW_pciDioToggle                     AbGetABW( ABN_pciDioToggle )
extern const int ABN_unitGroup;
#define ABW_unitGroup                        AbGetABW( ABN_unitGroup )
extern const int ABN_latchToggle;
#define ABW_latchToggle                      AbGetABW( ABN_latchToggle )
extern const int ABN_mexwexToggle;
#define ABW_mexwexToggle                     AbGetABW( ABN_mexwexToggle )
extern const int ABN_plexonToggle;
#define ABW_plexonToggle                     AbGetABW( ABN_plexonToggle )
extern const int ABN_fakeToggle;
#define ABW_fakeToggle                       AbGetABW( ABN_fakeToggle )
extern const int ABN_pcmessageToggle;
#define ABW_pcmessageToggle                  AbGetABW( ABN_pcmessageToggle )
extern const int ABN_pcs_socketToggle;
#define ABW_pcs_socketToggle                 AbGetABW( ABN_pcs_socketToggle )
extern const int ABN_hardwareYesToggle;
#define ABW_hardwareYesToggle                AbGetABW( ABN_hardwareYesToggle )
extern const int ABN_hardwareNoToggle;
#define ABW_hardwareNoToggle                 AbGetABW( ABN_hardwareNoToggle )
extern const int ABN_plexEcodeGroup;
#define ABW_plexEcodeGroup                   AbGetABW( ABN_plexEcodeGroup )
extern const int ABN_plexEcodeNoToggle;
#define ABW_plexEcodeNoToggle                AbGetABW( ABN_plexEcodeNoToggle )
extern const int ABN_plexEcodeYesToggle;
#define ABW_plexEcodeYesToggle               AbGetABW( ABN_plexEcodeYesToggle )
extern const int ABN_isaCtToggle;
#define ABW_isaCtToggle                      AbGetABW( ABN_isaCtToggle )
extern const int ABN_pciCtToggle;
#define ABW_pciCtToggle                      AbGetABW( ABN_pciCtToggle )
extern const int ABN_isaD2AToggle;
#define ABW_isaD2AToggle                     AbGetABW( ABN_isaD2AToggle )
extern const int ABN_pciD2AToggle;
#define ABW_pciD2AToggle                     AbGetABW( ABN_pciD2AToggle )
extern const int ABN_daBasePortInteger;
#define ABW_daBasePortInteger                AbGetABW( ABN_daBasePortInteger )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"

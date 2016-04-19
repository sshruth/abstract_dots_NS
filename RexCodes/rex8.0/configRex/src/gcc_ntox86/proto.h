
/* ../abmain.c */

/* ../atodChoice.c */
int atodChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../basePortChoice.c */
int basePortChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../d2aChoice.c */
int d2aChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../dioChoice.c */
int dioChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../initArgs.c */
int initArgs ( int argc , char *argv []);
void parseFile ( void );
void readLine ( char *linePtr );

/* ../initDialog.c */
int initDialog ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../messageChoice.c */
int messageChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../plexEcodeChoice.c */
int plexEcodeChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../pstopSwitchChoice.c */
int pstopSwitchChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../smiChoice.c */
int smiChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../timerChoice.c */
int timerChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../unitChoice.c */
int unitChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../vectChoice.c */
int vectChoice ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../writeCnfFile.c */
int writeCnfFile ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );
void writeLine ( FILE *cnfFile , char *linePtr );

/* ../rexLineTok.c */
char *rexlinetok ( char *string );


/* awind.c */
int awind ( long flag );
int pre_post ( long pre , long post );
int Ppre_post ( long *pre , long *post );
int mwind ( long flag );

/* cal.c */
int hold_eye ( void );
int keep_eye ( void );

/* da_set.c */
int da_cntrl ( int cnt , DA_CNTRL_ARG *ap );
int Pda_cntrl ( int *cnt , DA_CNTRL_ARG *ap );
int da_cntrl_1 ( long danum , long source , long src_num );
int Pda_cntrl_1 ( long *danum , long *source , long *src_num );
int da_cntrl_2 ( long danum0 , long source0 , long src_num0 , long danum1 , long source1 , long src_num1 );
int Pda_cntrl_2 ( long *danum0 , long *source0 , long *src_num0 , long *danum1 , long *source1 , long *src_num1 );
int da_mode ( long danum , long mode );
int Pda_mode ( long *danum , long *mode );
int da_set ( int cnt , DA_SET_ARG *ap );
int Pda_set ( int *cnt , DA_SET_ARG *ap );
int da_set_1 ( long danum , long val );
int Pda_set_1 ( long *danum , long *val );
int da_set_2 ( long danum0 , long val0 , long danum1 , long val1 );
int Pda_set_2 ( long *danum0 , long *val0 , long *danum1 , long *val1 );
int da_offset ( long danum , long val );
int Pda_offset ( long *danum , long *val );
int da_cursor ( long da_x , long da_y , long cursor );
int Pda_cursor ( long *da_x , long *da_y , long *cursor );
int da_init ( void );

/* dout.c */
int dio_on ( DIO_ID id );
int Pdio_on ( DIO_ID *id );
int dio_off ( DIO_ID id );
int Pdio_off ( DIO_ID *id );
int dio_onoff ( DIO_ID id_on , DIO_ID id_off );
int Pdio_onoff ( DIO_ID *id_on , DIO_ID *id_off );
int dio_out ( DIO_ID id , long value );
int Pdio_out ( DIO_ID *id , long *value );

/* ma_set.c */
int ma_cntrl ( int manum , int *bap , int count , int rate , int repeat );
int Pma_cntrl ( int *manum , int *bap , int *count , int *rate , int *repeat );
int ma_reset ( int manum );
int Pma_reset ( int *manum );
int ma_start ( int manum );
int Pma_start ( int *manum );
int ma_stop ( int manum );
int Pma_stop ( int *manum );
void ma_unlink ( MA *map );
int ma_init ( void );

/* off_eye.c */
int off_eye ( long hoff , long voff );
int Poff_eye ( long *hoff , long *voff );

/* off_oeye.c */
int off_oeye ( long hoff , long voff );
int Poff_oeye ( long *hoff , long *voff );

/* ramp.c */
void ra_compute ( int rnum );

/* ramp_act.c */
int ra_new ( int rnum , int len , int ang , int vel , int xoff , int yoff , int ecode , int type );
int Pra_new ( int *rnum , int *len , int *ang , int *vel , int *xoff , int *yoff , int *ecode , int *type );
int ra_start ( long rnum , long flag , DIO_ID device );
int Pra_start ( long *rnum , long *flag , DIO_ID *device );
int ra_stop ( long rnum );
int Pra_stop ( long *rnum );
int ra_tostart ( long rnum );
int Pra_tostart ( long *rnum );
int ra_phistart ( long rnum );
int Pra_phistart ( long *rnum );
int ra_phiend ( long rnum );
int Pra_phiend ( long *rnum );
void ra_unlink ( RAMP *rp );
RA_RAMP_TIME ra_compute_time ( long rnum , long presetper , long randper );
int Pra_compute_time ( long *rnum , long *presetper , long *randper , RA_RAMP_TIME *ra_time );
int ramptd ( long rnum );
int Pramptd ( long *rnum );
int ra_init ( void );

/* rl_set.c */
int rl_setspike ( long spike_num );
int rl_setspike_bits ( long spike_bits );
int rl_setbar ( long val );
int rl_addbar ( long val );
int rl_trig ( void );
int rl_stop ( void );
int rl_erase ( void );

/* scntrl.c */
int awake ( long x );
int asleep ( long x );
int abort_s ( long x );
int reset_s ( long x );

/* score.c */
int score ( long x );

/* sd_set.c */
int sd_set ( long ctlval );
int sd_mark ( long chan , long mark );
void sd_mark_sig ( char *sig_name , int mark );

/* sf_init.c */
int sf_init ( long x );

/* shuffle.c */
int shuffle ( unsigned k , int shift , int *table );

/* stateTime.c */
int set_times ( char *name , long preset , long random );
int Pset_times ( char *name , long *preset , long *random );
long getClockTime ( void );
int PgetClockTime ( long *time );
int PsetFlag ( int *flag , int val );

/* ustatus.c */
int ustatus ( char *sp );

/* uw_set.c */
int uw_set ( long flag );

/* vexActions.c */
int tst_rx_new ( void );
int tst_tx_rdy ( void );
int to_vex ( unsigned short len_of_msg );
unsigned char *vex_message ( void );
unsigned char vex_code ( unsigned char *msgfvex );
int vex_location ( unsigned char *msgfvex );
unsigned char *float2byte ( float value );
unsigned char hibyte ( short sval );
unsigned char lobyte ( short sval );
int PvexEraseMethod ( int *method );
int PvexVideoSync ( int *sync );
int PvexDigitalSync ( int *sync );
int PvexSetRexScale ( int *distance , int *width );
int PvexSetBackLum ( int *r , int *g , int *b );
int PvexSetFixColors ( int *nr , int *ng , int *nb , int *dr , int *dg , int *db );
int PvexSetFixSiz ( int *size );
int PvexSetStimLuminances ( int *nObjects , int *objList , int *fgList , int *bgList );
int PvexSetStimColors ( int *nObjects , int *objList , int *fgrList , int *fggList , int *fgbList , int *bgrList , int *bggList , int *bgbList );
int PvexSetColorMask ( int *nObjects , int *objList , int *redList , int *greenList , int *blueList , int *alphaList );
int PvexSetGrayScale ( int *start , int *length );
int PvexSetObjectGrayScale ( int *nObjects , int *objList , int *startList , int *lengthList );
int PvexSetLutEntryClr ( int *nEntries , int *entryList , int *rList , int *gList , int *bList );
int PvexSetObjectLutEntryClr ( int *nObjects , int *objList , int *nEntries , int *entryList , int *rList , int *gList , int *bList );
int PvexAllOff ( void );
int PvexSwitchFix ( int *flag );
int PvexDimFix ( void );
int PvexPreloadStim ( int *nObjects , int *objList , int *swtchList );
int PvexSwapBuffers ( void );
int PvexSwitchStim ( int *nObjects , int *objList , int *swtchList );
int PvexSetStimSwitch ( int *nObjects , int *objList , int *swtchList );
int PvexTimeStim ( int *nObjects , int *objList , int *nFields );
int PvexSequenceStim ( int *nFrstObj , int *frstList , int *frstFields , int *gapFields , int *nScndObj , int *scndList , int *scndFields );
int PvexSetFixLocation ( float *x , float *y );
int PvexStimLocation ( int *nObjects , int *objList , float *xList , float *yList );
int PvexStimFromFixPoint ( int *nObjects , int *objList , float *xList , float *yList );
int PvexShiftLocation ( int *nObjects , int *objList , float *xList , float *yList );
int PvexReportLocation ( int *obj );
int PvexMessage ( unsigned char *code , int *x , int *y );
int PvexSetActiveObject ( int *obj );
int PvexClipRectSet ( int *nObjects , int *objList , int *widthList , int *heightList , int *xList , int *yList );
int PvexClipRectFromFixPoint ( int *nObjects , int *objList , int *widthList , int *heightList , int *xList , int *yList );
int PvexClearClipRect ( int *nObjects , int *objList );
int PvexDrawWalsh ( int *nObjects , int *objList , int *patList , int *sizList , int *cntrstList );
int PvexDrawHaar ( int *nObjects , int *objList , int *patList , int *sizList , int *cntrstList );
int PvexDrawRandom ( int *nObjects , int *objList , int *colList , int *rowList , int *sizList , int *pwhiteList );
int PvexDrawAnnulus ( int *nObjects , int *objList , int *outrList , int *innrList , int *cntrstList );
int PvexDrawBar ( int *nObjects , int *objList , int *anglList , int *widList , int *lenList , int *cntrstList );
int PvexDrawFlowField ( int *nObjects , int *objList , int *widthList , int *heightList , int *nearList , int *farList , int *covList , int *sizeList );
int PvexDrawEllipticalFlowField ( int *nObjects , int *objList , int *radList , int *widthList , int *heightList , int *nearList , int *farList , int *covList , int *sizeList );
int PvexMaskFlowField ( int *nObjects , int *objList , int *widthList , int *heightList , int *xList , int *yList );
int PvexDrawUserPattern ( int *nObjects , int *objList , int *patList , int *sizeList , int *cntrstList );
int PvexDrawRgbUserPattern ( int *nObjects , int *objList , int *patList , int *sizeList );
int PvexDrawTiffImage ( int *nObjects , int *objList , int *patList );
int PvexDrawSineGrating(int *nObjects, int *objList, int *orList, float *spatFreqList, float *tempFreqList, int *cntrPhaseList, float *redList, float *greenList, float *blueList, int *sizList)
int PvexDrawOknGrating ( int *nObjects , int *objList , int *dirList , int *speedList , int *widList , int *heightList , int *barwidList );
int PvexLoadPatterns ( int *nObjects , int *objList , int *stimList , int *sizList , int *rowList , int *colList , int *checkList );
int PvexLoadPointArray ( int *obj , int *nPoints , int *siz , int *pointArray );
int PvexCopyObject ( int *source , int *destination , float *xscale , float *yscale );
int PvexSetAliasing ( int *nObjects , int *objList , int *aliasList );
int PvexRotateObject ( int *nObjects , int *objList , float *Xlist , float *Ylist , float *Zlist );
int PvexNewRamp ( int *nRamps , int *rampList , int *lenList , int *dirList , int *velList , int *xList , int *yList , int *typeList , int *actList );
int PvexNewRampFromFixPoint ( int *nRamps , int *rampList , int *lenList , int *dirList , int *velList , int *xList , int *yList , int *typeList , int *actList );
int PvexLoadRamp ( int *nRamps , int *rampList , int *actList , int *stpList , float *xList , float *yList );
int PvexToRampStart ( int *nObjects , int *objList , int *rmpList );
int PvexStartRamp ( int *nRamps , int *rmpList , int *objList , int *cycleList );
int PvexResetRamps ( void );
int PvexStartSine ( int *nObjects , int *objList );
int PvexStopSine ( void );
int PvexStartOkn ( int *nObjects , int *objList );
int PvexStopOkn ( void );
int PvexNewFlow ( int *nObjects , int *objList , int *xyList , int *zList , int *velList , int *rollList , int *pitchList , int *yawList , int *spanList , int *coherList );
int PvexShiftFlow ( int *nObjects , int *objList , int *xyList , int *zList , int *velList , int *rollList , int *pitchList , int *yawList , int *spanList , int *coherList );
int PvexMakeFlowMovie ( int *nObjects , int *nFrames , int *objList );
int PvexToFlowMovieStart ( int *nObjects , int *startFrame , int *objList );
int PvexStartFlow ( int *nObjects , int *objList );
int PvexTimeFlow ( int *nObjects , int *fields , int *objList );
int PvexShowFlowMovie ( int *nObjects , int *startFrame , int *lastFrame , int *objList );
int PvexStartFlowRamp ( int *nFlows , int *flwList , int *nRamps , int *rmpList , int *objList );
int PvexStopFlowRamp ( void );
int PvexShowMovieClip ( int *nClips , int *objList , int *nFrames , int *interval , int *cycles );
int PvexStopMovie ( void );
int PvexSetTriggers ( int *nTrigs , int *rmpList , int *trigList , int *frameList );

/* wd_set.c */
int wd_cntrl ( long wdnum , long flag );
int Pwd_cntrl ( long *wdnum , long *flag );
int wd_src_pos ( long wdnum , long x_src , long x_src_num , long y_src , long y_src_num );
int Pwd_src_pos ( long *wdnum , long *x_src , long *x_src_num , long *y_src , long *y_src_num );
int wd_src_check ( long wdnum , long x_src , long x_src_num , long y_src , long y_src_num );
int Pwd_src_check ( long *wdnum , long *x_src , long *x_src_num , long *y_src , long *y_src_num );
int wd_siz ( long wdnum , long xsiz , long ysiz );
int Pwd_siz ( long *wdnum , long *xsiz , long *ysiz );
int wd_pos ( long wdnum , long xpos , long ypos );
int Pwd_pos ( long *wdnum , long *xpos , long *ypos );
int wd_test ( long flag );
int Pwd_test ( long *flag );
int wd_init ( void );
int wd_disp ( long disp );
int wd_cursor ( long wdnum , long cursor );
int wd_center ( long cursor );

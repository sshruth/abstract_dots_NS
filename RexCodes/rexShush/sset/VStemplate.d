#define WIND0 0
#define PHOTO_RESET_BIT Dio_id(PCDIO_DIO, 2, 0x04)
#define PHOTO_CELL_INPUT 0x1000
#define PHOTO_TRIG 1

int photoTrigger = 0;

rinitf(void)
{
	char *vexHost = "lsr-xpsbb";		/* if you are going to use Vex */
	char *mexHost = "lsr-labmex";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);		/* if you are going to use Vex */
	pcsConnectMex(mexHost);		/* if you are going to use Mex */
	
	wd_disp(D_W_EYE_X);
	wd_cursor(WIND0, CU_WIND_8);
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_center(CU_CENTER);
	wd_cntrl(WIND0, WD_ON);
	return (0);
}

/*
 * statelist menu
 */
VLIST state_vl[] = {
	NS,
};
char hm_sv_vl[]= "";

/*
 * list of user menus
 */
MENU umenus[] = {
	"state_vars",	&state_vl,	NP,	NP,	0,	NP,	hm_sv_vl,
	NS,
};

/*
 *
 */
USER_FUNC ufuncs[] = {
	{""},
};

/*
 * User-supplied real-time variables
 */
RTVAR rtvars[] = {
"", 0,
};

%%
id 100
restart rinitf
main_set {
status ON
begin	first:
		to disabl
	disabl:
		to enable on -PSTOP & softswitch  /*is bar enabled?*/
	enable:
		code ENABLECD
	    to .....
	.
	.
	.
	.
	.
	last:
		to first
}

photoTrig {
status ON
begin	phfrst:
		to phdsbl
	phdsbl:
		to reset on -PSTOP & drinput
	reset:
		do PsetFlag(&photoTrigger, 0)
		time 10
		to rstpht
	rstpht:
		do dio_on(PHOTO_RESET_BIT)
		time 10
		to clrrst
	clrrst:
		do dio_off(PHOTO_RESET_BIT)
 		to photo on +PHOTO_CELL_INPUT & drinput
	photo:
		do PsetFlag(&photoTrigger, 1)
		to phfrst
}

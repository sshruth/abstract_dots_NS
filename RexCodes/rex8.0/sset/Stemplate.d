#define WIND0 0
#define WIND1 1

rinitf(void)
{
	char *vexHost = "lsr-xpsbb";		/* if you are going to use Vex */
	char *mexHost = "lsr-labmex";		/* if you are going to use Mex */

	pcsConnectVex(vexHost);		/* if you are going to use Vex */
	pcsConnectMex(mexHost);		/* if you are going to use Mex */
	
	wd_disp(D_W_EYE_X);
	wd_cursor(WIND0, CU_WIND_8);
	wd_cursor(WIND1, CU_WIND_12);
	wd_src_pos(WIND0, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_pos(WIND1, WD_DIRPOS, 0, WD_DIRPOS, 0);
	wd_src_check(WIND0, WD_SIGNAL, 0, WD_SIGNAL, 1);
	wd_src_check(WIND1, WD_SIGNAL, 2, WD_SIGNAL, 3);
	wd_center(CU_CENTER);
	wd_cntrl(WIND0, WD_ON);
	return (0);
}

/*
 * statlist menu
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

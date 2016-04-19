#ifndef RebelState_H
#define RebelState_H

class rebelState;
class rebelChain;

const uint32_t maxCmnt = 97;
const uint32_t maxEscp = 10;

#define MAXESC	10
typedef	struct {
	char action[32];
	char act_code_type[16];
	char act_code[32];
	char act_code_array[16];
	char act_2code_type[16];
	char act_2code[32];
	char act_2code_array[16];
	char act_3code_type[16];
	char act_3code[32];
	char act_3code_array[16];
	char act_4code_type[16];
	char act_4code[32];
	char act_4code_array[16];
	char act_5code_type[16];
	char act_5code[32];
	char act_5code_array[16];
	char act_6code_type[16];
	char act_6code[32];
	char act_6code_array[16];
	char act_7code_type[16];
	char act_7code[32];
	char act_7code_array[16];
	char act_8code_type[16];
	char act_8code[32];
	char act_8code_array[16];
	char act_9code_type[16];
	char act_9code[32];
	char act_9code_array[16];
	char act_10code_type[16];
	char act_10code[32];
	char act_10code_array[16];
} ACTION;

typedef	struct {
	char nextesc[32];
	char awakestate[32];
	char stateptr[32];
	char sflags[32];
	char funct[32];
	char smask[32];
	PtWidget_t *ePath;
	rebelState *toState;
	char escapeOn[128];
	char comment[97];
} ESCAPE;

typedef struct {
	char statename[30];
	char code[32];
	char ptrCode[32];
	char rlflag[32];
	char rlval[32];
	ACTION device;
	char preset[32];
	char random[32];
	char comment[97];
	ESCAPE stateEscapes[MAXESC];
} REX_STATE;

class rebelState : public rebelButton {
 public:
	rebelState();														// default constructor
	rebelState(PtWidget_t *parent, char *name);							// alternate constructor
	rebelState(PtWidget_t *parent, char *name, PhPoint_t pos);			// alternate constructor
	rebelState(rebelState& rs);											// copy constructor
	virtual ~rebelState();												// destructor
	rebelState& operator=(rebelState& rs);								// assignment operator

	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void stateName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void ecode( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void rexAction( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void runLine( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void rexTime( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void rexRand( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void stateComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void actionName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void actionArg( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void rexActionOk( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void clearActionArgs( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void escapeOn( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void escapeComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void escapeOk( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void escapeResources( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void parseEscapeOn(ESCAPE *escp);
	void saveButton(FILE *fp);
	void genCode(FILE *fp);
	void genSpot(FILE *fp);
	void portButton(rebelWin *w, string line = "NULL");
	ESCAPE *newEscape(rebelState *toState);
	int nextRightSocket();
	int nextLeftSocket();
	void cutEscape( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void cutEscape( int i );

	static int resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int stateNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int ecodeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int rexActionCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int runLineCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int rexTimeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int rexRandCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int stateCommentCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int actionNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int actionArgCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int rexActionOkCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int clearActionArgsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int escapeOnCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int escapeCommentCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int escapeDialogCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int escapeOkCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int escapeResourcesCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	inline char *sname() { return(_sname); }
	inline void ecode(char *e) { strncpy(_rexState.code, e, 32); }
	inline char *ecode() {
		if(!strcmp(_rexState.code, "0")) return(_rexState.ptrCode);
		else return(_rexState.code);
	}
	inline char *action() { return(_action); }
	inline ACTION device() { return(_rexState.device); }
	inline void runLineLevel(char *n) { strncpy(_runLineLevel, n, 8); }
	inline char *runLineLevel() { return(_runLineLevel); }
	inline void time(char *t) { strncpy(_rexState.preset, t, 8); }
	inline char *time() { return(_rexState.preset); }
	inline void rand(char *r) { strncpy(_rexState.random, r, 8); }
	inline char *rand() { return(_rexState.random); }
	inline void comment(char *r) { strncpy(_rexState.comment, r, 97); }
	inline char *comment() { return(_rexState.comment); }
	inline REX_STATE *rexState() { return(&_rexState); }
	inline int numEscapes() { return(_escapeIndx + 1); }
	inline ESCAPE *escapes() { return(_rexState.stateEscapes); }
	inline ESCAPE *escapes(int indx) {
		if((indx >= 0) && (indx < MAXESC)) return(&_rexState.stateEscapes[indx]);
		else return((ESCAPE *)NULL);
	}
	inline void genDecl(FILE *fp) { fprintf(fp, "extern STATE %s;\n", _sname); }
	inline void genArrayName(FILE *fp) { fprintf(fp, "\t&%s, \n", _sname); }

 protected:
	int parseAction();
	void checkGlobalVariables();
	int buildActionString();
	void buildEscapeString(ESCAPE *escp);
	void setActionArgTypes();
	void setActionArgArrays();
	void scanStateName(rebelWin *w, string line);
	void scanRunLine(rebelWin *w, string line);
	void scanAction(rebelWin *w, string line);
	void scanTime(rebelWin *w, string line);
	void scanEscape(rebelWin *w, string line, ESCAPE *escp);
 private:
	REX_STATE _rexState;

	char _sname[32];
	char _action[384];
	char _runLineLevel[8];
	int _rightSockets[MAXESC];
	int _leftSockets[MAXESC];
	int _rightSocketIndx;
	int _leftSocketIndx;
	int _escapeIndx;

};
#endif

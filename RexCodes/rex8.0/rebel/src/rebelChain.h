#ifndef RebelChain_H
#define RebelChain_H

class rebelChain : public rebelWin {
 public:
	rebelChain();													// default constructor
	rebelChain(const char *n, PtCallbackInfo_t *cbinfo);			// constructor
	rebelChain(const char *n, PhPoint_t origin, PhDim_t size, PtCallbackInfo_t *cbinfo);
	rebelChain(const rebelChain& rc);								// copy constructor
	virtual ~rebelChain();
	rebelChain& operator=(const rebelChain& rc);					// assignment operator

	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void chainName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void restartFunction( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void newEscape( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void eventDrag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void addEscape( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void initAbortList( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void addAbortState( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void abortListOk( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void saveWindow(FILE *fp);
	void readWindow();
	void portWindow(rebelApp *app, string first, string second = "NULL");
	void restartFunction(char *f);
	void snapToGrid();
	void cutButton(rebelButton *b);
	void genStateDecls(FILE *fp);
	void genStateArrayNames(FILE *fp);
	void genStateDefs(FILE *fp);
	void genAbortList(FILE *fp);
	void genSpotDefs(FILE *fp);

	static int chainNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int restartFunctionCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int newStateCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int newEscapeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int addEscapeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int initAbortListCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int addAbortStateCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int abortListOkCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	inline vector<string>abortList() { return(_abortList); }
	inline string firstState() {
		rebelState *s = (rebelState *)_buttons[0];
		return(s->sname());
	}
	inline char *restartFunction() { return(_restartFunc); }

 protected:
	void escapePath();
	void escapePath(rebelState *_fromState, rebelState *_toState);
	void createEscapeWidget(ESCAPE *escp, rebelState *_fromState, rebelState *_toState);
	void portAborts(char *tok, string abortName);
	void setupEscapes();
	void setAbortImage();
	void setAbortImage(const char *name);

	rebelState *toState(char *name);
 private:
	static PgColor_t _colorList[10];
	static int _colorListIndx;

    char* _source;
    vector<char> _sourceTemp;

	// these variables get written to the save file
	vector<string> _abortList;
	char _restartFunc[32];

	bool _addEscape;

	PhPoint_t _escpPoints[12];

	rebelState *_fromState;
	rebelState *_toState;
};

#endif

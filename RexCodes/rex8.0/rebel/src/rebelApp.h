#ifndef RebelApp_H
#define RebelApp_H


template <class T>
class windowLessThan : public binary_function<T, T, bool> {
 public:
	bool operator()(const T a, const T b) {
		if(*a < *b) return(true);
		else return(false);
	}
};


class rebelApp {
 public:
	rebelApp(PtCallbackInfo_t *cbinfo);
	rebelApp(string name, PtCallbackInfo_t *cbinfo);
	virtual ~rebelApp();

	void readPrototypes(const char *prototype, size_t size);
	void newChain( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void newMenu( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void newVar( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void newFunc( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void hideAll( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void showAll( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void listWindows( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void clearCursor( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void initActionList( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void initGlobalList( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void jed( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void jedHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void ws( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void wsHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void vi( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void viHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void ped( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void pedHdr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void makeApp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int newChainCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int newMenuCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int newVarCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int newFuncCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int hideAllCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int showAllCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int listWindowsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int clearCursorCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int initActionListCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int initGlobalListCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int jedCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int jedHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int wsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int wsHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int viCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int viHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int pedCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int pedHdrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int makeAppCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	inline bool saved() { return(_saved); }
	inline void saved(bool flag) { _saved = flag; }
	inline rebelWin *activeWindow() { return(_activeWindow); }
	inline char *srcBeg() { return(_source); }
	inline char *srcEnd() { return(_source + _SstatBuf.st_size); }
	string protoType(char *actName);

	void selectWindow(rebelWin *w);
	void cutWindow(rebelWin *w);

	void addGlobalVariables(char *type, char *var, char *arrayInd = "0");
	void deleteGlobalVariable(char *variable);
	void addFunction(char *funcName, string proto);
	void deleteAction(char *action);

	void readApp();
	bool saveApp(string fileName, bool spot = false);
	bool generate();
	bool generateSpot();
	void portApp(string fileName, PtCallbackInfo_t *cbinfo);
	void portChains(PtCallbackInfo_t *cbinfo);
	void portMenus(PtCallbackInfo_t *cbinfo);
	void portRealTimeVars(PtCallbackInfo_t *cbinfo);
	void portUserFunctions(PtCallbackInfo_t *cbinfo);
	void portRestartFunctions();
 protected:
	void setActionList();
	void setIncludeList();
	bool generateHeader();
	bool generateSource();
	bool checkIncludes(string line);
	void checkGlobals(size_t s);
	void checkActions(size_t s);
	void printHeader(FILE *fp, size_t s);
	void printIncludes(FILE *fp, size_t s);
	void printUserIncludes(FILE *fp, size_t s);
	void printGlobals(FILE *fp, size_t s);
	void printActions(FILE *fp);
	void printUserSource(FILE *fp, size_t s);
	void printSpot(FILE *fp);
	void printUserMenu(FILE *fp);
	void printUserFunction(FILE *fp);
	void printRealTimeVar(FILE *fp);
	void printStateDecl(FILE *fp);
	void printStateName(FILE *fp);
   	void printStateDef(FILE *fp);
	void printAbortLists(FILE *fp);
	void printInitLists(FILE *fp);
	void printProperties(FILE *fp);
 private:
	rebelLineTok *_lineTok;
	PhDim_t _prefSize;
	vector<rebelWin *> _chains;
	vector<rebelWin *> _menus;
	rebelWin *_function;
	rebelWin *_variable;
	rebelWin *_activeWindow;
	string _name;
	string _Pname;
	string _Hname;
	string _Cname;
	string _Dname;
	bool _spot;
	bool _saved;
	bool _generatedCode;

	int _pdgmNum;
	struct stat _HstatBuf;
	struct stat _SstatBuf;
	struct stat _PstatBuf;
	char *_prototype;
	char *_header;
	char *_source;
	char *_tok;
	vector<char> _prototypeTemp;
	vector<char> _sourceTemp;
	vector<char> _headerTemp;
	vector<char> _listBuffer;
	vector<string> _rexIncludeList;
	vector<string> _rexActionList;
	vector<string> _labActionList;
	vector<string> _localActionList;
	vector<string> _globalVariables;
	vector<string> _neededGlobals;
	vector<string> _neededActions;
};
#endif

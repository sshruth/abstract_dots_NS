#ifndef RebelMenu_H
#define RebelMenu_H

typedef struct {
	char name[32];
	char vlistPtr[32];
	char basePtr[32];
	char maf[32];
	char aff[32];
	char agf[32];
	char helpName[32];
	vector<char> help;
} REX_MENU;

class rebelMenu : public rebelWin {
 public:
	rebelMenu();
	rebelMenu(const char *n, PtCallbackInfo_t *cbinfo);
	rebelMenu(const char *n, PhPoint_t origin, PhDim_t size, PtCallbackInfo_t *cbinfo);
	rebelMenu(const rebelMenu& rm);
	virtual ~rebelMenu();
	rebelMenu& operator=(const rebelMenu& rm);

	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void menuName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void variableList( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void basePtr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void maf( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void accessFlag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void agf( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void helpName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void help( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void saveWindow(FILE *fp);
	void readWindow();
	void portWindow(rebelApp *app, string first, string second = "NULL");
	void genVarLists(FILE *fp);
	void genMenuArray(FILE *fp);

	static void clearHelpList();
	static int newItemCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int menuNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int variableListCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int basePtrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int mafCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int accessFlagCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int agfCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int helpNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int helpCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	
	inline char *vlistPtr() { return(_rexMenu.vlistPtr); }
	inline char *basePtr() { return(_rexMenu.basePtr); }
	inline char *maf() { return(_rexMenu.maf); }
	inline char *aff() { return(_rexMenu.aff); }
	inline char *agf() { return(_rexMenu.agf); }
	inline char *helpName() { return(_rexMenu.helpName); }
	inline char *help() { return(&_rexMenu.help[0]); }

 protected:
 private:
	static vector<string> _helpList;

	void portVlist(rebelApp *app);
	void portHelp(rebelApp *app);
	REX_MENU _rexMenu;
};

#endif

#ifndef RebelVariableItem_h
#define RebelVariableItem_h

class rebelVariableItem : public rebelButton {
 public:
	rebelVariableItem();
	rebelVariableItem(PtWidget_t *parent, const char *name, PhPoint_t pos);
	rebelVariableItem(rebelVariableItem& mi);
	virtual ~rebelVariableItem();
	rebelVariableItem& operator=(const rebelVariableItem& mi);

	void resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void variableName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void variable( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void saveButton(FILE *fp);
	void genCode(FILE *fp);
	void readButton();
	void portButton(rebelWin *w, string line = "NULL");

	inline char *variable() { return(_variable); }

	static int resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int variableNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int variableCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
 protected:
 private:
	char _variable[32];
};
#endif

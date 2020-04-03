#ifndef RebelVariable_H
#define RebelVariable_H

class rebelVariable : public rebelWin {
 public:
	rebelVariable(PtCallbackInfo_t *cbinfo);
	rebelVariable(PhPoint_t origin, PhDim_t size, PtCallbackInfo_t *cbinfo);
	virtual ~rebelVariable();

	void addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int newVariableCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	void saveWindow(FILE *fp);
	void readWindow();
	void portWindow(rebelApp *app, string first, string second = "NULL");

	void genVariables(FILE *fp);
 protected:
 private:
};

#endif

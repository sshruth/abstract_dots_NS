#ifndef RebelFunction_H
#define RebelFunction_H

class rebelFunction : public rebelWin {
 public:
	rebelFunction(PtCallbackInfo_t *cbinfo);
	rebelFunction(PhPoint_t origin, PhDim_t size, PtCallbackInfo_t *cbinfo);
	virtual ~rebelFunction();

	void addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int newFunctionCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	void saveWindow(FILE *fp);
	void readWindow();
	void portWindow(rebelApp *app, string first, string second = "NULL");

	void genFunctions(FILE *fp);

 protected:
 private:
};
#endif

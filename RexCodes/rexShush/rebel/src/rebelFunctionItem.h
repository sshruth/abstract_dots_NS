#ifndef RebelFunctionItem_h
#define RebelFunctionItem_h

class rebelFunctionItem : public rebelButton {
 public:
	rebelFunctionItem();
	rebelFunctionItem(PtWidget_t *parent, const char *name, PhPoint_t pos);
	rebelFunctionItem(rebelFunctionItem& mi);
	virtual ~rebelFunctionItem();
	rebelFunctionItem& operator=(const rebelFunctionItem& mi);

	void resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void functionName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void function( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void format( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void saveButton(FILE *fp);
	void readButton();
	void genCode(FILE *fp);
	void portButton(rebelWin *w, string line = "NULL");

	inline char *function() { return(_function); }
	inline char *format() { return(_format); }

	static int resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int functionNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int functionCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int formatCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
 protected:
 private:
	char _function[32];
	char _format[32];
};
#endif

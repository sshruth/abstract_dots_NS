#ifndef RebelMenuItem_H
#define RebelMenuItem_H

typedef struct {
	char name[32];
	char type[32];
	char variable[32];
	char basePtr[32];
	char vaf[32];
	char aff[32];
} REX_ITEM;

class rebelMenuItem : public rebelButton {
 public:
	rebelMenuItem();
	rebelMenuItem(PtWidget_t *parent, const char *name, PhPoint_t pos);
	rebelMenuItem(rebelMenuItem& mi);
	virtual ~rebelMenuItem();
	rebelMenuItem& operator=(const rebelMenuItem& mi);

	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void itemName( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void variable( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void basePtr( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void vaf( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void accessFlag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void varType( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void saveButton(FILE *fp);
	void readButton();
	void genCode(FILE *fp);
	void portButton(rebelWin *w, string line = "NULL");

	static int resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int itemNameCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int variableCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int basePtrCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int vafCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int accessFlagCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int varTypeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	inline char *variable() { return(_rexItem.variable); }
	inline char *basePtr() { return(_rexItem.basePtr); }
	inline char *vaf() { return(_rexItem.vaf); }
	inline char *aff() { return(_rexItem.aff); }
	inline char *varType() { return(_rexItem.type); }
	inline REX_ITEM rexItem() { return(_rexItem); }
 protected:
 private:
	REX_ITEM _rexItem;
};
#endif

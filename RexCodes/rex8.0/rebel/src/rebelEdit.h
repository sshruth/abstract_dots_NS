#ifndef RebelEdit_H
#define RebelEdit_H

class rebelEdit {
 public:
	rebelEdit();
	virtual ~rebelEdit();

	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void copy( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void textGotFocus( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void paste( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void deleteObject( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void snap( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int cutCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int copyCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int textGotFocusCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int pasteCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int deleteCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int snapCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void selectObject(PtWidget_t *object);
	void copyObject();
 protected:
	void cutState(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo);
	void cutEscape(PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo);
 private:
	PtWidget_t *_selectedObject;
	PtWidgetClassRef_t *_selectedClass;

	PtWidget_t *_copiedObject;
	PtWidget_t *_focusWidget;
	vector<char> copiedString;
};
#endif

#ifndef rebelResource_H
#define rebelResource_H

class rebelResource {
 public:
	rebelResource();
	virtual ~rebelResource();

	void initAction( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void initEscape( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void resize( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void winOpen( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void clearActionArgs( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	void setChainWidgets(rebelChain *actChain);
	void setStateWidgets(rebelState *actState);
	void setActionArgLabels(char *actName);
	void setEscapeWidgets(ESCAPE *ptr);
	void setMenuWidgets(rebelMenu *actMenu);
	void setVlistWidgets(rebelMenuItem *actItem);
	void setFunctionWidgets(rebelFunctionItem *actItem);
	void setVariableWidgets(rebelVariableItem *actItem);
	void setCommentWidgets(rebelComment *actCmnt);

	static int init( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int initActionCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int initEscapeCallback( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int resizeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int winOpenCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int textNotify( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int clearActionArgsCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int setMenuAffText( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int setVlistAffText( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int setVlistTypeText( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int switchCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	inline void origin(PhPoint_t o) { _origin = o; }
	inline void size(PhDim_t s) { _size = s; }
	inline PtWidget_t *panelGroup() { return(_panelGroup); }

 protected:
 private:
	PtWidget_t *_window;
	PhPoint_t _origin;
	PhDim_t _size;
	PtWidget_t *_panelGroup;
};
#endif

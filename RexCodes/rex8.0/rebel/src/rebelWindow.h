#ifndef RebelWindow_H
#define RebelWindow_H

enum WinType{
	REBEL_WINDOW,
	REBEL_CHAIN,
	REBEL_MENU,
	REBEL_VARIABLE,
	REBEL_FUNCTION
};

template <class T>
class buttonLessThan : public binary_function<T, T, bool> {
 public:
	bool operator()(const T a, const T b) {
		if(*a < *b) return(true);
		else return(false);
	}
};

// forward declaration of rebelApp for the portWindow function
class rebelApp;

class rebelWin {
 public:
	rebelWin();
	rebelWin(const char *n);										// constructor
	rebelWin(const char *n, const PhDim_t size);					// constructor
	rebelWin(const rebelWin& rw);									// copy constructor
	virtual ~rebelWin();

	void newButton( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void resize( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	PhPoint_t origin();
	void winOpen( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void closeWin( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void openWin( PtWidget_t *widget, void *client_data, PtCallbackInfo_t *cbinfo);
	void initMove( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void newComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void addComment( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void activeButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void nextButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void prevButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void select();

	virtual void saveWindow(FILE *fp) = 0;
	virtual void readWindow() = 0;
	virtual void portWindow(rebelApp *app, string first, string second = "NULL") = 0;
	virtual void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ) = 0;

	virtual void addButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	virtual void cutButton(rebelButton *b);
	virtual void snapToGrid();
	virtual void groupButton(rebelButton *b, long int key_mods);
	virtual void ungroupButtons();
	virtual void eventBoundary( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	virtual void eventDrag( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int init( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static void resetNextOrigin();

	static int resizeCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int winOpenCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int closeWinCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int openWinCallback( PtWidget_t *widget, void *client_data, PtCallbackInfo_t *cbinfo);
	static int initMoveCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int eventCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int addButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int newCommentCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int activeButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int nextButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int prevButtonCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	inline char *name() { return(_name); }
	inline bool isRealized() { return(PtWidgetIsRealized(_window)); }
	inline PtWidget_t *window() { return(_window); }
	inline PtWidget_t *scrollContainer() { return(_scrollContainer); }
	inline vector<rebelButton *>buttons() { return(_buttons); }
	inline void origin(PhPoint_t o) { _origin = o; }
	inline void size(PhDim_t s) { _size = s; }
	inline PhDim_t size() { return(_size); }
	inline WinType winTyp() { return(_winTyp); }
	inline void deselect() {
		PtSetResource(_scrollContainer, Pt_ARG_FILL_COLOR, Pg_LGREY, 0);
		_addButton = false;
	}
	inline int numItems() { return(_buttons.size()); }
	inline char *lineTok(char *string) { return(_lineTok->tok(string)); }
	inline void extractString(string& line, string& tmp, string fd, string nd) {
		_lineTok->extractString(line, tmp, fd, nd);
	}
 protected:
	PhPoint_t getNextCell(PhPoint_t);
	void setButtonResources(vector<rebelButton *>::iterator bp);

	static int _gridVertStep;
	static int _gridHorzStep;
	static int _moveDamp;
	static PhPoint_t _nextOrigin;

	char _name[32];
	WinType _winTyp;
	PhPoint_t _origin;
	PhPoint_t _orgPos;
	PhPoint_t _newPos;
	PhDim_t _size;
	PhDim_t _prefSize;
	PtWidget_t *_window;
	PtWidget_t *_scrollContainer;
	PtWidget_t *_scrollCanvas;
	PhRect_t _scrollCanvasExtent;
	PtWidget_t *_draggedWidget;

	PtCallbackInfo_t _cbinfo;

	rebelLineTok *_lineTok;
	vector<rebelButton *>_buttons;
	vector<rebelButton *>_buttonGroup;
	vector<rebelButton *>::iterator _activeBtnPtr;

	bool _addButton;
	bool _addComment;

	vector<char> _sourceTemp;
 private:
};

#endif

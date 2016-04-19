#ifndef RebelButton_h
#define RebelButton_h

enum ButType{
	REBEL_BUTTON,
	REBEL_STATE,
	REBEL_MENU_ITEM,
	REBEL_VARIABLE_ITEM,
	REBEL_FUNTION_ITEM,
	REBEL_COMMENT
};

// forward declaration of rebelWindow for the port button function
class rebelWin;

class rebelButton {
 public:
	rebelButton();
	rebelButton(const char *name);
	rebelButton(const char *name, PhPoint_t pos);
	virtual ~rebelButton();
	bool operator==(const rebelButton& rs);								// equality operator
	bool operator<(const rebelButton& rs);								// less than operator
	bool operator>(const rebelButton& rs);								// greater than operator

	char *name();
	void origin(PhPoint_t o);

	virtual void resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ) = 0;
	virtual void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo ) = 0;
	virtual void saveButton(FILE *fp) = 0;
	virtual void genCode(FILE *fp) = 0;
	virtual void portButton(rebelWin *w, string line = "NULL") = 0;

	inline PhPoint_t origin() { return(_origin); }
	inline PtWidget_t *button() { return(_button); }
	inline ButType butType() { return(_butType); }
 protected:
	char _name[30];
	ButType _butType;
	PhPoint_t _origin;
	PtWidget_t *_button;

 private:
};

#endif

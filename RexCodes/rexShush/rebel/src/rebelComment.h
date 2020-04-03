#ifndef RebelComment_h
#define RebelComment_h

class rebelComment : public rebelButton {
 public:
	rebelComment();
	rebelComment(PtWidget_t *parent, PhPoint_t pos);
	rebelComment(const rebelComment& c);
	virtual ~rebelComment();
	rebelComment& operator=(const rebelComment& c);

	void resource( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void text( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static int resourceCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int textCallback( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	void cut( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	void saveButton(FILE *fp);
	void readButton(string line);

	void genCode(FILE *fp);
	void portButton(rebelWin *w, string line = "NULL");

	char *comment() { return(&_comment[0]); }
 protected:
 private:
	vector<char> _comment;
};

#endif

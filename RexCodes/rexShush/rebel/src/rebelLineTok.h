#ifndef RebelLineTok_h
#define RebelLineTok_h

class rebelLineTok {
 public:
	rebelLineTok();
	virtual ~rebelLineTok();

	char *tok(char *string);
	void extractString(string& line, string& tmp, string fd, string nd);
 protected:
 private:
	char *_currentPosition;
	char *_endPosition;
	char _returnString[1024];
};

#endif

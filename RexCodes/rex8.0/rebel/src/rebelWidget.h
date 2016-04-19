#ifndef RebelWidget_H
#define RebelWidget_H

class rebelWidget {
 public:
	rebelWidget();
	virtual ~rebelWidget();

	static int init( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	void origin(PhPoint_t o) { _origin = o; }
	void size(PhDim_t s) { _size = s; }

 protected:
 private:
	PtWidget_t *_window;
	PhPoint_t _origin;
	PhDim_t _size;
};
#endif

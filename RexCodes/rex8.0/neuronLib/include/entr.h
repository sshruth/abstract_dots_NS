typedef struct ipt {
	long iptoff;
	long ipttime;
	int pdgm;
} IPT;

struct where {
	int w_code;
	long location;
	long etime;
};

IPT *fill_ilist();


int hwd_off(long hwdnum);
int hwd_on(long hwdnum);
int hwd_setPosition(long hwdnum, long x, long y, long z);
int hwd_setPositionDegrees(long hwdnum, long x, long y);
int hwd_init(void);

void inputPolaris(char *buf);
void loadPolarisPlexonBuf(int f, int err, int x, int y, int z);



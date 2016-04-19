// timerFunction.h: interface for the timerFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMERFUNCTION_H__35189260_14F9_11D3_B2D9_00104B93918B__INCLUDED_)
#define AFX_TIMERFUNCTION_H__35189260_14F9_11D3_B2D9_00104B93918B__INCLUDED_

#define MINLONG     0x80000000  
#define MAXLONG     0x7fffffff  

typedef struct {
        double centerTime;
        double minTime;
        double maxTime;
        double leftEdge;
        double rightEdge;
        int numberTimes;
} TFBIN;

typedef struct {
	char label[64];
        uint64_t tfStartTime;
        int numBins;
        int maxBins;
        double binWidth;
        TFBIN *tfBins;
} TFHISTO;

// public function declarations
void printTimes(void);
void stopTimer(int index);
void startTimer(int index);
void initThisTimerHisto(int index, char *label, int bins, int width);
void timerFunction(int number);
void clearTimerFunction(void);

#endif // !defined(AFX_TIMERFUNCTION_H__35189260_14F9_11D3_B2D9_00104B93918B__INCLUDED_)

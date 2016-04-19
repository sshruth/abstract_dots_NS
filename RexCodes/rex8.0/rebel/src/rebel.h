/* Header "rebel.h" for rebel Application */
#ifndef Rebel_H
#define Rebel_H

#include <errno.h>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

#include <photon/keycodes.h>
#include "rebelButton.h"
#include "rebelState.h"
#include "rebelMenuItem.h"
#include "rebelVariableItem.h"
#include "rebelFunctionItem.h"
#include "rebelComment.h"
#include "rebelLineTok.h"
#include "rebelApp.h"
#include "rebelWindow.h"
#include "rebelChain.h"
#include "rebelMenu.h"
#include "rebelFunction.h"
#include "rebelVariable.h"
#include "rebelEdit.h"
#include "rebelResource.h"
#include "rebelWidget.h"
#include "rebelFile.h"

/*
 * Return argument for 'ra_compute_time()' function.
 */
typedef struct {
    long    ra_ramp_time_preset;	/* preset */
    int	    ra_ramp_time_random;	/* random */
} RA_RAMP_TIME;

#define REBEL 1
#define SPOT 2

#define REBEL_FLAG1 127
#define REBEL_FLAG2 128

#endif

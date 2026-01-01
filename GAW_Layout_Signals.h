/* ------------------------------------------------------------------------- *
 * Project: GAW_Layout_Signals.h
 * Author : Gerard Wassink
 * Date   : December 2025
 * 
 * File   : GAW_Layout Signals.h
 *            
 *------------------------------------------------------------------------- */

#include "LibPrintf.h"

#ifndef GAW_LAYOUT_SIGNALS
#define GAW_LAYOUT_SIGNALS

enum signalFace { OCCUPIED, CLEAR };        // Enumerated values

void signal(int signalNum, signalFace face);

#endif
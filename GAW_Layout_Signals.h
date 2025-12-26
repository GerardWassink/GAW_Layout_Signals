/* ------------------------------------------------------------------------- *
 * Project: GAW_Layout_Signals.h
 * Author : Gerard Wassink
 * Date   : December 2025
 * 
 * File   : GAW_Layout Signals.h
 *            
 *------------------------------------------------------------------------- */

#ifndef GAW_LAYOUT_SIGNALS
#define GAW_LAYOUT_SIGNALS

/* ------------------------------------------------------------------------- *
 *                                                        DEBUGGING ON / OFF
 * Compiler directives to switch debugging on / off
 * Do not enable debug when not needed, Serial takes space and time!
 * ------------------------------------------------------------------------- */
#define DEBUG 0

#if DEBUG == 1
#define debugstart(x) Serial.begin(x)
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debugstart(x)
#define debug(x)
#define debugln(x)
#endif

enum signalFace { OCCUPIED, CLEAR };        // Enumerated values

void signal(int signalNum, signalFace face);

#endif
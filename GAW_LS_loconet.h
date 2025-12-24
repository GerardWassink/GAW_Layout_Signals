/* ------------------------------------------------------------------------- *
 * Project: GAW_Layout_Signals
 * Author : Gerard Wassink
 * Date   : December 2025
 * 
 * File   : GAW_LS_loconet.cpp
 * 
 * ------------------------------------------------------------------------- */

#include <Arduino.h>
#include "GAW_Layout_Signals.h"

#ifndef LOCONET_H
#define LOCONET_H

void executeSignalAction(uint16_t Address, uint8_t Direction, uint8_t Output);
void processLocoNetMessage();
void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction );

#endif

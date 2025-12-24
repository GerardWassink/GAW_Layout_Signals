/* ------------------------------------------------------------------------- *
 * Project: GAW_Layout_Signals
 * Author : Gerard Wassink
 * Date   : December 2025
 * 
 * File   : GAW_LS_loconet.cpp
 * 
 * ------------------------------------------------------------------------- */

#include <LocoNet.h>
#include "GAW_Layout_Signals.h"

lnMsg *LnPacket;


/* ------------------------------------------------------------------------- *
 *                                                     executeSignalAction()
 * ------------------------------------------------------------------------- */
void executeSignalAction(uint16_t Address, uint8_t Direction, uint8_t Output)
{
  Direction == THROWN ? signal(Address, OCCUPIED) : signal(Address, CLEAR);
}



/* ------------------------------------------------------------------------- *
 *                                                   processLocoNetMessage()
 * ------------------------------------------------------------------------- */
void processLocoNetMessage()
{
  LnPacket = LocoNet.receive();
  if (LnPacket) LocoNet.processSwitchSensorMessage(LnPacket);
}



/* ------------------------------------------------------------------------- *
 *                                                     notifySwitchRequest()
 * This call-back function is called from LocoNet.processSwitchSensorMessage
 * for all Switch Request messages
 * ------------------------------------------------------------------------- */
void notifySwitchRequest( uint16_t Address, uint8_t Output, uint8_t Direction )
{
  Serial.print(F("Signal Request: "));
  Serial.print(Address, DEC);
  Serial.print(F(" - "));
  Serial.print(Direction ? F("Clear") : F("Occupied"));
  Serial.print(F(" - "));
  Serial.println(Output ? F("On") : F("Off"));  

  executeSignalAction(Address, Direction, Output);
}



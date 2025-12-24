/* ------------------------------------------------------------------------- *
 * Name   : GAW_Layout_Signals
 * Author : Gerard Wassink
 * Date   : December 2025
 * Purpose: Test model railroad signals over I2C
 * Versions:
 *   0.1  : Initial code base
 *   0.2  : Code improved
 *            modules now in array
 *            simplified the code tremendously
 *   0.3  : Loconet built in for two aspect signals
 *            working with switch addresses
 *            
 *------------------------------------------------------------------------- */
#define progVersion "0.3"  // Program version definition
/* ------------------------------------------------------------------------- *
 *             GNU LICENSE CONDITIONS
 * ------------------------------------------------------------------------- *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * ------------------------------------------------------------------------- *
 *       Copyright (C) May 2024 Gerard Wassink
 * ------------------------------------------------------------------------- */

#include <LocoNet.h>                        // Loconet library
#include <EEPROM.h>                         // EEPROM library
#include <Adafruit_MCP23X17.h>              // MCP23017 mux library library

#include "GAW_LS_loconet.h"
#include "GAW_Layout_Signals.h"


/* ------------------------------------------------------------------------- *
 * Below:
 * 1. define the number of MCP23017 chips you want to address in numMcps
 * 2. comment out the not used chips
 * 3. change the mcp[] array to contain the first number of chips 
 * 4. change the addresses in the mcpAdr[] array in the order they will 
 *      be addressed
 * ------------------------------------------------------------------------- */
#define numMcps 6                           // number of chips in use

Adafruit_MCP23X17 mcp0;                     // Individual chip definitions
Adafruit_MCP23X17 mcp1;                     // Comment out not used spots
Adafruit_MCP23X17 mcp2;                     //  "
Adafruit_MCP23X17 mcp3;                     //   "
Adafruit_MCP23X17 mcp4;                     //    "
Adafruit_MCP23X17 mcp5;                     //     "
//Adafruit_MCP23X17 mcp6;                     //      "
//Adafruit_MCP23X17 mcp7;                     //       "


/* ------------------------------------------------------------------------- *
 *                                            Arrays for chips and addresses
 * ------------------------------------------------------------------------- */
Adafruit_MCP23X17 mcp[numMcps] =  { mcp0, mcp1, mcp2, mcp3, mcp4, mcp5 };
int mcpAdr[numMcps] =  { 0x24, 0x25, 0x26, 0x27, 0x20, 0x21 }; 


/* ------------------------------------------------------------------------- *
 *                                            Base signal addresses per chip
 * each chip can handle 8 signals
 * per layout module we allow for 16 signals
 * ------------------------------------------------------------------------- */
uint16_t baseAdr[numMcps] = {130, 230, 330, 430, 530, 630 }; // 730, 830...


/* ------------------------------------------------------------------------- *
 *                                                   Initial routine setup()
 * ------------------------------------------------------------------------- */
void setup() {

  delay(1000);                              // delay before start

  LocoNet.init();                           // Initialize the LocoNet interface

  debugstart(57600);

  debugln("---===### START PROGRAM ###===---");
  debug("GAW_Layout_Signals v");
  debugln(progVersion);
  debugln();


  debugln("---===### Initalizing Multiplexers ###===---");
  for (int i = 0; i < numMcps; i++) {
    debug("Initalizing I2C module "); debug(i); debug(": addr="); debugln(mcpAdr[i]);
    if ( !mcp[i].begin_I2C(mcpAdr[i]) ) {
      debug("mcp"); debug(i); debugln(" init error");
      while(1);
    }
  }

  debugln("Setting all pins to OUTPUT and switch them all off");
  debug("For module: ");
  for (int m = 0; m < numMcps; m++) {
                    debug(m); debug(", ");
    for (int i = 0; i<16; i++) {
      mcp[m].pinMode(i, OUTPUT);
      mcp[m].digitalWrite(i, LOW);
    }
  }
  debugln();

  debugln(); debugln("---===### INIT COMPLETE ###===---");

}



/* ------------------------------------------------------------------------- *
 *                                                     Repeating code loop()
 * Go look if there are LocoNet messages to process
 * ------------------------------------------------------------------------- */
void loop() {
	processLocoNetMessage();                  // Process Loconet commands
}



/* ------------------------------------------------------------------------- *
 *                                                                  signal()
 * Set a signal to RED (occupied) or GREEN (clear)
 * ------------------------------------------------------------------------- */
void signal(int signalNum, signalFace face) {

  int signal = signalNum - 1;               // to offset zero
  int module = signal >> 3;                 // which module?
  int port   = (signal << 1) % 16;          // determine port

  switch (face) {

    case CLEAR:
      mcp[module].digitalWrite(port, HIGH);
      mcp[module].digitalWrite(port+1, LOW);
      break;

    case OCCUPIED:
      mcp[module].digitalWrite(port, LOW);
      mcp[module].digitalWrite(port+1, HIGH);
      break;

    default:
      break;

  }

}

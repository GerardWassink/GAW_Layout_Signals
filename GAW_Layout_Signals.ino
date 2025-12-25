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
 *   1.0  : Translation from sigal addresses to module and port numbers
 *          Prepared for my four modules with designated signal addresses
 *          Code cleanup
 *          
 *          Created relase 1.0
 *          
 *------------------------------------------------------------------------- */
#define progVersion "1.0"  // Program version definition
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
 * On every layout module there will be one arduino operating the signals
 * Define the module for which the definitions below are meant
 * ------------------------------------------------------------------------- */
#define MODULE5
//#define MODULE6
//#define MODULE7
//#define MODULE8


/* ------------------------------------------------------------------------- *
 * Below:
 * 1. define the number of MCP23017 chips you want to address in numMcps
 *      (two per MCP23017 board, see README)
 * 2. comment out the not used chips
 * 3. change the mcp[] array to contain the number of chips 
 * 4. change the addresses in the mcpAdr[] array in the order they will 
 *      be addressed
 * ------------------------------------------------------------------------- */
#define numMcps   4                         // number of MCP23017 chips in use
#define numPerMcp 8                         // number of signals per mcp

Adafruit_MCP23X17 mcp0;                     // Individual chip definitions
Adafruit_MCP23X17 mcp1;                     // Comment out not used spots
Adafruit_MCP23X17 mcp2;                     //  "
Adafruit_MCP23X17 mcp3;                     //   "


/* ------------------------------------------------------------------------- *
 *                                            Arrays for chips and addresses
 * ------------------------------------------------------------------------- */
Adafruit_MCP23X17 mcp[numMcps] =  { mcp0, mcp1, mcp2, mcp3 };
int mcpAdr[numMcps] =  { 0x24, 0x25, 0x26, 0x27 }; 

/* ------------------------------------------------------------------------- *
 *                                                 Signal addresses per chip
 * each chip can handle 8 signals
 * on each layout module we allow for max 32 signals (i.e. 2 MCP23017 boards)
 *
 * ==>> signal addresses start at M30, M being the layout module number <<==
 *
 * ------------------------------------------------------------------------- */
#ifdef MODULE5
int signals[numMcps][8] = { {530, 531, 532, 533, 534, 535, 536, 537},
                            {538, 539, 540, 541, 542, 543, 544, 545},
                            {546, 547, 548, 549, 550, 551, 552, 553},
                            {554, 555, 556, 557, 558, 559, 560, 561}
                          };
#endif

#ifdef MODULE6
int signals[numMcps][8] = { {630, 631, 632, 633, 634, 635, 636, 637},
                            {638, 639, 640, 641, 642, 643, 644, 645},
                            {646, 647, 648, 649, 650, 651, 652, 653},
                            {654, 655, 656, 657, 658, 659, 660, 661}
                          };
#endif

#ifdef MODULE7
int signals[numMcps][8] = { {730, 731, 732, 733, 734, 735, 736, 737},
                            {738, 739, 740, 741, 742, 743, 744, 745},
                            {746, 747, 748, 749, 750, 751, 752, 753},
                            {754, 755, 756, 757, 758, 759, 760, 761}
                          };
#endif

#ifdef MODULE8
int signals[numMcps][8] = { {830, 831, 832, 833, 834, 835, 836, 837},
                            {838, 839, 840, 841, 842, 843, 844, 845},
                            {846, 847, 848, 849, 850, 851, 852, 853},
                            {854, 855, 856, 857, 858, 859, 860, 861}
                          };
#endif


/* ------------------------------------------------------------------------- *
 * Initial routine                                                   setup()
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
 * Repeating code                                                     loop()
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
  
  int port;

  for (int module = 0; module < numMcps; module++) {  // Walk through modules

    for (int n = 0; n < numPerMcp; n++) {             // Search signal in module

      if (signalNum == signals[module][n]) {          // found?

        port = n << 1;                                // Calculate port number

        debug("==> signalNum: "); debug(signalNum);
        debug(" module: ");       debug(module);
        debug(" port: ");         debug(port);      debugln();

        switch (face) {                               // What to do ?

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

        continue;                                     // break from loops

      }
    }
  }
}

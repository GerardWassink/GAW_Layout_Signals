/* ------------------------------------------------------------------------- *
 * Name   : GAW_Layout_Signals
 * Author : Gerard Wassink
 * Date   : December 2025
 * Purpose: Model railroad signals over I2C
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
 *   1.1  : Layout module definitions moved to seperate .h files
 *   1.2  : Built in the LibPrintf library to sustitute Serial.print
 *            and add the printf placeholders like %s, %i, %x etc.
 *          
 *------------------------------------------------------------------------- */
#define progVersion "1.2"  // Program version definition
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
 * depending on the number of siganls there will be one or two MCP23017 boards
 * with respectively 2 or 4 MCP23017 chips.
 * The definitions per layout module have been put into seperate .h files.
 * Define the layout module for which the definitions are meant below:
 * ------------------------------------------------------------------------- */
#define MODULE5
//#define MODULE6
//#define MODULE7
//#define MODULE8

/* ------------------------------------------------------------------------- *
 * Include the layout module definition
 * ------------------------------------------------------------------------- */
#if defined(MODULE5)
  #include "GAW_LS_Module_5.h"
#elif defined(MODULE6)
  #include "GAW_LS_Module_6.h"
#elif defined(MODULE7)
  #include "GAW_LS_Module_7.h"
#elif defined(MODULE8)
  #include "GAW_LS_Module_8.h"
#endif



/* ------------------------------------------------------------------------- *
 * Initial routine                                                   setup()
 * ------------------------------------------------------------------------- */
void setup() {

  delay(1000);                              // delay before start

  LocoNet.init();                           // Initialize the LocoNet interface

  Serial.begin(57600);

  printf("---===### START PROGRAM ###===---\n");
  printf("GAW_Layout_Signals v%s\n\n", progVersion);

  printf("---===### Initalizing Multiplexers ###===---\n");
  for (int i = 0; i < numMcps; i++) {
    printf("Initalizing I2C module %d: addr=0x%x\n", i, mcpAdr[i]);
    if ( !mcp[i].begin_I2C(mcpAdr[i]) ) {
      printf("mcp %i - init error\n", i);
      while(1);
    }
  }

  printf("\nSetting pins to OUTPUT and turn them off\n");
  printf("For modules: ");
  for (int m = 0; m < numMcps; m++) {
    printf("0x%x ", mcpAdr[m]);
    for (int i = 0; i<16; i++) {
      mcp[m].pinMode(i, OUTPUT);
      mcp[m].digitalWrite(i, LOW);
    }
  }
  printf("\n\n---===### INIT COMPLETE ###===---\n\n");

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

        printf("==> signalNum: %d module: 0x%02x port: %d\n", signalNum, mcpAdr[module], port);

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

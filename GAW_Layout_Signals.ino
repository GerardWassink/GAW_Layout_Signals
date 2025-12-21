/* ------------------------------------------------------------------------- *
 * Name   : GAW_Layout_Signals
 * Author : Gerard Wassink
 * Date   : December 2025
 * Purpose: Test model railroad signals over I2C
 * Versions:
 *   0.1  : Initial code base
 *------------------------------------------------------------------------- */
#define progVersion "0.1"  // Program version definition
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


/* ------------------------------------------------------------------------- *
 *                                                        DEBUGGING ON / OFF
 * Compiler directives to switch debugging on / off
 * Do not enable debug when not needed, Serial takes space and time!
 * ------------------------------------------------------------------------- */
#define DEBUG 1

#if DEBUG == 1
#define debugstart(x) Serial.begin(x)
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debugstart(x)
#define debug(x)
#define debugln(x)
#endif


//#include <Loconet.h>                        // Loconet library
#include <EEPROM.h>                         // EEPROM library
#include <Adafruit_MCP23X17.h>              // MCP23017 mux library library

#define mcp1Addr 0x24                       // address of first mux MCP23017
#define mcp2Addr mcp1Addr + 1               // address of second mux MCP23017
#define mcp3Addr mcp1Addr + 2               // address of third mux MCP23017
#define mcp4Addr mcp1Addr + 3               // address of fourth mux MCP23017

Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;
Adafruit_MCP23X17 mcp3;
Adafruit_MCP23X17 mcp4;

/* ------------------------------------------------------------------------- *
 *                                                   Initial routine setup()
 * ------------------------------------------------------------------------- */
void setup() {

  delay(1000);

  debugstart(57600);

  debugln("---===### START PROGRAM ###===---");
  debug("GAW_Layout_Signals v");
  debugln(progVersion);
  debugln();
  debugln("Initalizing Multiplexers");

  debugln("begin.wire mcp1");
  Wire.begin(mcp1Addr);

  if (!mcp1.begin_I2C(mcp1Addr)) {
    debugln("mcp1 init error");
    while(1);
  }

  debugln("begin.wire mpc2");
  Wire.begin(mcp2Addr);

  if (!mcp2.begin_I2C(mcp2Addr)) {
    debugln("mcp2 init error");
    while(1);
  }

  debugln("begin.wire mpc3");
  Wire.begin(mcp3Addr);

  if (!mcp3.begin_I2C(mcp3Addr)) {
    debugln("mcp3 init error");
    while(1);
  }

  debugln("begin.wire mpc4");
  Wire.begin(mcp4Addr);

  if (!mcp4.begin_I2C(mcp4Addr)) {
    debugln("mcp4 init error");
    while(1);
  }


  debugln("Setting all pins to OUTPUT and switch them all off");
  for (int i = 0; i<16; i++) {
    mcp1.pinMode(i, OUTPUT);
    mcp1.digitalWrite(i, LOW);

    mcp2.pinMode(i, OUTPUT);
    mcp2.digitalWrite(i, LOW);

    mcp3.pinMode(i, OUTPUT);
    mcp3.digitalWrite(i, LOW);

    mcp4.pinMode(i, OUTPUT);
    mcp4.digitalWrite(i, LOW);
  }

  debugln("---===### Set all signals Occupied ###===---");
  for (int i = 1; i<33; i++) {
    signalOccupied(i);
  }

  debugln("---===### Make signals 4 and 16 Clear ###===---");
  signalClear(1);
  signalClear(32);

  debugln(); debugln("---===### ALL DONE ###===---");

}



/* ------------------------------------------------------------------------- *
 *                                                     Repeating code loop()
 * ------------------------------------------------------------------------- */
void loop() {

//  signalClear(1);
//  delay(500);
//  signalOccupied(1);
//  delay(500);


}



/* ------------------------------------------------------------------------- *
 *                                                             SignalClear()
 * ------------------------------------------------------------------------- */
void signalClear(int signalNum) {

  int signal = signalNum - 1;               // to offset zero
  int module = signal >> 3;                 // which module?
  int port = (signal << 1) % 16 ;           // determine port

  debug(" sigNum: ");  debug(signalNum);   debug(": ");
  debug(" signal: "); debug(signal);      debug(":");
  debug(" module: "); debug(module);      debug(":");
  debug(" port: ");   debug(port);
  debugln(" - sigClean");

  switch (module) {
    case 0:
      mcp1.digitalWrite(port, HIGH);
      mcp1.digitalWrite(port+1, LOW);
      break;

    case 1:
      mcp2.digitalWrite(port, HIGH);
      mcp2.digitalWrite(port+1, LOW);
      break;

    case 2:
      mcp3.digitalWrite(port, HIGH);
      mcp3.digitalWrite(port+1, LOW);
      break;

    case 3:
      mcp4.digitalWrite(port, HIGH);
      mcp4.digitalWrite(port+1, LOW);
      break;

    default:
      break;
  }
}



/* ------------------------------------------------------------------------- *
 *                                                          SignalOccupied()
 * ------------------------------------------------------------------------- */
void signalOccupied(int signalNum) {

  int signal = signalNum - 1;               // to offset zero
  int module = signal >> 3;                 // which module?
  int port = (signal << 1) % 16 +1;         // determine port

  debug(" sigNum: "); debug(signalNum);   debug(": ");
  debug(" signal: "); debug(signal);      debug(":");
  debug(" module: "); debug(module);      debug(":");
  debug(" port: ");   debug(port);
  debugln(" - sigOccup");

  switch (module) {
    case 0:
      mcp1.digitalWrite(port, HIGH);
      mcp1.digitalWrite(port-1, LOW);
      break;

    case 1:
      mcp2.digitalWrite(port, HIGH);
      mcp2.digitalWrite(port-1, LOW);
      break;

    case 2:
      mcp3.digitalWrite(port, HIGH);
      mcp3.digitalWrite(port-1, LOW);
      break;

    case 3:
      mcp4.digitalWrite(port, HIGH);
      mcp4.digitalWrite(port-1, LOW);
      break;

    default:
      break;
  }
}

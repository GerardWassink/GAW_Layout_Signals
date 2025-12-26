/* ------------------------------------------------------------------------- *
 * Project: GAW_Layout_Signals.h
 * Author : Gerard Wassink
 * Date   : December 2025
 * 
 * File   : GAW_LS_Module_8.h
 * 
 *------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- *
 * -------------------================ MODULE 8 ==================---------- *
 * ------------------------------------------------------------------------- */
#define numMcps   2                         // number of MCP23017 chips in use
#define numPerMcp 8                         // number of signals per mcp

Adafruit_MCP23X17 mcp0;                     // Individual chip definitions
Adafruit_MCP23X17 mcp1;

/* ------------------------------------------------------------------------- *
 *                                            Arrays for chips and addresses
 * ------------------------------------------------------------------------- */
Adafruit_MCP23X17 mcp[numMcps] =  { mcp0, mcp1 };
int mcpAdr[numMcps] =  { 0x20, 0x21 }; 

int signals[numMcps][8] = { {830, 831, 832, 833, 834, 835, 836, 837},
                            {838, 839, 840, 841, 842, 843, 844, 845}
                          };

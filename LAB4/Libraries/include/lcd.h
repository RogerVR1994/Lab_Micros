#include <inttypes.h>

extern void lcdCommand( uint8_t command);
extern void lcdData( char data);
extern void lcdInit(void);
extern void lcdGotoYX(uint8_t y, uint8_t x);
extern void lcdPrintln(char msg[]);

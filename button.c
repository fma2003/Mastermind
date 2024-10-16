/* ***************************************************************************** */
/* You can use this file to define the low-level hardware control fcts for       */
/* LED, button and LCD devices.                                                  */ 
/* Note that these need to be implemented in Assembler.                          */
/* You can use inline Assembler code, or use a stand-alone Assembler file.       */
/* Alternatively, you can implement all fcts directly in master-mind.c,          */  
/* using inline Assembler code there.                                            */
/* The Makefile assumes you define the functions here.                           */
/* ***************************************************************************** */


#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(1==2)
#endif

#define	PAGE_SIZE		(4*1024)
#define	BLOCK_SIZE		(4*1024)

#define	INPUT			 0
#define	OUTPUT			 1

#define	LOW			 0
#define	HIGH			 1


// APP constants   ---------------------------------

// Wiring (see call to lcdInit in main, using BCM numbering)
// NB: this needs to match the wiring as defined in master-mind.c

// #define STRB_PIN 24
// #define RS_PIN   25
// #define DATA0_PIN 23
// #define DATA1_PIN 10
// #define DATA2_PIN 27
// #define DATA3_PIN 22

// -----------------------------------------------------------------------------
// includes 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>

// -----------------------------------------------------------------------------
// prototypes

int failure (int fatal, const char *message, ...);

// -----------------------------------------------------------------------------
// Functions to implement here (or directly in master-mind.c)

/* this version needs gpio as argument, because it is in a separate file */


// adapted from setPinMode
// void pinMode(uint32_t *gpio, int pin, int mode /*, int fSel, int shift */) {
//   /* ***  COMPLETE the code here, using inline Assembler  ***  */
// }



void ledpinMode(uint32_t *gpio, int pin, int mode /*, int fSel, int shift */) {
  
  int offset = pin / 10;
  int shift = (pin % 10) * 3;
  int r;

  // Set the GPIO pin mode
  asm(  
  "\tLDR R0, %[gpi]\n"  // R0 = gpio
  "\tMOV R1, #1\n"      // R1 = 1
  "\tLSL R1, %[offset]\n" // R1 = 1 << offset
  "\tADD R0, R1\n"      // R0 = gpio + (1 << offset)

  "\tLDR R1, [%[gpo]]\n" // R1 = *R0
  "\tMOV R2, #7\n"      // R2 = 7
  "\tLSL R2, %[shift]\n" // R2 = 7 << shift
  "\tBIC R1, R2\n"      // R1 = R1 & ~(7 << shift)
  "\tMOV R2, %[mode]\n" // R2 = mode
  "\tLSL R2, %[shift]\n" // R2 = mode << shift
  "\tORR R1, R2\n"      // R1 = R1 | (mode << shift)
  "\tSTR R1, [%[gpo]]\n" // *R0 = R1
  : /* no output */
  : [gpi] "m" (gpio),
  [offset] "r" (offset),
  [gpo] "r" (gpio + offset), // offset for setting the mode
  [shift] "r" (shift),
  [mode] "r" (mode)
  : "r0", "r1", "r2", "cc", "memory"
  );
}

void writeLED(uint32_t *gpio, int pin, int value) {
  
    if(value == 1){
        if (pin == 5) {
            asm(
                "\tMOV R1, %[gpio] \n" //move gpio address to R1
                "\tMOV R2, #0b1 \n" //move 1 to R2
                "\tLSL R2, #5 \n" 
                "\tSTR R2, [R1, #28] \n" //set pin 5 to 1
                : //no output
                : [gpio] "r" (gpio) //input
                : "r1", "r2", "cc" //registers
            );
        } else if (pin == 13) { //same as above but for pin 13
            asm(
                "\tMOV R1, %[gpio] \n" 
                "\tMOV R2, #0b1 \n" 
                "\tLSL R2, #13 \n" 
                "\tSTR R2, [R1, #28] \n"
                :
                : [gpio] "r" (gpio)
                : "r1", "r2", "cc" //registers
            );
        }
    }else{
        if (pin == 5) { //same as above but for pin 5
            asm(
                "\tMOV R1, %[gpio]\n"
                "\tMOV R2, #0b1\n"
                "\tLSL R2, #5\n"
                "\tSTR R2, [R1, #40]\n"
                :
                : [gpio] "r" (gpio)
                : "r1", "r2", "cc" //registers
            );
        } else if (pin == 13) { //same as above but for pin 13
            asm(
                "\tMOV R1, %[gpio]\n"
                "\tMOV R2, #0b1\n"
                "\tLSL R2, #13\n"
                "\tSTR R2, [R1, #40]\n"
                :
                : [gpio] "r" (gpio)
                : "r1", "r2", "cc" //registers
            );
        }
    }
}

int readButton(uint32_t *gpio, int button) {
  int offset = ((button / 32) + 13) * 4; // offset for reading the input
    int pinSet = button % 32; // pin to be read
    int r; // return value
    asm(
        "\tLDR R0, %[gpi]\n" // R0 = gpio
        "\tMOV R1, R0\n" // R1 = R0
        "\tADD R1, %[offset]\n"  // R1 = R0 + offset
        "\tLDR R1, [R1]\n"  // R1 = *R1
        "\tMOV R2, #1\n"  // R2 = 1
        "\tLSL R2, %[pinShift]\n" // R2 = 1 << pinShift
        "\tAND %[r], R2, R1\n"  // r = R2 & R1  
        : [r] "=r" (r)  // output
        : [gpi] "m" (gpio), // input
          [offset] "r" (offset),   
          [pinShift] "r" (pinSet)
        : "r0", "r1", "r2", "cc", "memory"
    );

    if (r != 0) // if r is not 0
        return 1; // return 1 
    else  
        return 0; // else return 0
}

void waitForButton(uint32_t *gpio, int button) {
  while (readButton(gpio, button) == 0) {
    // wait
  }

}

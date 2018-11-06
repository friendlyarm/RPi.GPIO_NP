/*
Copyright (c) 2013-2014 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Python.h"
#include "c_gpio.h"
#include "common.h"

int gpio_mode = MODE_UNKNOWN;

const int physToGpio_neo [MAX_PIN_COUNT] ={
    -1, 
    /* 24 Pin */
    -1, -1,   //1, 2
    12, -1,   //3, 4
    11, -1,   //5, 6
    203, 198, //7, 8
    -1,  199, //9, 10
    0,   6,   //11, 12
    2,   -1,  //13, 14
    3,  200,  //15, 16
    -1, 201,  //17, 18
    64, -1,   //19, 20
    65, 1,    //21, 22
    66, 67,   //23, 24

    /* 12 Pin */
    -1, -1,   //25, 26
    -1, -1,   //27, 28
    -1, -1,   //29, 30
    17, -1,   //31, 32
    -1, -1,   //33, 34
    -1, -1,   //35, 36

    /* UART0 Tx,Rx */
    4, 5,     //37, 38

    /* 39~63 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    /* 64~73 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};



const int physToGpio_neocore [MAX_PIN_COUNT] ={
    -1, 
    /* GPIO-1 24Pin */
    -1, -1,   //1, 2
    12, -1,   //3, 4
    11, -1,   //5, 6
    203, 198, //7, 8
    -1,  199, //9, 10
    0,   6,   //11, 12
    2,   -1,  //13, 14
    3,  200,  //15, 16
    -1, 201,  //17, 18
    64, -1,   //19, 20
    65, 1,    //21, 22
    66, 67,   //23, 24

/* GPIO-2 24Pin */
  -1,  15,   //25, 26  -> 1, 2
  -1,  16,   //27, 28  -> 3, 4
  -1,  14,   //29, 30  -> 5, 6
  -1,  13,   //31, 32  -> 7, 8
  -1,  -1,   //33, 34  -> 9, 10
  -1,  -1,   //35, 36  -> 11, 12
  17,  -1,   //37, 38  -> 13, 14
  -1,  -1,   //39, 40  -> 15, 16
  -1,   5,   //41, 42  -> 17, 18
  -1,   4,   //43, 44  -> 19, 20
  -1,  -1,   //45, 46  -> 21, 22
  -1,  -1,   //47, 48  -> 23, 24

 /* GPIO-3 24Pin */
  -1,  -1,   //49, 50  -> 1, 2
  -1,  -1,   //51, 52  -> 3, 4
  -1,  -1,   //53, 54  -> 5, 6
  -1,  -1,   //55, 56  -> 7, 8
  -1,  -1,   //57, 58  -> 9, 10
  -1,  -1,   //59, 60  -> 11, 12
  -1,   7,   //61, 62  -> 13, 14
  -1,  -1,   //63, 64  -> 15, 16
  -1,  -1,   //65, 66  -> 17, 18
  -1,  -1,   //67, 68  -> 19, 20
  -1,  -1,   //69, 70  -> 21, 22
  -1,  -1,   //71, 72  -> 23, 24
  -1,        //73
};

const int physToGpio_m1 [MAX_PIN_COUNT] ={
    -1, // 0
    -1, -1, // 1, 2
    12, -1, // 3, 4
    11, -1, // 5, 6
    203, 198, // 7, 8
    -1, 199, // 9, 10
    0, 6, //11, 12
    2, -1, //13, 14
    3, 200, //15, 16
    -1, 201, //17, 18
    64, -1, //19, 20
    65, 1, //21, 22
    66, 67, //23, 24
    -1, 17, //25, 26
    19, 18, //27, 28
    20, -1, //29, 30
    21, 7, //31, 32
    8, -1, //33, 34
    16, 13, //35, 36
    9, 15, //37, 38
    -1, 14, //39, 40
    -1, -1, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //41-> 55
    -1, -1, -1, -1, -1, -1, -1, -1 // 56-> 63

    /* 64~73 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

// phys pin number to /sys/gpio number
const int physToGpio_duo [MAX_PIN_COUNT] ={
   -1,        //0
 /* 32 Pin */
   -1, -1,   //1, 2
   -1, -1,   //3, 4
   -1, -1,   //5, 6
   -1, -1,   //7, 8
   -1, -1,   //9, 10
   198,363,   //11, 12
   199,-1,   //13, 14
   15, -1,   //15, 16
   16, -1,   //17, 18
   14, -1,   //19, 20
   13, 203,  //21, 22
   12, -1,  //23, 24
   11, -1,   //25, 26
   -1, -1,   //27, 28
   4,  -1,   //29, 30
   5,  -1,   //31, 32
   /* ---------nanopi duo end----------- */

    -1, -1,   //33, 34
    -1, -1,   //35, 36

    /* UART0 Tx,Rx */
    -1, -1,     //37, 38

    /* 39~63 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    /* 64~73 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};


// phys pin number to /sys/gpio number
const int physToGpio_duo2 [MAX_PIN_COUNT] ={
   -1,        //0
 /* 32 Pin */
   -1,   5,   //1, 2
   -1,   4,   //3, 4
   -1,  -1,   //5, 6
   -1,  11,   //7, 8
  363,  12,   //9, 10
  203,  13,   //11, 12
   -1,  14,   //13, 14
   -1,  16,   //15, 16
   -1,  15,   //17, 18
   -1, 199,   //19, 20
   -1, 198,  //21, 22
   -1,  -1,  //23, 24
   -1,  -1,   //25, 26
   -1,  -1,   //27, 28
   -1,  -1,   //29, 30
   -1,  -1,   //31, 32
   /* ---------nanopi duo end----------- */

    -1, -1,   //33, 34
    -1, -1,   //35, 36

    /* UART0 Tx,Rx */
    -1, -1,     //37, 38

    /* 39~63 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

    /* 64~73 */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

/*
const int pinTobcm_BP [MAX_PIN_COUNT] =	//BCM MODE
{
	-1,-1, //map to BCM GPIO0,1
	53,52, //map to BCM GPIO2,3
	259,-1, //map to BCM GPIO4,5
	-1,270, //map to BCM GPIO6,7
	266,269, //map to BCM GPIO8,9
	268,267, //map to BCM GPIO10,11
	-1,-1, //map to BCM GPIO12,13
	224,225, //map to BCM GPIO14,15
	-1,275, //map to BCM GPIO16,17
	226,-1, //map to BCM GPIO18,19
	-1, -1, //map to BCM GPIO20,21
	273,244, //map to BCM GPIO22,23
	245,272, //map to BCM GPIO24,25
	-1, 274, //map to BCM GPIO26,27
	
	-1,-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 29... 44
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //45... 60
	-1, -1, -1, -1 // ...63
} ;
*/

/*
const int physToGpioR3 [MAX_PIN_COUNT] =//head num map to BCMpin
{
	-1, // 0
	-1, -1, // 1, 2
	2, -1,
	3, -1,
	4, 14,
	-1, 15,
	17, 18,
	27, -1,
	22, 23,
	-1, 24,
	10, -1,
	9, 25,
	11, 8,
	-1, 7, // 25, 26
	
	0, 1, //27, 28
	5, -1, //29, 30
	6, 12, //31, 32
	13, -1, //33, 34
	19, 16, //35, 36
	26, 20, //37, 38
	-1, 21, //39, 40
	// Padding:
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 56
	-1, -1, -1, -1, -1, -1, -1, // ... 63
} ;
*/

int setup_error = 0;
int module_setup = 0;
int revision = -1;

extern int nanopi_found;

int check_gpio_priv(void)
{
    // check module has been imported cleanly
    if (setup_error)
    {
        PyErr_SetString(PyExc_RuntimeError, "Module not imported correctly!");
        return 1;
    }

    // check mmap setup has worked
    if (!module_setup)
    {
        PyErr_SetString(PyExc_RuntimeError, "No access to /dev/mem.  Try running as root!");
        return 2;
    }
    return 0;
}

int is_valid_raw_port(int channel)
{
    if (channel >=   0 && channel <  18) return 1; // PA
    if (channel >=  32 && channel <  56) return 2; // PB
    if (channel >=  64 && channel <  89) return 3; // PC
    if (channel >=  96 && channel < 124) return 4; // PD
    if (channel >= 128 && channel < 140) return 5; // PE
    if (channel >= 160 && channel < 166) return 6; // PF
    if (channel >= 192 && channel < 204) return 7; // PG
    if (channel >= 224 && channel < 252) return 8; // PH
    if (channel >= 256 && channel < 278) return 9; // PI
    return 0;
}

int get_gpio_number(int channel, unsigned int *gpio)
{
    // check channel number is in range
    if ( (gpio_mode == BCM && (channel < 0 || channel > MAX_PIN_COUNT))
      || (gpio_mode == BOARD && (channel < 1 || channel > MAX_PIN_COUNT)) )
    {
        PyErr_SetString(PyExc_ValueError, "The channel sent is invalid on a NanoPi");
        return 4;
    }

    // convert channel to gpio
    if (gpio_mode == BOARD)
    {
    if (*(*pin_to_gpio+channel) == -1)
        {
            PyErr_SetString(PyExc_ValueError, "The channel sent is invalid on a NanoPi");
            return 5;
        } else {
            *gpio = *(*pin_to_gpio+channel);	//pin_to_gpio is initialized in py_gpio.c, the last several lines
        }
    }
    // else if (gpio_mode == BCM)
    // {
    //      *gpio = *(pinTobcm_BP + channel); 
    // }
    else if (gpio_mode == MODE_RAW)
    {
        if (!is_valid_raw_port(channel))
        {
            PyErr_SetString(PyExc_ValueError, "The channel sent does not exist");
            return 5;
        }
        *gpio = channel;
    } else {
        // setmode() has not been run
        PyErr_SetString(PyExc_RuntimeError, "Please set pin numbering mode using GPIO.setmode(GPIO.BOARD) or GPIO.setmode(GPIO.RAW)");
        return 3;
    }
    
	if(nanopiDebug)
		printf("GPIO = %d\n", *gpio);
    return 0;
}

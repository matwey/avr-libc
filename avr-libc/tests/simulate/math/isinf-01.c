/* Test of isinf() function.
   $Id$
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "progmem.h"

#ifndef	__AVR__
# define PRINTFLN(fmt, ...)	\
    printf ("\nLine %d: " fmt "\n", __LINE__, ##__VA_ARGS__)
# define EXIT(code)	exit ((code) < 255 ? (code) : 100 + (code) % 100)
#else
# define PRINTFLN(args...)
# define EXIT	exit
#endif

union lofl_u {
    long lo;
    float fl;
};

/* Result is placed into SRAM variable, allocated at the start of
   memory. This is convinient to debug: read a core dump.	*/
volatile int v = 1;

PROGMEM const struct {		/* Table of test cases.	*/
    union lofl_u x;		/* argument	*/
    int z;			/* result	*/
} t[] = {

    /* Zero	*/
    { { .fl= +0.0 },	0 },
    { { .fl= -0.0 },	0 },
    
    /* A few of normal values	*/
    { { 0x00800000 },	0 },
    { { 0x00800001 },	0 },
    { { 0x00ffffff },	0 },
    { { 0x3f800000 },	0 },
    { { 0x7f7fffff },	0 },
    { { 0x80800000 },	0 },
    { { 0x80800001 },	0 },
    { { 0x80ffffff },	0 },
    { { 0xdf800000 },	0 },
    { { 0xff7fffff },	0 },
    
    /* Subnormal	*/
    { { 0x00000001 }, 0 },
    { { 0x00000100 }, 0 },
    { { 0x00010000 }, 0 },
    { { 0x007fffff }, 0 },
    { { 0x80000001 }, 0 },
    { { 0x80000100 }, 0 },
    { { 0x80010000 }, 0 },
    { { 0x807fffff }, 0 },

    /* Inf	*/
    { { 0x7f800000 },	1 },
    { { 0xff800000 },  -1 },    

    /* NaN	*/
    { { 0x7f800001 },	0 },
    { { 0x7fc00000 },	0 },
    { { 0x7fffffff },	0 },
    { { 0xff800001 },	0 },
    { { 0xffc00000 },	0 },
    { { 0xffffffff },	0 },
};

int main ()
{
    union lofl_u x;
    int z;
    int i;

    /* Default implementation.	*/    
    for (i = 0; i < (int) (sizeof(t) / sizeof(t[0])); i++) {
	x.lo = pgm_read_dword (& t[i].x);
	z = pgm_read_word (& t[i].z);
	v = isinf (x.fl);
	if (v != z) {
	    PRINTFLN ("i= %d  v= %d", i, v);
	    EXIT (i + 1);
	}
    }

#ifdef	__AVR__
    {
	int (* volatile vp)(double);
	/* Force to use the library implementation.	*/
	vp = & isinf;
	for (i = 0; i < (int) (sizeof(t) / sizeof(t[0])); i++) {
	    x.lo = pgm_read_dword (& t[i].x);
	    z = pgm_read_word (& t[i].z);
	    v = vp (x.fl);
	    if (v != z) {
		PRINTFLN ("i= %d  v= %d", i, v);
		EXIT (i + 101);
	    }
	}
    }
#endif

    return 0;
}

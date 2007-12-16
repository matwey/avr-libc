/* Test of isprint() function.
   $Id$
 */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifndef	__AVR__
# include <stdio.h>
# define PRINTFLN(fmt, ...)	\
    printf ("\nLine %d: " fmt "\n", __LINE__, ##__VA_ARGS__)
# define EXIT(code)	exit ((code) < 255 ? (code) : 100 + (code) % 100)
#else
# define PRINTFLN(args...)
# define EXIT	exit
#endif

int main ()
{
    int i;
    
    for (i = -1; i < 256; i++) {
	if (i >= ' ' && i <= 0x7E) {
	    if (!isprint (i)) {
		PRINTFLN ("isprint(%#x) --> false", i);
		EXIT (__LINE__);
	    }
	} else {
	    if (isprint (i)) {
		PRINTFLN ("isprint(%#x) --> true", i);
		EXIT (__LINE__);
	    }
	}
    }
    

/* Skip the host, as according to C standart it is not safety to use an
   argument beyound -1..255 value. Avr-libc's ctype functions permit
   this.	*/
#ifdef	__AVR__
    {
	unsigned int u;
	for (u = 0xffff; u > 0xff; u--)
	    if (isprint (u))
		EXIT (__LINE__);
    }
#endif

    return 0;
}

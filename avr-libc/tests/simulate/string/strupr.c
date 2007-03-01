/* $Id$	*/

#ifndef __AVR__
# include <ctype.h>
# include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "progmem.h"

#ifndef	__AVR__			/* strupr() is't a standart function	*/
char * strupr (char *s)
{
    char *p = s;
    while (*p) {
	*p = toupper(*p);
	p++;
    }
    return s;
}
#endif

void Check (int line, const char *s, const char *expect)
{
    char t[200];
    char *p;
    int code;

    if (strlen_P(s) > sizeof(t) - 1)
	exit (1);
    strcpy_P (t, s);
    p = strupr (t);
    if (p != t)
	code = line + 1000;
    else if (strcmp_P (t, expect))
	code = line;
    else
	return;
#if	!defined(__AVR__)
    printf ("\nLine %d: expect: \"%s\""
	    "\n         result: \"%s\"\n",
	    line, expect, t);
    if (code > 255) code = 255;
#elif	defined(DEBUG)
    exit ((int)t);
#endif
    exit (code);
}

#define CHECK(s, expect)	do {		\
    Check (__LINE__, PSTR(s), PSTR(expect));	\
} while (0)

int main ()
{
    /* Empty string.	*/
    CHECK ("", "");
    
    CHECK ("a", "A");
    CHECK ("z", "Z");
    CHECK ("@[", "@[");		/* '@'=='A'-1, '['=='Z'+1	*/
    CHECK ("`az{", "`AZ{");	/* '`'=='a'-1, '{'=='z'+1	*/

    CHECK ("qwertyuiopasdfghjklzxcvbnm", "QWERTYUIOPASDFGHJKLZXCVBNM");
    CHECK ("fOo", "FOO");
    
    /* non-ASCII	*/
    CHECK ("\001a\177\200\201b\377", "\001A\177\200\201B\377");

    return 0;
}

/* Test of sprintf_min(), invalid format.
   $Id$	*/

#ifndef __AVR__
# define PRINTFLN(line, fmt, ...)	\
    printf("\nLine %2d: " fmt "\n", line, ##__VA_ARGS__)
# define EXIT(code)	exit ((code) < 255 ? (code) : 255)
# define sprintf_P	sprintf
#else
# define PRINTFLN(args...)
# define EXIT	exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "progmem.h"

void Check (int line,
	    int explen,
            const char *expstr,
	    int retval, const char *retstr)
{
    int code;

    if (retval != explen)
	code = 1000 + line;
    else if (strcmp_P (retstr, expstr))
	code = line;
    else
	return;
    PRINTFLN (line, "expect: %3d, \"%s\",\n%8s output: %3d, \"%s\"\n",
	      explen, expstr, " ", retval, retstr);
#ifdef	DEBUG
    code = (int)retstr;
#endif
    EXIT (code);
}

/* 'vp' is used to avoid gcc warnings about format string.	*/
#define CHECK(explen, expstr, fmt, ...)	do {			\
    char s[260];						\
    int i;							\
    int (* volatile vp)(char *, const char *, ...) = sprintf_P;	\
    memset (s, 0, sizeof(s));					\
    i = vp (s, PSTR(fmt), ##__VA_ARGS__);			\
    Check (__LINE__, explen, PSTR(expstr), i, s);		\
} while (0)

int main ()
{
    /* Format last symbol is absent	*/
    CHECK (-1, "", "%");
    CHECK (-1, ".", ".%0");
    CHECK (-1, "12345 ", "%d %+l", 12345);

    /* 'hh' length modifier is possible, but it is ignored in avr-libc	*/
    CHECK (3, "123", "%hhd", 123);
    
    /* Length modifier and conversion mismatch: ignored	*/
    CHECK (1, "a", "%hhc", 'a');
    CHECK (1, "b", "%hc", 'b');
    CHECK (3, "foo", "%hhs", "foo");
    CHECK (3, "foo", "%hs", "foo");
    
    /* If the `space' and `+' flags both appear, the `space' flag
       is ignored.	*/
#ifdef	__AVR__
    CHECK (5, "10.11", "% +d.%+ d", 10, 11);	/* PRINTF_MIN: both ignored */
#else
    CHECK (7, "+10.+11", "% +d.%+ d", 10, 11);
#endif
    
#ifdef	__AVR__
    /* Unknown format: glibc output is "%b", return value 2.	*/
    CHECK (-1, "", "%b");

    /* Two precision dots.	*/
    // CHECK (-1, "", "%8..4d", 1);
    CHECK (1, "1", "%8..4d", 1);	/* PRINTF_MIN: not checked	*/
    
    /* Long long arg	*/
    CHECK (-1, "", "%lld", 1LL);

    /* wint_t, wchar_t: ignore	*/
    CHECK (1, "c", "%lc", 'c');
    CHECK (3, "foo", "%ls", "foo");
    
    /* The asterisk `*' in width or precision.	*/
    CHECK (-1, "", "%*d", 10, 12345);
    CHECK (-1, "", "%10.*d", 6, 12345);
    CHECK (-1, "", "%*.*d", 10, 6, 12345);
    
    /* Float numbers: are skipped	*/
    CHECK (1, "?", "%e", 0.0);
    CHECK (23, "? e ? f ? g ? E ? F ? G",
	 "%e %c %e %c %e %c %e %c %e %c %e %c",
	 1.0, 'e', 2.0, 'f', 3.0, 'g', 4.0, 'E', 5.0, 'F', 6.0, 'G');
    CHECK (14, "? 10 ? 11 ? 12",
	   "%+0e %d %-10f %d % 10.5g %d",
	   10.0, 10, 11.0, 11, 12.0, 12);
#endif

    return 0;
}

/* $Id$ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "progmem.h"

PROGMEM const struct {
    char s[10];
    int val;
} t[] = {

    /* Empty string	*/
    { "", 0 },
    { " ", 0 },
    { "\t\n\v\f\r", 0 },

    /* Common values	*/
    { "0", 0 },
    { "+0", 0 },
    { "-0", 0 },
    { "1", 1 },
    { "+1", 1 },
    { "-1", -1 },
    { "32767", 32767 },
    { "-32767", -32767 },
    { "-32768", -32768 },
    
    /* Nonzero end character	*/
    { "12\001", 12 },
    { "123\377", 123 },
    { "1234/", 1234 },		/* '0'-1 == '/'	*/
    { "12345:", 12345 },	/* '9'+1 == ':'	*/
    { "321 4", 321 },
    
    /* Empty symbols at begin	*/
    { " -4", -4 },
    { "\t+5", 5 },
    { "\t\n\v\f\r-321", -321 },		/* bug #18899	*/
    
    /* No digits	*/
    { "\001123", 0 },
    { "\377-123", 0 },
    { "\010123", 0 },		/* '\t'-1 == 010	*/
    { "\016+123", 0 },		/* '\r'+1 == 016	*/
    
    /* atoi() accepts 10-base only	*/
    { "010", 10 },
    { "0x10", 0 },
    { "0X10", 0 },
};

volatile int vlt = 1;		/* for debug conveniency	*/

void t_exit (int idx)
{
#ifdef	__AVR__
    exit (idx + 1);
#else
    printf ("result=%d against t[%d]={\"%s\", %d}\n",
	    vlt, idx, t[idx].s, t[idx].val);
    exit (1);
#endif
}

int main ()
{
    int i;
    char s[sizeof(t[0].s)];

    for (i = 0; i < (int)(sizeof(t) / sizeof(t[0])); i++) {
	strcpy_P (s, t[i].s);
	vlt = atoi (s);
	if (vlt != (int)pgm_read_word (& t[i].val))
	    t_exit (i);
    }
    return 0;
}

/* Copyright (c) 2002, Marek Michalkiewicz
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

/* $Id$ */

/*
   pgmspace.h

   Contributors:
     Created by Marek Michalkiewicz <marekm@linux.org.pl>
     Eric B. Weddington <eric@ecentral.com>
 */

/** \defgroup avr_pgmspace Program Space String Utilities
    \code
    #include <avr/io.h>
    #include <avr/pgmspace.h>
    \endcode

    The functions in this module provide interfaces for a program to access
    data stored in program space (flash memory) of the device.  In order to
    use these functions, the target device must support either the \c LPM or
    \c ELPM instructions.

    \note These function are an attempt to provide some compatibility with
    header files that come with IAR C, to make porting applications between
    different compilers easier.  This is not 100% compatibility though (GCC
    does not have full support for multiple address spaces yet).

    \note If you are working with strings which are completely based in ram,
    use the standard string functions described in \ref avr_string. */

#ifndef __PGMSPACE_H_
#define __PGMSPACE_H_ 1

#define __need_size_t
#include <inttypes.h>
#include <stddef.h>
#include <avr/io.h>


#ifndef __ATTR_CONST__
#define __ATTR_CONST__ __attribute__((__const__))
#endif

#ifndef __ATTR_PROGMEM__
#define __ATTR_PROGMEM__ __attribute__((__progmem__))
#endif

#ifndef __ATTR_PURE__
#define __ATTR_PURE__ __attribute__((__pure__))
#endif

#define PROGMEM __ATTR_PROGMEM__

#ifdef __cplusplus
extern "C" {
#endif

typedef void prog_void PROGMEM;
typedef char prog_char PROGMEM;
typedef unsigned char prog_uchar PROGMEM;
typedef int prog_int PROGMEM;
typedef long prog_long PROGMEM;
typedef long long prog_long_long PROGMEM;


/** \ingroup avr_pgmspace
    \def PSTR(s)

    Used to declare a static pointer to a string in program space. */

#define PSTR(s) ({static char __c[] PROGMEM = (s); __c;})


#define __LPM_classic__(addr)   \
({                              \
    uint16_t __addr16 = (uint16_t)(addr); \
    uint8_t __result;           \
    __asm__ __volatile__        \
    (                           \
        "lpm" "\n\t"            \
        "mov %0, r0" "\n\t"     \
        : "=r" (__result)       \
        : "z" (__addr16)        \
        : "r0"                  \
    );                          \
    __result;                   \
})



#define __LPM_enhanced__(addr)  \
({                              \
    uint16_t __addr16 = (uint16_t)(addr); \
    uint8_t __result;           \
    __asm__ __volatile__        \
    (                           \
        "lpm %0, Z+" "\n\t"     \
        : "=r" (__result)       \
        : "z" (__addr16)        \
    );                          \
    __result;                   \
})


#define __LPM_word_classic__(addr)      \
({                                      \
    uint16_t __addr16 = (uint16_t)(addr);   \
    uint16_t __result;                  \
    __asm__ __volatile__                \
    (                                   \
        "lpm"           "\n\t"          \
        "mov %A0, r0"   "\n\t"          \
        "adiw r30, 1"   "\n\t"          \
        "lpm"           "\n\t"          \
        "mov %B0, r0"   "\n\t"          \
        : "=r" (__result)               \
        : "z" (__addr16)                \
        : "r0"                          \
    );                                  \
    __result;                           \
})


#define __LPM_word_enhanced__(addr)     \
({                                      \
    uint16_t __addr16 = (uint16_t)(addr);   \
    uint16_t __result;                  \
    __asm__ __volatile__                \
    (                                   \
        "lpm %A0, Z+"   "\n\t"          \
        "lpm %B0, Z+"   "\n\t"          \
        : "=r" (__result)               \
        : "z" (__addr16)                \
    );                                  \
    __result;                           \
})



#if defined (__AVR_ENHANCED__)
#define __LPM(addr)         __LPM_enhanced__(addr)
#define __LPM_word(addr)    __LPM_word_enhanced__(addr)
#else
#define __LPM(addr)         __LPM_classic__(addr)
#define __LPM_word(addr)    __LPM_word_classic__(addr)
#endif


/** \ingroup avr_pgmspace
    \def pgm_read_byte_near(address_short)
    Read a byte from the program space with a 16-bit (near) address. 
    \note The address is a byte address.
    The address is in the program space. */

#define pgm_read_byte_near(address_short)   __LPM((unsigned short)(address_short))


/** \ingroup avr_pgmspace
    \def pgm_read_word_near(address_short)
    Read a word from the program space with a 16-bit (near) address. 
    \note The address is a byte address. 
    The address is in the program space. */

#define pgm_read_word_near(address_short)   __LPM_word((unsigned short)(address_short))





#ifdef RAMPZ  /* >64K program memory (ATmega103, ATmega128) */

/* Only for devices with more than 64K of program memory.
   RAMPZ must be defined (see iom103.h, iom128.h).  */

/* The classic functions are needed for ATmega103. */

#define __ELPM_classic__(addr)      \
({                                  \
    uint32_t __addr32 = (uint32_t)(addr); \
    uint8_t __result;               \
    __asm__ __volatile__            \
    (                               \
        "out %2, %C1" "\n\t"        \
        "mov r31, %B1" "\n\t"       \
        "mov r30, %A1" "\n\t"       \
        "elpm" "\n\t"               \
        "mov %0, r0" "\n\t"         \
        : "=r" (__result)           \
        : "r" (__addr32),           \
          "I" (_SFR_IO_ADDR(RAMPZ)) \
        : "r0", "r30", "r31"        \
    );                              \
    __result;                       \
})


#define __ELPM_enhanced__(addr)     \
({                                  \
    uint32_t __addr32 = (uint32_t)(addr); \
    uint8_t __result;               \
    __asm__ __volatile__            \
    (                               \
        "out %2, %C1" "\n\t"        \
        "movw r30, %1" "\n\t"       \
        "elpm %0, Z+" "\n\t"        \
        : "=r" (__result)           \
        : "r" (__addr32),           \
          "I" (_SFR_IO_ADDR(RAMPZ)) \
        : "r30", "r31"              \
    );                              \
    __result;                       \
})


#define __ELPM_word_classic__(addr)     \
({                                      \
    uint32_t __addr32 = (uint32_t)(addr); \
    uint16_t __result;                  \
    __asm__ __volatile__                \
    (                                   \
        "out %2, %C1"   "\n\t"          \
        "mov r31, %B1"  "\n\t"          \
        "mov r30, %A1"  "\n\t"          \
        "elpm"          "\n\t"          \
        "mov %A0, r0"   "\n\t"          \
        "in r0, %2"     "\n\t"          \
        "adiw r30, 1"   "\n\t"          \
        "adc r0, __zero_reg__" "\n\t"   \
        "out %2, r0"    "\n\t"          \
        "elpm"          "\n\t"          \
        "mov %B0, r0"   "\n\t"          \
        : "=r" (__result)               \
        : "r" (__addr32),               \
          "I" (_SFR_IO_ADDR(RAMPZ))     \
        : "r0", "r30", "r31"            \
    );                                  \
    __result;                           \
})



#define __ELPM_word_enhanced__(addr)    \
({                                      \
    uint32_t __addr32 = (uint32_t)(addr); \
    uint16_t __result;                  \
    __asm__ __volatile__                \
    (                                   \
        "out %2, %C1"   "\n\t"          \
        "movw r30, %1"  "\n\t"          \
        "elpm %A0, Z+"  "\n\t"          \
        "elpm %B0, Z+"  "\n\t"          \
        : "=r" (__result)               \
        : "r" (__addr32),               \
          "I" (_SFR_IO_ADDR(RAMPZ))     \
        : "r30", "r31"                  \
    );                                  \
    __result;                           \
})


#if defined (__AVR_ENHANCED__)
#define __ELPM(addr)        __ELPM_enhanced__(addr)
#define __ELPM_word(addr)   __ELPM_word_enhanced__(addr)
#else
#define __ELPM(addr)        __ELPM_classic__(addr)
#define __ELPM_word(addr)   __ELPM_word_classic__(addr)
#endif


#endif




/** \ingroup avr_pgmspace
    \def pgm_read_byte_far(address_long)
    Read a byte from the program space with a 32-bit (far) address. 
    \note The address is a byte address. 
    The address is in the program space. */

/** \ingroup avr_pgmspace
    \def pgm_read_word_far(address_long)
    Read a word from the program space with a 32-bit (far) address. 
    \note The address is a byte address.
    The address is in the program space. */

/** \ingroup avr_pgmspace

    \note If possible, put your constant tables in the lower 64K and use 
    pgm_read_byte_near() or pgm_read_word_near() since it is more 
    efficient that way, and you can still use the upper 64K for 
    executable code. */

#ifdef RAMPZ

#define pgm_read_byte_far(address_long)     __ELPM((unsigned long)(address_long))
#define pgm_read_word_far(address_long)     __ELPM_word((unsigned long)(address_long))

#else

#define pgm_read_byte_far(address_long)     pgm_read_byte_near(address_long)
#define pgm_read_word_far(address_long)     pgm_read_word_near(address_long)

#endif



/** \ingroup avr_pgmspace
    \def pgm_read_byte(address_short)
    Read a byte from the program space with a 16-bit (near) address. 
    \note The address is a byte address. 
    The address is in the program space. */

#define pgm_read_byte(address_short)    pgm_read_byte_near(address_short)

/** \ingroup avr_pgmspace
    \def pgm_read_word(address_short)
    Read a word from the program space with a 16-bit (near) address. 
    \note The address is a byte address. 
    The address is in the program space. */

#define pgm_read_word(address_short)    pgm_read_word_near(address_short)



/** \ingroup avr_pgmspace
    \def PGM_P

    Used to declare a variable that is a pointer to a string in program
    space. */

#ifndef PGM_P
#define PGM_P const prog_char *
#endif


/** \ingroup avr_pgmspace
    \def PGM_VOID_P

    Used to declare a generic pointer to an object in program space. */

#ifndef PGM_VOID_P
#define PGM_VOID_P const prog_void *
#endif


extern void *memcpy_P(void *, PGM_VOID_P, size_t);
extern char *strcat_P(char *, PGM_P);
extern int strcmp_P(const char *, PGM_P) __ATTR_PURE__;
extern char *strcpy_P(char *, PGM_P);
extern int strcasecmp_P(const char *, PGM_P) __ATTR_PURE__;
extern size_t strlen_P(PGM_P) __ATTR_CONST__; /* program memory can't change */
extern int strncmp_P(const char *, PGM_P, size_t) __ATTR_PURE__;
extern int strncasecmp_P(const char *, PGM_P, size_t) __ATTR_PURE__;
extern char *strncpy_P(char *, PGM_P, size_t);

#if 0  /* not implemented yet */
extern char *strncat_P(char *, PGM_P, size_t);
extern int printf_P(PGM_P, ...);
extern int puts_P(PGM_P);
extern int scanf_P(PGM_P, ...);
extern int sprintf_P(char *, PGM_P, ...);
extern int sscanf_P(const char *, PGM_P, ...);
extern PGM_P strerror_P(int);
#endif

#ifdef __cplusplus
}
#endif


/** \name Backwards compatibility macros */


/** \ingroup avr_pgmspace
    \def PRG_RDB
    \deprecated
    Use pgm_read_byte() in new programs. */

#define PRG_RDB(addr)       pgm_read_byte(addr)




#endif /* __PGMSPACE_H_ */


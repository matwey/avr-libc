/* Copyright (c) 2002, Marek Michalkiewicz <marekm@amelek.gda.pl>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

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
   POSSIBILITY OF SUCH DAMAGE.  */

/* avr/sfr_defs.h - macros for accessing AVR special function registers */

#ifndef _AVR_SFR_DEFS_H_
#define _AVR_SFR_DEFS_H_ 1

/*
   This file is included by all of the <ioXXXX.h> files, which use macros
   defined here to make the special function register definitions look like
   C variables or simple constants, depending on the _SFR_ASM_COMPAT define.
   Some examples from <iom128.h> to show how to define such macros:

#define PORTA _SFR_IO8(0x1b)
#define TCNT1 _SFR_IO16(0x2c)
#define PORTF _SFR_MEM8(0x61)
#define TCNT3 _SFR_MEM16(0x88)

   If _SFR_ASM_COMPAT is not defined, C programs can use names like PORTA
   directly in C expressions (also on the left side of assignment operators)
   and GCC will do the right thing (use short I/O instructions if possible).
   The __SFR_OFFSET definition is not used in any way in this case.

   Define _SFR_ASM_COMPAT as 1 to make these names work as simple constants
   (addresses of the I/O registers).  This is necessary when included in
   preprocessed assembler (*.S) source files, so it is done automatically
   if __ASSEMBLER__ is defined.  By default, all addresses are defined as
   if they were memory addresses (used in lds/sts instructions).  To use
   these addresses in in/out instructions, you must subtract 0x20 from them.

   For more backwards compatibility, insert the following at the start of
   your old assembler source file:

#define __SFR_OFFSET 0

   This automatically subtracts 0x20 from I/O space addresses, but it's
   a hack, so it is recommended to change your source: wrap such addresses
   in macros defined here, as shown below.  After this is done, the
   __SFR_OFFSET definition is no longer necessary and can be removed.

   Real example - this code could be used in a boot loader that is portable
   between devices with SPMCR at different addresses.

   <iom163.h>: #define SPMCR _SFR_IO8(0x37)
   <iom128.h>: #define SPMCR _SFR_MEM8(0x68)

#if _SFR_IO_REG_P(SPMCR)
	out	_SFR_IO_ADDR(SPMCR), r24
#else
	sts	_SFR_MEM_ADDR(SPMCR), r24
#endif

   You can use the in/out/cbi/sbi/sbic/sbis instructions, without the
   _SFR_IO_REG_P test, if you know that the register is in the I/O space
   (as with SREG, for example).  If it isn't, the assembler will complain
   (I/O address out of range 0...0x3f), so this should be fairly safe.

   If you do not define __SFR_OFFSET (so it will be 0x20 by default), all
   special register addresses are defined as memory addresses (so SREG is
   0x5f), and (if code size and speed are not important, and you don't
   like the ugly #if above) you can always use lds/sts to access them.
   But, this will not work if __SFR_OFFSET != 0x20, so use a different
   macro (defined only if __SFR_OFFSET == 0x20) for safety:

	sts	_SFR_ADDR(SPMCR), r24

   In C programs, all 3 combinations of _SFR_ASM_COMPAT and __SFR_OFFSET
   are supported - the _SFR_ADDR(SPMCR) macro can be used to get the
   address of the SPMCR register (0x57 or 0x68 depending on device).

   The old inp/outp macros are still supported, but not recommended
   to use in new code.  The order of outp() arguments is confusing.
 */

#ifdef __ASSEMBLER__
#define _SFR_ASM_COMPAT 1
#endif

#ifndef __ASSEMBLER__
/* These only work in C programs.  */
#define _MMIO_BYTE(mem_addr) (*(volatile unsigned char *)(mem_addr))
#define _MMIO_WORD(mem_addr) (*(volatile unsigned int *)(mem_addr))
#endif

#if _SFR_ASM_COMPAT

#ifndef __SFR_OFFSET
/* Define as 0 before including this file for compatibility with old asm
   sources that don't subtract __SFR_OFFSET from symbolic I/O addresses.  */
#define __SFR_OFFSET 0x20
#endif

#if (__SFR_OFFSET != 0) && (__SFR_OFFSET != 0x20)
#error "__SFR_OFFSET must be 0 or 0x20"
#endif

#define _SFR_MEM8(mem_addr) (mem_addr)
#define _SFR_MEM16(mem_addr) (mem_addr)
#define _SFR_IO8(io_addr) ((io_addr) + __SFR_OFFSET)
#define _SFR_IO16(io_addr) ((io_addr) + __SFR_OFFSET)

#define _SFR_IO_ADDR(sfr) ((sfr) - __SFR_OFFSET)
#define _SFR_MEM_ADDR(sfr) (sfr)
#define _SFR_IO_REG_P(sfr) ((sfr) < 0x40 + __SFR_OFFSET)

#if (__SFR_OFFSET == 0x20)
/* No need to use ?: operator, so works in assembler too.  */
#define _SFR_ADDR(sfr) _SFR_MEM_ADDR(sfr)
#elif !defined(__ASSEMBLER__)
#define _SFR_ADDR(sfr) (_SFR_IO_REG_P(sfr) ? (_SFR_IO_ADDR(sfr) + 0x20) : _SFR_MEM_ADDR(sfr))
#endif

#else  /* !_SFR_ASM_COMPAT */

#define _SFR_MEM8(mem_addr) _MMIO_BYTE(mem_addr)
#define _SFR_MEM16(mem_addr) _MMIO_WORD(mem_addr)
#define _SFR_IO8(io_addr) _MMIO_BYTE((io_addr) + 0x20)
#define _SFR_IO16(io_addr) _MMIO_WORD((io_addr) + 0x20)

#define _SFR_MEM_ADDR(sfr) ((unsigned int) &(sfr))
#define _SFR_IO_ADDR(sfr) (_SFR_MEM_ADDR(sfr) - 0x20)
#define _SFR_IO_REG_P(sfr) (_SFR_MEM_ADDR(sfr) < 0x60)

#define _SFR_ADDR(sfr) _SFR_MEM_ADDR(sfr)

#endif /* !_SFR_ASM_COMPAT */

/** \name Bit manipulation */

/*@{*/
/** \def _BV
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Converts a bit number into a byte value. */
    
#define _BV(bit) (1 << (bit))

/*@}*/

#ifndef __ASSEMBLER__

#ifndef _VECTOR
#define _VECTOR(N) __vector_ ## N
#endif

#define _SFR_BYTE(sfr) _MMIO_BYTE(_SFR_ADDR(sfr))
#define _SFR_WORD(sfr) _MMIO_WORD(_SFR_ADDR(sfr))

/* The outb/outw macros now have the correct order of arguments.  */

/** \name IO operations */

/*@{*/

/** \def inb
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Read a byte from IO register \c sfr. */

#define inb(sfr) _SFR_BYTE(sfr)

/** \def inw
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Read a 16-bit word from IO register pair \c sfr. */

#define inw(sfr) _SFR_WORD(sfr)

/** \def outb
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Write \c val to IO register \c sfr. */

#define outb(sfr, val) (_SFR_BYTE(sfr) = (val))

/** \def outw
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Write the 16-bit value \c val to IO register pair \c sfr.  Care
    will be taken to write the lower register first.  When used to
    update 16-bit registers where the timing is critical and the
    operation can be interrupted, the programmer is responsible to
    disable interrupts before accessing the register pair. */

#define outw(sfr, val) (_SFR_WORD(sfr) = (val))

/*@}*/

/** \name IO register bit manipulation */

/*@{*/

/** \def cbi
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Clear bit \c bit in IO register \c sfr. */

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

/** \def sbi
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Set bit \c bit in IO register \c sfr. */

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))


/** \def bit_is_set
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Test whether bit \c bit in IO register \c sfr is set. */

#define bit_is_set(sfr, bit) (inb(sfr) & _BV(bit))

/** \def bit_is_clear
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Test whether bit \c bit in IO register \c sfr is clear. */

#define bit_is_clear(sfr, bit) (~inb(sfr) & _BV(bit))

/** \def loop_until_bit_is_set
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Wait until bit \c bit in IO register \c sfr is set. */

#define loop_until_bit_is_set(sfr, bit) do { } while (bit_is_clear(sfr, bit))

/** \def loop_until_bit_is_clear
    \ingroup avr_sfr

    \code #include <avr/io.h>\endcode

    Wait until bit \c bit in IO register \c sfr is clear. */

#define loop_until_bit_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit))

/*@}*/

#endif /* !__ASSEMBLER__ */

/* Backwards compatibility, do not use in new programs.  */
#define outp(val, sfr) outb(sfr, val)
#define inp(sfr) inb(sfr)
#define BV(bit) _BV(bit)

#endif  /* _SFR_DEFS_H_ */

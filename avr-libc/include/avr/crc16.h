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

#ifndef _AVR_CRC16_H_
#define _AVR_CRC16_H_

/** \defgroup avr_crc CRC Computations
    \code#include <avr/crc16.h>\endcode

    This header file provides a optimized inline functions for calculating 16
    bit cyclic redundancy checks (CRC) using common polynomials.

    \par References:

    \par

    See the Dallas Semiconductor app note 27 for 8051 assembler example and
    general CRC optimization suggestions. The table on the last page of the
    app note is the key to understanding these implementations.

    \par

    Jack Crenshaw's "Impementing CRCs" article in the January 1992 isue of \e
    Embedded \e Systems \e Programming. This may be difficult to find, but it
    explains CRC's in very clear and concise terms. Well worth the effort to
    obtain a copy. */

/** \ingroup avr_crc
    Optimized CRC-16 calcutation.

    Polynomial: x^16 + x^15 + x^2 + 1 (0xa001)
    Initial value: 0xffff

    This CRC is normally used in disk-drive controllers. */

static inline unsigned int
_crc16_update(unsigned int __crc, unsigned char __data)
{
	unsigned char __tmp;
	unsigned int __ret;

	__asm__ (
		"eor %A0,%2" "\n\t"
		"mov %1,%A0" "\n\t"
		"swap %1" "\n\t"
		"eor %1,%A0" "\n\t"
		"mov __tmp_reg__,%1" "\n\t"
		"lsr %1" "\n\t"
		"lsr %1" "\n\t"
		"eor %1,__tmp_reg__" "\n\t"
		"mov __tmp_reg__,%1" "\n\t"
		"lsr %1" "\n\t"
		"eor %1,__tmp_reg__" "\n\t"
		"andi %1,0x07" "\n\t"
		"mov __tmp_reg__,%A0" "\n\t"
		"mov %A0,%B0" "\n\t"
		"lsr %1" "\n\t"
		"ror __tmp_reg__" "\n\t"
		"ror %1" "\n\t"
		"mov %B0,__tmp_reg__" "\n\t"
		"eor %A0,%1" "\n\t"
		"lsr __tmp_reg__" "\n\t"
		"ror %1" "\n\t"
		"eor %B0,__tmp_reg__" "\n\t"
		"eor %A0,%1"
		: "=r" (__ret), "=d" (__tmp)
		: "r" (__data), "0" (__crc)
		: "r0"
	);
	return __ret;
}

/** \ingroup avr_crc
    Optimized CRC-XMODEM calculation.

    Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
    Initial value: 0x0

    This is the CRC used by the Xmodem-CRC protocol.

    The following is the equivalent functionality written in C.

    \code
    uint16_t
    crc_xmodem_update (uint16_t crc, uint8_t data)
    {
        int i;

        crc = crc ^ ((uint16_t)data << 8);
        for (i=0; i<8; i++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }

        return crc;
    }
    \endcode */

static inline unsigned int
_crc_xmodem_update(unsigned int __crc, unsigned char __data)
{
    unsigned int __ret;         /* %B0:%A0 (alias for __crc) */
    unsigned char __tmp1;       /* %1 */
    unsigned char __tmp2;       /* %2 */
                                /* %3  __data */

    __asm__ (
        "eor    %B0,%3"          "\n\t" /* crc.hi ^ data */
        "mov    __tmp_reg__,%B0" "\n\t"
        "swap   __tmp_reg__"     "\n\t" /* swap(crc.hi ^ data) */

        /* Calculate the ret.lo of the CRC. */
        "mov    %1,__tmp_reg__"  "\n\t"
        "andi   %1,0x0f"         "\n\t"
        "eor    %1,%B0"          "\n\t"
        "mov    %2,%B0"          "\n\t"
        "eor    %2,__tmp_reg__"  "\n\t"
        "lsl    %2"              "\n\t"
        "andi   %2,0xe0"         "\n\t"
        "eor    %1,%2"           "\n\t" /* __tmp1 is now ret.lo. */
        
        /* Calculate the ret.hi of the CRC. */
        "mov    %2,__tmp_reg__"  "\n\t"
        "eor    %2,%B0"          "\n\t"
        "andi   %2,0xf0"         "\n\t"
        "lsr    %2"              "\n\t"
        "mov    __tmp_reg__,%B0" "\n\t"
        "lsl    __tmp_reg__"     "\n\t"
        "rol    %2"              "\n\t"
        "lsr    %B0"             "\n\t"
        "lsr    %B0"             "\n\t"
        "lsr    %B0"             "\n\t"
        "andi   %B0,0x1f"        "\n\t"
        "eor    %B0,%2"          "\n\t"
        "eor    %B0,%A0"         "\n\t" /* ret.hi is now ready. */
        "mov    %A0,%1"          "\n\t" /* ret.lo is now ready. */
        : "=d" (__ret), "=d" (__tmp1), "=d" (__tmp2)
        : "r" (__data), "0" (__crc)
        : "r0"
    );
    return __ret;
}

#endif /* _AVR_CRC16_H_ */

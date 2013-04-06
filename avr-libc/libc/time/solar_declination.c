/*
 * (C)2012 Michael Duane Rice All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions
 * and the following disclaimer in the documentation and/or other materials
 * provided with the distribution. Neither the name of the copyright holders
 * nor the names of contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* $Id$ */

/*
    Basic Solar Declination calculation.
*/

#include <time.h>
#include <math.h>

#define TROP_YEAR 31556925
#define ANOM_YEAR 31558433
#define INCLINATION 0.409105176667471 /* Earths axial tilt */
#define PERIHELION 31218166 /* perihelion of 1999 */
#define SOLSTICE 836160 /* winter solstice of 1999 */
#define TWO_PI 6.283185307179586
#define DELTA_V 0.0167

double
solar_declination(time_t * timer)
{

    uint32_t   fT, oV;
    double          dV, dT;

    /* fractional anomalistic year */
    oV = *timer % ANOM_YEAR;
    oV += PERIHELION;

    /* convert to angle */
    dV = oV;
    dV /= ANOM_YEAR;
    dV *= TWO_PI;

    /* orbital velocity correction */
    dV = sin(dV);
    dV *= DELTA_V;

     /* fractional tropical year */
    fT = *timer % TROP_YEAR;
    fT += SOLSTICE;
    dT = fT;
    dT /= TROP_YEAR;
    dT *= TWO_PI;

    dT = cos(dT+dV) * INCLINATION;

    return -dT;
}

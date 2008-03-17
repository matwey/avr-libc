#! /bin/sh
#
# Copyright (c) 2004,  Theodore A. Roth
# Copyright (c) 2005,2006,2007,2008  Anatoly Sokolov
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in
#   the documentation and/or other materials provided with the
#   distribution.
# * Neither the name of the copyright holders nor the names of
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# $Id$
#

# This is a script to automate the generation of the avr/lib/ tree. Since
# there is a build directory for each device and each of those directories is
# virtually the same, it is easier to maintain a single file instead of an
# ever growing number of small Makefile.am fragments.

# Make sure that we are the top-level of the source tree. We will look for the
# the AUTHORS file in the current dir and the parent. After that, we complain
# and fatal error out.

# Define the special flags for special sub-targets.

CFLAGS_SPACE="-mcall-prologues -Os"
CFLAGS_SPEED="-Os"

ASFLAGS_SPEED="-DOPTIMIZE_SPEED"

AVR12_DEV_INFO="\
at90s1200:crts1200.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny11:crttn11.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny12:crttn12.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny13:crttn13.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny15:crttn15.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny2313:crttn2313.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny28:crttn28.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s2313:crts2313.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s2323:crts2323.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s2333:crts2333.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s2343:crts2343.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s4433:crts4433.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s4414:crts4414.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s4434:crts4434.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s8515:crts8515.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90s8535:crts8535.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90c8534:crtc8534.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny22:crttn22.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny26:crttn26.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at86rf401:crt86401.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny25:crttn25.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny45:crttn45.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny85:crttn85.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny24:crttn24.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny44:crttn44.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny84:crttn84.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny261:crttn261.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny461:crttn461.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny861:crttn861.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR25_DEV_INFO="\
at86rf401:crt86401.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny13:crttn13.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny2313:crttn2313.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny24:crttn24.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny25:crttn25.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny261:crttn261.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny44:crttn44.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny45:crttn45.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny461:crttn461.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny84:crttn84.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny85:crttn85.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny861:crttn861.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny43u:crttn43u.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny48:crttn48.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
attiny88:crttn88.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR3_DEV_INFO="\
atmega103:crtm103.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at43usb320:crt43320.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at43usb355:crt43355.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at76c711:crt76711.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb82:crtusb82.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb162:crtusb162.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR31_DEV_INFO="\
atmega103:crtm103.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR35_DEV_INFO="\
at90usb82:crtusb82.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb162:crtusb162.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR4_DEV_INFO="\
atmega48:crtm48.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega48p:crtm48p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega8:crtm8.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega88:crtm88.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega88p:crtm88p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega8515:crtm8515.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega8535:crtm8535.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega8hva:crtm8hva.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90pwm1:crt90pwm1.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90pwm2:crt90pwm2.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90pwm2b:crt90pwm2b.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90pwm3:crt90pwm3.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90pwm3b:crt90pwm3b.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR5_DEV_INFO="\
atmega16:crtm16.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega161:crtm161.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega162:crtm162.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega163:crtm163.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega164p:crtm164p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega165:crtm165.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega165p:crtm165p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega168:crtm168.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega168p:crtm168p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega169:crtm169.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega169p:crtm169p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega16hva:crtm16hva.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega32:crtm32.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega323:crtm323.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega324p:crtm324p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega325:crtm325.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega325p:crtm325p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega3250:crtm3250.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega3250p:crtm3250p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega328p:crtm328p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega329:crtm329.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega329p:crtm329p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega3290:crtm3290.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega3290p:crtm3290p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega32hvb:crtm32hvb.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega406:crtm406.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega64:crtm64.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega640:crtm640.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega644:crtm644.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega644p:crtm644p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega645:crtm645.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega6450:crtm6450.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega649:crtm649.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega6490:crtm6490.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega128:crtm128.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega1280:crtm1280.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega1281:crtm1281.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega1284p:crtm1284p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90can32:crtcan32.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90can64:crtcan64.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90can128:crtcan128.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90pwm216:crt90pwm216.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90pwm316:crt90pwm316.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega32c1:crtm32c1.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega32m1:crtm32m1.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb646:crtusb646.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb647:crtusb647.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb1286:crtusb1286.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb1287:crtusb1287.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at94k:crtat94k.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR51_DEV_INFO="\
atmega128:crtm128.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega1280:crtm1280.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega1281:crtm1281.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega1284p:crtm1284p.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90can128:crtcan128.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb1286:crtusb1286.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
at90usb1287:crtusb1287.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

AVR6_DEV_INFO="\
atmega2560:crtm2560.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
atmega2561:crtm2561.o:${DEV_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

LIB_DEFS="-D__COMPILING_AVR_LIBC__"

AVR_ARH_INFO="\
avr2:AVR12_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr25:AVR25_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr3:AVR3_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr31:AVR31_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr35:AVR35_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr4:AVR4_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr5:AVR5_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr51:AVR51_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS};\
avr6:AVR6_DEV_INFO:${LIB_DEFS}:${CFLAGS_SPACE}:${DEV_ASFLAGS}\
"

echo "Generating source directories:"

top_dir="UNKNOWN"

if test -e AUTHORS
then
	top_dir="$PWD"
else
	cd ..
	if test -e AUTHORS
	then
		top_dir="$PWD"
	fi
fi

if test $top_dir = "UNKNOWN"
then
	echo "Can't determine the top level source dir. Aborting."
	exit 1
fi

test -d avr || mkdir avr
test -d avr/lib || mkdir avr/lib

cd avr/lib || exit 1

IFS=';'
ARH_SUBDIRS=""

for ath_lib in $AVR_ARH_INFO
do
	arh=$(echo $ath_lib | cut -d ':' -f 1)
	dev_info=$(echo $ath_lib | cut -d ':' -f 2)
	lib_defs=$(echo $ath_lib | cut -d ':' -f 3)
	lib_cflags=$(echo $ath_lib | cut -d ':' -f 4)
	lib_asflags=$(echo $ath_lib | cut -d ':' -f 5)

	install_dir=$arh
	if [ $arh = avr2 ]
	then
		install_dir=""
	fi

	echo "  avr/lib/$arh/"

	test -d $arh || mkdir $arh
	cd $arh || exit 1

	DEV_SUBDIRS=""

	eval DEV_INFO=\"\$\{$dev_info\}\"

	for dev_crt in $DEV_INFO
	do
		dev=$(echo $dev_crt | cut -d ':' -f 1)
		crt=$(echo $dev_crt | cut -d ':' -f 2)
		crt_defs=$(echo $dev_crt | cut -d ':' -f 3)
		crt_cflags=$(echo $dev_crt | cut -d ':' -f 4)
		crt_asflags=$(echo $dev_crt | cut -d ':' -f 5)

		echo "  avr/lib/$arh/$dev"

		test -d $dev || mkdir $dev

		cat $top_dir/devtools/Device.am > $dev/Makefile.am

		sed -e "s/<<dev>>/$dev/g" \
		    -e "s/<<crt>>/$crt/g" \
		    -e "s/<<crt_defs>>/$crt_defs/g" \
		    -e "s/<<crt_cflags>>/$crt_cflags/g" \
		    -e "s/<<crt_asflags>>/$crt_asflags/g"  \
		    -e "s/<<install_dir>>/$install_dir/g" $dev/Makefile.am \
		    > $dev/tempfile && mv -f $dev/tempfile $dev/Makefile.am

		DEV_SUBDIRS="$DEV_SUBDIRS $dev"
	done

	cat $top_dir/devtools/Architecture.am > Makefile.am

	sed -e "s/<<dev_subdirs>>/$DEV_SUBDIRS/g" \
	    -e "s/<<arh>>/$arh/g" \
	    -e "s/<<lib_defs>>/$lib_defs/g" \
	    -e "s/<<lib_cflags>>/$lib_cflags/g" \
	    -e "s/<<lib_asflags>>/$lib_asflags/g" \
	    -e "s/<<install_dir>>/$install_dir/g" Makefile.am \
	    > tempfile && mv -f tempfile Makefile.am

	ARH_SUBDIRS="$ARH_SUBDIRS $arh"

	cd ..
done

cat $top_dir/devtools/Lib.am > Makefile.am

sed -e "s/<<arh_subdirs>>/$ARH_SUBDIRS/g" Makefile.am \
    > tempfile && mv -f tempfile Makefile.am

cd ..

cat $top_dir/devtools/Avr.am > Makefile.am

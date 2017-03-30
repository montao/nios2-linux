#!/bin/bash
# Take a sigma dsp xml file and output a firmware blob
#
# Written by Mike Frysinger <vapier@gentoo.org>
# Copyright (C) 2009-2010 Analog Devices Inc.
# Licensed under the ADI BSD license:
#  http://docs.blackfin.uclinux.org/doku.php?id=adi_bsd

set -e

warn() { printf "%b\n" "$*" 1>&2; }
err() { warn "error: $@"; exit 1; }

if ! type -P xml >&/dev/null ; then
	err "You need the 'xml' utility from the xmlstarlet package installed:\n\thttp://xmlstar.sourceforge.net/"
fi
if ! type -P php >& /dev/null ; then
	err "You need the 'php' binary installed from the command line version of php"
fi

if [[ $# -eq 0 ]] ; then
	cat <<-EOF
	Usage: $0 <sigma dsp xml> > <fimware blob>

	Note that the firmware blob is written to stdout.

	Example: $0 source.xml > source.bin
	         ./read_firmware source.bin
	EOF
	exit 0
fi

if [[ ! -e $1 ]] ; then
	err "Input file '$1' does not exist!"
fi

unset end

export \
	INSTR_writexbytes=0 \
	INSTR_writesingle=1 \
	INSTR_writesafeload=2 \
	INSTR_delay=3 \
	INSTR_pllwait=4 \
	INSTR_noop=5 \
	INSTR_end=6

awk 'BEGIN {
	printf "ADISIGM%c", 1;
	printf "%c%c%c%c", 0, 0, 0, 0;
}'

xml sel -t -c ROM/page/action "$1" | \
	perl -p -e 's:\n::g' | \
	sed \
		-e 's:/>:\n:g' \
		-e 's:</action>:\n:g' \
		-e 's:<action ::g' \
		-e 's:ParamName="[^"]*"::g' \
		-e 's:CellName="[^"]*"::g' | \
	sed -e 's: *>\(.*\): data="\1":' | \
	tr '[:upper:]' '[:lower:]' | \
while read line ; do
	if [ "${end}" = "true" ] ; then
		err "too much input; page is supposed to be over"
	fi

	unset instr len addr data
	eval $line
	export instr len addr data
	data=$(set -- ${data}; echo ${@/#/0x})
	rlen=${len:-0}

	case $instr in
		writexbytes);;
		writesingle) len=3;;
		writesafeload);;
		delay) len=$(printf '%i' $data); unset data; rlen=0;;
		pllwait|noop) warn "ignoring unsupported instruction: $instr"; continue;;
		end) end=true; continue;;
	esac

	instr="INSTR_${instr}"
	warn "LINE: $line {inst:${!instr} rlen:${rlen}}"

#struct sigma_action {
#   uint8_t instr;
#   uint8_t len_hi;
#   uint16_t len;
#   uint16_t addr;
#   unsigned char payload[];
#};

#	awk 'BEGIN{printf "%i", '${!instr}';}'
	php -B "
		printf('%c', ${!instr});         /* instr */
		printf('%c', 0);                 /* len_hi */
		printf('%c', $((len % 256)));    /* len (msb) */
		printf('%c', $((len / 256)));    /* len (lsb) */
		printf('%c', $((addr % 256)));   /* addr (msb) */
		printf('%c', $((addr / 256)));   /* addr (lsb) */
		\$a = explode(' ', '${data}');
		if (!(count(\$a) == 1 && \$a[0] == '')) {
			foreach (\$a as \$d) { printf('%c', 0+\$d); }
			if (count(\$a) % 2 == 1) printf('%c', 0);
		}
	" </dev/null || exit
done

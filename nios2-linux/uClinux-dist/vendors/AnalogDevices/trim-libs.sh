#!/bin/bash

set -e

has() { [[ " ${*:2} " == *" "$1" "* ]] ; }

v=
vecho() { [ -z "$v" ] || echo "$*" ; }

if [ -z "${ROMFSDIR}" ] ; then
	if [ -d "$1" ] ; then
		ROMFSDIR=$1
	else
		echo "ERROR: no dir specified (arg or \$ROMFSDIR)"
		exit 1
	fi
fi

if ! scanelf -V > /dev/null ; then
	echo "ERROR: you do not have pax-utils installed"
	exit 1
fi

cd "${ROMFSDIR}"
libs=$(scanelf -F'%n#f' -qR bin sbin usr | sed 's:[, ]:\n:g' | sort -u)
if [ -z "$libs" ] ; then
	# all FLAT system, so leave libs alone
	exit 0
fi
cd lib

addlibs() {
	newlibs=$( (echo $libs; scanelf -F'%n#f' -qR ${libs}) | sed 's:[, ]:\n:g' | sort -u)
	newlibs=$(echo $newlibs)
	[ "$newlibs" != "$libs" ] || return 0
	libs=$newlibs
	addlibs
}
addlibs

# nptl like to dlopen() the libgcc_s.so library but not link against it,
# so make sure we do not prune it if we might possibly need it
if has "libpthread.so.*" ${libs} ; then
	if ! has "libgcc_s.so.*" ${libs} ; then
		libs="${libs} $(echo libgcc_s.so.*)"
	fi
fi

(
find . -maxdepth 1 -type l -printf '%P\n'
find . -maxdepth 1 -type f -printf '%P\n'
) | \
while read l ; do
	if has ${l} ${libs} ; then
		if [ -L "${l}" ] ; then
			vecho "delinking $l"
			cp "$l" "../.$l"
			rm "$l"
			mv "../.$l" "$l"
		else
			vecho "keeping $l"
		fi
	else
		vecho "trimming $l"
		rm "${l}"
	fi
done

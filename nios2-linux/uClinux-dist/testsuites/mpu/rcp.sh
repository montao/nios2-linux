#!/bin/sh

echo "$0:       Start rcp.sh at `date`"

CWD=`pwd`
cvs_server_addr=10.99.29.20
LTP_PATCH=ltp.patch
LTP_SUB_DIR=ltp-full-20101031


if [ -d $LTP_SUB_DIR ]
then
    rm -rf $LTP_SUB_DIR
    echo "$0:	Clean directory"
fi


echo "$0:	Checking out $LTP_SUB_DIR"
svn -q co svn://$cvs_server_addr/ltp/trunk/$LTP_SUB_DIR
if [ $? -ne 0 ]
then
    echo "$0:	Error, SVN checkout failed"
    exit 1
fi


# Go to working directory
echo "$0:	Go to working directory"
cd $LTP_SUB_DIR


# configure ltp
echo "$0:	Configure ..."
./configure --prefix=$CWD/$LTP_SUB_DIR --host=bfin-uclinux --build=i686-pc-linux-gnu LDFLAGS="-elf2flt=-s65536" > /dev/null 2>&1
if [ $? -ne 0 ]
then
    echo "$0:	Error, configure failed"
    exit 1
fi


# Patch for Makefiles
echo "$0:	Apply patch"
patch -p0 < ../$LTP_PATCH
if [ $? -ne 0 ]
then
    echo "$0:	Error, apply patch failed"
    exit 1
fi


# Build ltp testsuites
echo "$0:	Make ..."
make UCLINUX=1 > /dev/null 2>&1
if [ $? -ne 0 ]
then
    echo "$0:	Error, make failed"
    exit 1
fi


echo "$0:	Make install ..."
make UCLINUX=1 install > /dev/null 2>&1
if [ $? -ne 0 ]
then
    echo "$0:	Error, make install failed"
    exit 1
fi
echo "$0:	LTP build done"


cd $CWD

#Copy test cases to mpu local folder
echo "$0:       Copy mpu test cases to board"
rm -fr testcase
mkdir -p testcase

cp $LTP_SUB_DIR/testcases/bin/mmap1 $LTP_SUB_DIR/testcases/bin/mmap0[2-8] testcase
if [ $? != 0 ] ; then
    echo "copy mpu test cases failed"
    exit 1
fi

rcp testcase/mmap* root@10.100.4.50:/bin
if [ $? != 0 ] ; then
    echo "rcp failed"
    exit 1
fi

echo "$0:       Finish rcp.sh at `date`"
echo "rcp pass"

exit 0

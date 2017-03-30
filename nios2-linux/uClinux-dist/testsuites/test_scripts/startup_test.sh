#!/bin/bash -x

USER=/home/test/work/cruise
CHECKOUT=$USER/checkouts

PROTOCOL=svn
SVN_SERVER=10.99.29.20
GIT_PREFIX=origin

DO_SWITCH=1
INDEX=1

if [ "$INDEX" == "1" ] ; then

    UCLINUX_DIST_INDEX=trunk
    LINUX_KERNEL_INDEX=trunk

elif [ "$INDEX" == "2" ] ; then

    UCLINUX_DIST_INDEX=branches/2011R1
    LINUX_KERNEL_INDEX=2011R1

elif [ "$INDEX" == "3" ] ; then

    UCLINUX_DIST_INDEX=tags/2011R1-RC1
    LINUX_KERNEL_INDEX=2011R1-RC1

fi

while [ 1 ]
do

  test_command=`ps aux | grep run_kernel_test | grep -v grep | grep -v vi  | head -1 | awk '{print $11}'`

  if [ "$test_command" == "" ] ; then

    if [ "$DO_SWITCH" == "1" ] ; then

        cd $CHECKOUT/
        rm -rf uclinux-dist

        echo -n "Checking out kernel     " ; date
        svn checkout --ignore-externals --username anonymous $PROTOCOL://$SVN_SERVER/uclinux-dist/$UCLINUX_DIST_INDEX uclinux-dist 1>/dev/null 2>&1
      #  svn checkout --username anonymous $PROTOCOL://$SVN_SERVER/linux-kernel/$LINUX_KERNEL_INDEX uclinux-dist/linux-2.6.x 1>/dev/null 2>&1

        cd uclinux-dist

        if [ ! -d  $USER/download ] ; then
             mkdir -p $USER/download  
        fi
        ln -sf $USER/download
    
        for FILE in .svn/dir-props .svn/dir-prop-base
        do
            if [ -e $FILE ] && [ `cat $FILE | grep externals` ] ; then
            chmod 777 $FILE
            ls -l $FILE
            LINES=`cat $FILE | wc -l`
            TAIL=$((LINES - 5 ))
            tail -$TAIL $FILE > tmp
            mv tmp $FILE
            fi
        done

        git clone git://$SVN_SERVER/linux-kernel linux-2.6.x

          cd linux-2.6.x

            if [ "$INDEX" == "1" ] ; then
            
                git checkout $LINUX_KERNEL_INDEX
            
            elif [ "$INDEX" == "2" ] ; then
            
                git branch -a
                git checkout -b $LINUX_KERNEL_INDEX $GIT_PREFIX/$LINUX_KERNEL_INDEX 
            
            elif [ "$INDEX" == "3" ] ; then
            
                git tag -l 
                git checkout -b $LINUX_KERNEL_INDEX $LINUX_KERNEL_INDEX
            
            fi

        DO_SWITCH=0
    
    fi

 
    cd $USER/test_scripts/uclinux-dist/ 
    RUN="./run_kernel_test"
    (echo $RUN ; ) | sh

  fi

  sleep 8000

done

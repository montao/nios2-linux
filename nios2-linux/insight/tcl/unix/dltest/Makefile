# This Makefile is used to create several test cases for Tcl's load
# command.  It also illustrates how to take advantage of configuration
# exported by Tcl to set up Makefiles for shared libraries.
# RCS: @(#) $Id: Makefile.in,v 1.11 2002/07/16 22:44:43 mdejong Exp $

TCL_DBGX =		
CC = gcc -pipe
LIBS =			-L/home/dwesterg/data/nios2-linux/nios2-linux/insight/tcl/unix -ltclstub8.4${TCL_DBGX} -ldl  -lieee -lm
AC_FLAGS =		
SHLIB_CFLAGS =		-fPIC
SHLIB_LD =		gcc -pipe -shared
SHLIB_LD_LIBS =		${LIBS}
SHLIB_SUFFIX =		.so
SRC_DIR =		../../.././tcl/unix/dltest
TCL_VERSION=		8.4

CFLAGS_DEBUG		= -g
CFLAGS_OPTIMIZE		=  -D__NO_STRING_INLINES -D__NO_MATH_INLINES

CFLAGS = -g -O2 $(CFLAGS_OPTIMIZE)
CC_SWITCHES = $(CFLAGS) -I${SRC_DIR}/../../generic -DTCL_MEM_DEBUG \
	${SHLIB_CFLAGS} -DUSE_TCL_STUBS ${AC_FLAGS}

all: pkga${SHLIB_SUFFIX} pkgb${SHLIB_SUFFIX} pkgc${SHLIB_SUFFIX} pkgd${SHLIB_SUFFIX} pkge${SHLIB_SUFFIX}
	@touch ../dltest.marker

pkga${SHLIB_SUFFIX}: $(SRC_DIR)/pkga.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkga.c
	${SHLIB_LD} -o pkga${SHLIB_SUFFIX} pkga.o ${SHLIB_LD_LIBS}

pkgb${SHLIB_SUFFIX}: $(SRC_DIR)/pkgb.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgb.c
	${SHLIB_LD} -o pkgb${SHLIB_SUFFIX} pkgb.o ${SHLIB_LD_LIBS}

pkgc${SHLIB_SUFFIX}: $(SRC_DIR)/pkgc.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgc.c
	${SHLIB_LD} -o pkgc${SHLIB_SUFFIX} pkgc.o ${SHLIB_LD_LIBS}

pkgd${SHLIB_SUFFIX}: $(SRC_DIR)/pkgd.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkgd.c
	${SHLIB_LD} -o pkgd${SHLIB_SUFFIX} pkgd.o ${SHLIB_LD_LIBS}

pkge${SHLIB_SUFFIX}: $(SRC_DIR)/pkge.c
	$(CC) -c $(CC_SWITCHES) $(SRC_DIR)/pkge.c
	${SHLIB_LD} -o pkge${SHLIB_SUFFIX} pkge.o ${SHLIB_LD_LIBS}

clean:
	rm -f *.o *${SHLIB_SUFFIX} config.cache config.log config.status
	rm -f lib.exp ../dltest.marker

distclean: clean
	rm -f Makefile

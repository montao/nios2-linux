
EXEC = dhrystone
OBJS = dhry_1.o dhry_2.o

CFLAGS += -DNO_PROTOTYPES=1

ifndef IP
IP=10.64.204.74
endif

ifndef ITT
ITT=10000000
endif

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm $(LDLIBS)

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

$(OBJS): dhry.h

results:
	cpu=`rsh -l root $(IP) 'grep "cpu MHz" /proc/cpuinfo' | awk -F ":" '{print $$2}' | awk -F "/" '{print $$1}' | sed 's/^[ \t]*//;s/[ \t]*$$//'` ;\
	echo "testing on $$cpu MHz with $$CC" ;\
	cflags=`echo $(CFLAGS) | sed -e 's:-O[0-3s]::g' -e 's/[[:space:]][[:space:]]*/ /g'`; \
	echo "^ Flags ((standard ''CFLAGS'' include ''$${cflags}''))  ^  size (bytes)  ^  Loops  ^  Dhrystones per Second ((Based on $(CC))) ^  Dhrystone MIPS ((DMIPS is obtained when the Dhrystone score is divided by 1,757 (the number of Dhrystones per second obtained on the VAX 11/780, nominally a 1 MIPS machine))  ^  DMIPS/MHz  ^";\
	( for level in -O{s,0,1,2,3}{" "," -fomit-frame-pointer"}{" "," -static"}{""," -ffast-math"}{""," -funroll-loops"}{""," -funsafe-loop-optimizations"} ; do \
	    if [ -z "$${CONFIG_FMT_USE_FDPIC_ELF}" -a `echo $${level} | grep static | wc -l` -eq 1 ] ; then \
		continue; \
	    fi; \
	    echo -n "| ''$${level}'' " | sed 's/ [ ]*/ /g' | sed "s/ '' /''/" ; \
	    touch dhry.h;\
	    $(MAKE) -s --no-print-directory all CFLAGS="$${cflags} $${level} -w" ;\
	    if [ -n "$${CONFIG_FMT_USE_FDPIC_ELF}" ] ; then \
		$(CROSS)strip $(EXEC); \
	    fi;\
	    find ./ -name $(EXEC) -printf " |  %s  |  " ;\
	    tmp=`md5sum $(EXEC) | awk '{print $$1}'`;\
	    printf " $$tmp |  $(ITT)  |  " ; \
	    rcp $(EXEC) root@$(IP):/var/$(EXEC) ; \
	    tmp=`rsh -l root $(IP) /var/$(EXEC) $(ITT)` ;\
	    echo $$cpu $$tmp | awk '{print $$NF "  |  " $$NF/1757 "  |  " $$NF/(1757*$$1) "  |" }' ; \
	  done; \
	)


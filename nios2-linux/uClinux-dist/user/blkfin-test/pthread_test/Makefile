LDFLAGS += -pthread

EXEC = ex1 ex2 ex3 ex4 ex5 ex6 ex7 ptest

all: $(EXEC)

ex1: ex1.c
	$(CC) -o $@ ex1.c $(LDFLAGS)

ex2: ex2.c
	$(CC) -o $@ ex2.c $(LDFLAGS)

ex3: ex3.c
	$(CC) -o $@ ex3.c $(LDFLAGS)

ex4: ex4.c
	$(CC) -o $@ ex4.c $(LDFLAGS)

ex5: ex5.c
	$(CC) -o $@ ex5.c $(LDFLAGS)

ex6: ex6.c
	$(CC) -o $@ ex6.c $(LDFLAGS)

ex7: ex7.c
	$(CC) -o $@ ex7.c $(LDFLAGS)

ptest: ptest.cpp
	$(CXX) -O -o $@ ptest.cpp $(LDFLAGS)

romfs:
	$(ROMFSINST) /bin/ex1
	$(ROMFSINST) /bin/ex2
	$(ROMFSINST) /bin/ex3
	$(ROMFSINST) /bin/ex4
	$(ROMFSINST) /bin/ex5
	$(ROMFSINST) /bin/ex6
	$(ROMFSINST) /bin/ex7
	$(ROMFSINST) /bin/ptest

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

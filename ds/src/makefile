CC = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Wextra
GD = -g
GC = -DNDEBUG -o3

#GC_FLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3

ht : ht_tests.o ht.o sll.o

ht_dict : ht_tests_dict.o ht.o sll.o
	$(CC) $(CFLAGS) $(GD) ht_tests_dict.o ht.o sll.o -o ht_tests_dict.out
ht.o : ht.h sll.h 

sll.o : sll.h

ht_tests.o : ht.h

clean :
	rm ht_tests.out ht_tests.o ht.o sll.o ht_tests_release.out
all : debug release

debug : ht_tests.o ht.o sll.o
	$(CC) $(CFLAGS) $(GD) ht_tests.o ht.o sll.o -o ht_tests.out

release : ht_tests.o ht.o sll.o
	$(CC) $(CFLAGS) $(GC) ht_tests.o ht.o sll.o -o ht_tests_release.out

libht.ar : ht.o
	ar -rcs libht.ar ht.o
CC=gcc
gd= -ansi -pedantic-errors -Wall -Wextra -g
gc =-ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3
objects= fsa vsa stack sll ht ht_dictionary

all : $(objects) 


fsa: ./src/fsa.c ./test/test_fsa.c ./include/fsa.h
	$(CC) $(gd) ./src/fsa.c ./test/test_fsa.c -o ./bin/debug/fsa
	
bst: ./src/bst.c ./test/bst_test.c ./include/bst.h
	$(CC) $(gd) ./src/bst.c ./test/bst_test.c -o ./bin/debug/bst	
	
vsa: ./src/vsa.c ./test/vsa_test.c ./include/vsa.h
	$(CC) $(gd) ./src/vsa.c ./test/vsa_test.c -o ./bin/debug/vsa
	
stack: ./src/stack.c ./test/stack_tests.c ./include/stack.h
	$(CC) $(gd) ./src/stack.c ./test/stack_tests.c -o ./bin/debug/stack
	
sll: ./src/sll.c ./test/sll_tests.c ./include/sll.h
	$(CC) $(gd) ./src/sll.c ./test/sll_tests.c -o ./bin/debug/sll	

ht: ./src/ht.c ./src/sll.c ./test/ht_tests.c ./include/ht.h
	$(CC) $(gd) ./src/ht.c ./src/sll.c ./test/ht_tests.c -o ./bin/debug/ht
	
ht_dictionary: ./src/ht.c ./src/sll.c ./test/ht_tests_dict.c ./include/ht.h
	$(CC) $(gd) ./src/ht.c ./src/sll.c ./test/ht_tests_dict.c -o ./bin/debug/ht_dictionary


clean :
	rm $(objects) 


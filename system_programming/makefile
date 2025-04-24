CC=gcc
gd=-ansi -pedantic-errors -Wall -Wextra -g
gc=-ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3
vlg=valgrind --leak-check=yes --track-origins=yes
objects= ext2

all : $(objects) 


ext2: ./src/ExtParser.c ./test/ExtParser_tests.c ./include/ExtParser.h ./include/ExtParser_fs.h
	$(CC) $(gd) ./src/ExtParser.c ./test/ExtParser_tests.c -o ./bin/debug/ext2
	
run_ext2: ./bin/debug/ext2
	sudo ./bin/debug/ext2
	
auth: ./src/auth.c ./test/test_auth.c ./include/auth.h
	$(CC) $(gd) ./src/auth.c ./test/test_auth.c -lcrypt -o ./bin/debug/auth
	
run_auth: ./bin/debug/auth
	./bin/debug/auth

auth_PAM: ./test/test_PAM.c
	$(CC) $(gd) ./test/test_PAM.c -lpam -lpam_misc -o ./bin/debug/auth_pam
	
run_auth_PAM: ./bin/debug/auth_pam
	./bin/debug/auth_pam
	
vlg_auth: ./bin/debug/auth
	$(vlg) ./bin/debug/auth

clean :
	rm $(objects) 


ex2: main.o SP_Aux.o SP_Stack.o
	gcc -std=c99 -Wall -Werror -pedantic-errors  main.o SP_Aux.o SP_Stack.o -o ex2

Stack_UnitTest: SP_Stack.o SP_Stack_UnitTest.o
	gcc -std=c99 -Wall -Werror -pedantic-errors  SP_Stack.o SP_Stack_UnitTest.o -o Stack_UnitTest

main.o: main.c SP_Aux.h SP_Stack.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c main.c 

SP_Stack.o: SP_Stack.c SP_Stack.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SP_Stack.c

SP_Stack_UnitTest.o: SP_Stack.h SP_Stack_UnitTest.c
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SP_Stack_UnitTest.c

SP_Aux.o: SP_Aux.c SP_Aux.h
	gcc -std=c99 -Wall -Werror -pedantic-errors -c SP_Aux.c

clean:
	rm -f main.o SP_Aux.o SP_Stack.o SP_Stack_UnitTest.o Stack_UnitTest ex2


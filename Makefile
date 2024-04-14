scheduler: main.c scheduler.c tasks.c tests.c
	gcc -o scheduler main.c scheduler.c tasks.c tests.c -lrt

clean: 
	/bin/rm -f *.o scheduler

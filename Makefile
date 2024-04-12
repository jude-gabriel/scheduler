scheduler: main.c scheduler.c tasks.c
	gcc -o scheduler main.c scheduler.c tasks.c -lrt

clean: 
	/bin/rm -f *.o scheduler

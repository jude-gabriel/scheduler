RPI_MODE = 0

ifeq ($(RPI_MODE), 1) 
	FLAGS = -lwiringPi
else 
	FLAGS = 
endif

scheduler: main.c scheduler.c tasks.c tests.c
	gcc -o scheduler main.c scheduler.c tasks.c tests.c -lrt $(FLAGS)

clean: 
	/bin/rm -f *.o scheduler

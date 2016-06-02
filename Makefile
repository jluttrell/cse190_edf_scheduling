all: aw1 aw2 aw3 jw1 jw2 jw3

aw: aw1 aw2 aw3

jw: jw1 jw2 jw3

aw1:
	gcc -o main_section2_w1 main_section2.c assignment1.c assignment2.c governor.c scheduler.o a1/workload.o a1/deadlines.c -lwiringPi -lpthread
	cp -avr a1/* 1 

aw2:
	gcc -o main_section2_w2 main_section2.c assignment1.c assignment2.c governor.c scheduler.o a2/workload.o a2/deadlines.c -lwiringPi -lpthread
	cp -avr a2/* 2

aw3:
	gcc -o main_section2_w3 main_section2.c assignment1.c assignment2.c governor.c scheduler.o a3/workload.o a3/deadlines.c -lwiringPi -lpthread
	cp -avr a3/* 3

jw1:
	gcc -o main_section2_w1 main_section2.c assignment1.c assignment2.c governor.c scheduler.o j1/workload.o j1/deadlines.c -lwiringPi -lpthread
	cp -avr a1/* 1

jw2:
	gcc -o main_section2_w2 main_section2.c assignment1.c assignment2.c governor.c scheduler.o j2/workload.o j2/deadlines.c -lwiringPi -lpthread
	cp -avr a2/* 2

jw3:
	gcc -o main_section2_w3 main_section2.c assignment1.c assignment2.c governor.c scheduler.o j3/workload.o j3/deadlines.c -lwiringPi -lpthread
	cp -avr a3/* 3

clean:
	rm -rf main_section2_w1 main_section2_w2 main_section2_w3

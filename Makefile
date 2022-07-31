build:
	mkdir build
	g++ -c -o build/schedule_lib.o src/schedule_lib.cpp
	ar rvs build/schedule_lib.a build/schedule_lib.o

clear:
	rm -r build

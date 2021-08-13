all: main
main.o: main.cc
	g++ -g -c -std=c++17 main.cc -o main.o
inventory_mode.o: inventory_mode.cc
	g++ -g -c -std=c++17 inventory_mode.cc -o inventory_mode.o
classes.o: classes.cc
	g++ -g -c -std=c++17 classes.cc -o classes.o
draw.o: draw.cc
	g++ -g -c -std=c++17 draw.cc -o draw.o
generate.o: generate.cc
	g++ -g -c -std=c++17 generate.cc -o generate.o
main: main.o inventory_mode.o classes.o draw.o generate.o
	g++ -g -lncurses -std=c++17 main.o inventory_mode.o classes.o draw.o generate.o -o main
clean:
	rm -f main.o inventory_mode.o classes.o draw.o generate.o

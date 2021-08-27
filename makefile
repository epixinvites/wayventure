all: main
main.o: main.cc
	g++ -g -c -std=c++17 -Iinclude/ main.cc -o main.o
inventory_mode.o: inventory_mode.cc
	g++ -g -c -std=c++17 inventory_mode.cc -o inventory_mode.o
classes.o: classes.cc
	g++ -g -c -std=c++17 classes.cc -o classes.o
draw.o: draw.cc
	g++ -g -c -std=c++17 draw.cc -o draw.o
generate.o: generate.cc
	g++ -g -c -std=c++17 generate.cc -o generate.o
bar.o: bar.cc
	g++ -g -c -std=c++17 bar.cc -o bar.o
main: main.o inventory_mode.o classes.o draw.o generate.o bar.o
	g++ -g -std=c++17 main.o inventory_mode.o classes.o draw.o generate.o bar.o -lncurses -o main
clean:
	rm -f main.o inventory_mode.o classes.o draw.o generate.o bar.o

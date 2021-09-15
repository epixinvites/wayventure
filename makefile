all: project-cosmos
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
help_mode.o: help_mode.cc
	g++ -g -c -std=c++17 help_mode.cc -o help_mode.o
bar.o: bar.cc
	g++ -g -c -std=c++17 -Iinclude/ bar.cc -o bar.o
project-cosmos: main.o inventory_mode.o classes.o draw.o generate.o bar.o help_mode.o
	g++ -g -std=c++17 draw.o main.o inventory_mode.o classes.o generate.o bar.o help_mode.o -o project-cosmos `pkg-config libtcod --cflags --libs` `sdl2-config --cflags --libs`
clean:
	rm -f main.o inventory_mode.o classes.o draw.o generate.o bar.o

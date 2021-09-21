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
inventory_modifier_mode.o: inventory_modifier_mode.cc
	g++ -g -c -std=c++17 inventory_modifier_mode.cc -o inventory_modifier_mode.o
project-cosmos: main.o inventory_mode.o classes.o draw.o generate.o bar.o help_mode.o inventory_modifier_mode.o
	g++ -static-libstdc++ -static-libgcc -g -std=c++17 draw.o main.o inventory_mode.o classes.o generate.o bar.o help_mode.o inventory_modifier_mode.o -o wayventure -Llibs/ -ltcod -lSDL2 -lpthread -lz -ldl
clean:
	rm -f main.o inventory_mode.o classes.o draw.o generate.o bar.o help_mode.o inventory_modifier_mode.o wayventure

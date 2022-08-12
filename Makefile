all: main
main: main.cpp stack.cpp guard.cpp
	g++ -o ak main.cpp stack.cpp guard.cpp

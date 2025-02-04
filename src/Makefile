CXXFLAGS=-ggdb -std=c++20 -Wall -Wextra -Werror -Wfatal-errors -pedantic
# main: string.cpp
# 	g++ $(CXXFLAGS) string.cpp -o main
# main: alloc.o string.o standard_main.cpp
# 	g++ $(CXXFLAGS) standard_main.cpp -o main alloc.o string.o
# string.o: string.cpp string.hpp
# 	g++ $(CXXFLAGS) -c string.cpp -o string.o
# alloc.o: alloc.cpp alloc.hpp
# 	g++ $(CXXFLAGS) -c alloc.cpp -o alloc.o

main: alloc.o string.cpp
	g++ $(CXXFLAGS) string.cpp -o main alloc.o
alloc.o: alloc.cpp alloc.hpp
	g++ $(CXXFLAGS) -c alloc.cpp -o alloc.o

clean:
	/bin/rm main *o
CXX := g++
CXXFLAGS := -O3 -std=c++20 -Wall -Wextra -Werror
INCLUDE:= -I ~/Eigen/eigen-5.0.0
TARGET := calc.o
DATA:= magnetization.txt

all: $(TARGET)

$(TARGET): main.o
	$(CXX) $(CXXFLAGS) main.o -o $(TARGET)

main.o: src/main.cxx src/ising.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c src/main.cxx -o main.o

clean:
	rm -f *.o $(DATA)

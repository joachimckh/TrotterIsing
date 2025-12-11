CXX := g++
CXXFLAGS := -O3 -I ~/Eigen/eigen-5.0.0
TARGET := calc.o

all: $(TARGET)

$(TARGET): main.o
	$(CXX) $(CXXFLAGS) main.o -o $(TARGET)

main.o: main.cxx ising.hpp
	$(CXX) $(CXXFLAGS) -c main.cxx -o main.o

clean:
	rm -f *.o $(TARGET)

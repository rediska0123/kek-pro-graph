.PHONY: all clean

CXX=g++
CXXFLAGS = -O2 -Wall -Werror -std=c++11 -Iinclude

all: sum_test

sum_test: bin/test.o bin/Test.o bin/SumTest.o bin/sum.o bin
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/test.o bin/Test.o bin/SumTest.o bin/sum.o

bin:
	mkdir -p bin

bin/%.o: src/%.cpp include/*.h bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

bin/%.o: test/%.cpp include/*.h bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf bin sum_test

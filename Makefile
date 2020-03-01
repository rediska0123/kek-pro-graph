.PHONY: all clean

CXX=g++
CXXFLAGS = -O2 -Wall -Werror -std=c++11 -Iinclude -Iserd/serd

all: main

main: bin/main.o bin/parse_regex.o bin/automaton.o bin/regex_to_fsa.o
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/main.o bin/parse_regex.o bin/automaton.o bin/regex_to_fsa.o serd/build/src/*.3.o

tests: bin/test.o bin/Test.o bin/ParseRDFTest.o bin/parseRDF.o bin/parse_regex_test.o bin/parse_regex.o bin
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/test.o bin/Test.o bin/ParseRDFTest.o bin/parseRDF.o bin/parse_regex_test.o bin/parse_regex.o serd/build/src/*.3.o

bin:
	mkdir -p bin

bin/%.o: src/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

bin/%.o: test/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf bin main tests

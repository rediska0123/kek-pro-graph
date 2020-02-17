.PHONY: all clean

CXX=g++
CXXFLAGS = -O2 -Wall -Werror -std=c++11 -Iinclude -Iserd/serd

all: main

main: bin/main.o bin/parseRDF.o
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/main.o bin/parseRDF.o serd/build/src/*.3.o -lneo4j-client

rdf_test: bin/test.o bin/Test.o bin/ParseRDFTest.o bin/parseRDF.o bin
	$(CXX) $(CXXFLAGS) -o $@ -Iinclude bin/test.o bin/Test.o bin/ParseRDFTest.o bin/parseRDF.o serd/build/src/*.3.o -lneo4j-client

bin:
	mkdir -p bin

bin/%.o: src/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

bin/%.o: test/%.cpp bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf bin main rdf_test

[![Build Status](https://travis-ci.com/rediska0123/kek-pro-graph.svg?branch=dev)](https://travis-ci.org/rediska0123/kek-pro-graph)

# Kek Pro Graph
какой-то кек про граф

## Installing instructions (Linux OS)

From kek-pro-graph directory:
```
    git submodule update --init --recursive
```
and from kek-pro-graph/serd directory:
```
    ./waf configure
    ./waf
    sudo ./waf install
```

## Building instructions

To build the project simply run
```
    make
    make rdf_test
```

### Running tests instructions

```
    make rdf_test
    ./rdf_test
```

## Parsing RDF NTriple files:

To parse .nt file run:
```
    make
    ./main your_filepath
```
Last command will print edges found in the provided file (found in your_filepath)
in format from-vertex label, edge label, to-vertex label. Each label is a string.

To parse example file use 'test/files/test_general.nt' as *your_filepath*.

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
    make tests
    ./tests
```

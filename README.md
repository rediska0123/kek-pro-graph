# Kek Pro Graph
какой-то кек про граф

## Installing (Linux OS)

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

## Running the tests

```
    make rdf_test
    ./rdf_test
```

## Running main

Run main:
```
    make
    ./main test/files/test_single_edge.nt
```

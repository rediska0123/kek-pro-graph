# Kek Pro Graph
какой-то кек про граф

## Installing

From kek-pro-graph directory:
```
    git submodule update --init --recursive
    sudo apt-get install build-essential cmake m4 automake peg libtool autoconf
```
and from kek-pro-graph/serd directory:
```
    ./waf configure
    ./waf
    ./waf install # or sudo ./waf install
```

## Running the tests

```
    make rdf_test
    ./rdf_test
```



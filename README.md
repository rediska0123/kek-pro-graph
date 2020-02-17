# Kek Pro Graph
какой-то кек про граф

## Installing (Linux OS)
Install neo4j:

```
    wget -O - https://debian.neo4j.org/neotechnology.gpg.key | sudo apt-key add -
    echo 'deb https://debian.neo4j.org/repo stable/' | sudo tee -a /etc/apt/sources.list.d/neo4j.list
    sudo apt-get update
```
Install neo4j-client:
```
    sudo apt-get install -y neo4j-client
```
from kek-pro-graph directory:
```
    git submodule update --init --recursive
```
from kek-pro-graph/libneo4j-client directory:
```
    ./autogen.sh
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

## Running main

Start neo4j server:
```
    sudo neo4j start -env NEO4J_AUTH=neo4j/pass
```
Run main:
```
    make
    ./main
```

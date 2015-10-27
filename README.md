# gclust

An R package for community detection on (connected, undirected, simple)
networks. Makes use of the
[igraph](https://cran.r-project.org/web/packages/igraph/index.html) R package.
If the network edges contain weights they will be ignored.

## Installation

Clone or download this repo and install in R with:
```R
install.packages("/path/to/package/gclust.zip", repos = NULL, type = "source")
```
or using the devtools package:
```R
devtools::install_github("barrpet/gclust")
```

## Usage

Following the igraph package convention for community detection functions, a
heirarchal clustering (returning a `hclust` object) can be obtained with:
```R
cluster_berenhaut(graph, nodes = V(graph), short_paths = NULL)
```
`graph` can be either an igraph object or a |E|x2 matrix representing the
undirected edges of a graph. It is reccomended that the shortest path lengths be
passed to the functions or they will be computed every time. A function for
shortest path lengths is provided by the package which is more efficient than
the igraph function for unweighted, undirected graphs:
```R
shortest_path_lengths(graph, nodes = V(graph))
```

## Authors

Peter S. Barr* - <barrps12@wfu.edu>

Dr. Kenneth Berenhaut - <berenhks@wfu.edu>

Alyssa M. Kogel - <kogeam11@wfu.edu>

_* package author and maintainer_

## License

MIT

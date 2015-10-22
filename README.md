# gclust

An R package for community detection on networks. Makes use of the igraph R
package which can be found at
https://cran.r-project.org/web/packages/igraph/index.html. Currently only works
on undirected, connected, simple networks. If the graph edges contain weights
they will be ignored.

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
heirarcal clustering can be obtained with:
```R
cluster_berenhaut(graph, nodes = V(graph), short_paths = NULL)
```
If shortest path lengths are not provided, the will be calculated every time the
function is called. A function for shortest paths is provided in the package
which is more efficient than the igraph function for unweighted, undirected
graphs.

## Authors

Peter Barr - <barrps12@wfu.edu>

Dr. Kenneth Berenhaut - <berenhks@wfu.edu>

## License

MIT


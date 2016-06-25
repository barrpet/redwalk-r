#' @importFrom stats hclust as.dist
#' @importFrom igraph as_edgelist vcount V
#' @importFrom igraph is.igraph is.directed is.connected is.simple
# @importFrom igraph graph.laplacian distances degree neighbors is.weighted
NULL

#' The redwalk package
#'
#' \pkg{redwalk} is an R package for community detection that uses the
#' \pkg{\link{igraph}} package graphs for input and \code{\link{hclust}} objects
#' from the \pkg{\link{stats}} package for outputting the results of the
#' heirarchal clustering. For the main algorithm, see
#' \code{\link{cluster_redwalk}}.
#'
#' @name redwalk-package
#' @aliases redwalk-package redwalk
#' @docType package
#' @author Dr. Kenneth Berenhaut \email{berenhks@@wfu.edu}, Peter S. Barr
#'   \email{barrps12@@wfu.edu}, Alyssa M. Kogel \email{kogeam11@@wfu.edu}
#' @keywords cluster graphs package
#'
#' @section Community Detection:
#'   \code{\link{cluster_redwalk}} is the main function of the package. It
#'   takes an igraph graph and optionally a shortest paths matrix as input and
#'   returns an hclust object. Unlike most heirarchal clustering methods on
#'   graphs, the dendrograms produced by this method are extremely useful in
#'   determining community structure and the number of communities in a network.
#'   Time complexity is O(n^2 log n) and real world performance is comparable to
#'   that of \code{\link{cluster_walktrap}} with 4 steps. Notably, we do not
#'   attempt to maximize the popular "modularity" measure, although our
#'   partitions result in maximum modularity values that are on average among
#'   the highest of most commonly used algorithms for the widest variety of
#'   network structures.
#'
#' @section Dissimilarity:
#'   The heart of the algorithm involves computing the average shortest paths of
#'   a vertex's neighbors to all other vertices and then considering the
#'   dissimilarity between two vertices v_i and v_j to be the minimum of these
#'   two distances.
#'
#' @section Shortest Paths:
#'   The above functions can be called with or without the shortest paths
#'   provided. If shortest paths are not provided, they will be calculated on
#'   each function call. For faster performance, precomputing all pairs shortest
#'   paths for a network will provide much faster results, as a breadth first
#'   search for every vertex has time complexity \eqn{O(n^2 + nm)}. Although
#'   igraph provides functions for calculating these values, this package
#'   includes an equivalent function, \code{\link{shortest_path_lengths}}
#'   which is optimized for the APSP problem on undirected, unweighted graphs.
#'
#' @section Heirarchal Clustering:
#'   redwalk uses \code{\link{hclust}} objects to return information about
#'   community structure rather than igraph \code{\link{communities}} object due
#'   to the fact that the igraph communities make heavy use of modularity which
#'   the algorithm do not make use of in attempting to find community structure.
#'   The clustering algorithm simply performs a heirarcal clustering on the
#'   dissimilarity matrix described above using the average agglomeration method
#'   . The resulting clustering can be viewed as a dendrogram using the
#'   \code{\link{plot.hclust}} method. The resulting dendrograms provide great
#'   insight into the community structure of the network and generally give a
#'   reasonable cut location given a community structure exists in the network.
#'
#' @section Sample Networks:
#'   Several sample networks are included in this package, all of which have
#'   known ground truths. The actual membership for each vertex in a sample
#'   network is stored for each vertex as an igraph vertex attribute named
#'   "membership" (see \code{link{vertex_attr}} for more information). The full
#'   ground truth can be retrieved using \code{V(graph)$membership}. All of the
#'   provided sample networks are cited and contain sources.
#'
#'   \code{\link{karate}}, \code{\link{dolphins}}, \code{\link{football}},
#'   \code{\link{polblogs}}, \code{\link{polbooks}}, \code{\link{davisclub}}
#'
#' @references
#'   Need some references here, gotta be all official and what not. Probably
#'   will cite ourselves when published as well.
#'
#'   Wake Forest University, Department of Mathematics and Statistics:
#'   \url{http://college.wfu.edu/math}
#'
#'   Wake Forest University, Department of Computer Science:
#'   \url{http://college.wfu.edu/cs}
#'
#' @seealso
#'   \code{\link{cluster_redwalk}};
#'   \code{\link{shortest_path_lengths}};
NULL

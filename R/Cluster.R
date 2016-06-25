#' Berenhaut Community Detection on Networks
#'
#' Performs community detection on networks using average shortest path distance
#' of vertex neighbors as the set of dissimilarities for heirarcal clustering
#' with "average" as the agglomeration method. If shortest paths are supplied,
#' this function is ~50\% faster. It is also best to use a symmetric "matrix"
#' rather than a "dist" object if performance is critical. Time complexity is
#' O(n^2 log n) and space complexity is O(n^2). This function returns an
#' \code{\link{hclust}} object rather than an igraph \code{\link{communities}}
#' object because the algorithm does not attempt to use modularity use or
#' maximize modularity (although the resulting partitioning results in a maximum
#' modularity that is competitive with the top community detection algorithms).
#'
#' @param graph an igraph object.
#' @param nodes a subset of the nodes in the graph to cluster, defaults to all
#' nodes (full community detection)
#' @param short_paths either a \eqn{|V|x|V|} symmetric dissimilarity matrix of the
#' vertices in \code{graph} or a \code{\link{dist}} object from the
#' \pkg{\link{stats}} package. If \code{NULL} (default), the shortest paths will
#' be calculated. Note: much faster if \code{short_paths} is
#' provided and much faster if \code{diss} is of class "matrix" rather than
#' "dist".
#' @return \code{cluster_redwalk} returns a \code{\link{hclust}} object from
#' the \pkg{\link{stats}} package.
#' @export
#' @keywords cluster graphs
#'
#' @examples
#' require(stats, quietly = TRUE)
#' cb <- cluster_redwalk(karate)
#' ## cb is an hclust object
#' plot(cb)
#' ## membership for different community sizes (k)
#' cutree(cb, k = 2)
#' cutree(cb, k = 3)
#'
#' ## using precomputed shortest paths
#' sp <- shortest_path_lengths(dolphins)
#' cbd <- cluster_redwalk(dolphins, short_paths = sp)
#' plot(cbd)
#' cutree(cbd, k = 2)
#' cutree(cbd, k = 4)
#'
cluster_redwalk <- function(graph, nodes = V(graph), short_paths = NULL)
{
  # check graph
  check_graph(graph)

  # check nodes
  nodes <- check_nodes(vcount(graph), nodes)

  # compute the dissimilarity
  D <- dissimilarity(graph, nodes, short_paths)

  # return the hclust object, using fastcluster if available
  if(requireNamespace("fastcluster", quietly = TRUE)) {
    return(fastcluster::hclust(D, method = "average"))
  } else {
    return(stats::hclust(D, method = "average"))
  }
}

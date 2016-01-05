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
#' @param diss either a \eqn{|V|x|V|} symmetric dissimilarity matrix of the
#' vertices in \code{graph} or a \code{\link{dist}} object from the
#' \pkg{\link{stats}} package. If \code{NULL} (default), the shortest paths will
#' be calculated. Note: much faster if \code{diss} is
#' provided and much faster if \code{diss} is of class "matrix" rather than
#' "dist".
#' @return \code{cluster_redwalk} returns a \code{\link{hclust}} object from
#' the \pkg{\link{stats}} package.
#' @export
#' @keywords cluster graphs
#'
#' @examples
#' require(stats, quietly = TRUE)
#' require(Rcpp, quietly = TRUE)
#' cb <- cluster_redwalk(karate)
#' ## cb is an hclust object
#' plot(cb)
#' ## membership for different community sizes (k)
#' cutree(cb, k = 2)
#' cutree(cb, k = 3)
#'
#' ## using precomputed shortest paths
#' sp <- shortest_path_lengths(dolphins)
#' cbd <- cluster_redwalk(dolphins, diss = sp)
#' plot(cbd)
#' cutree(cbd, k = 2)
#' cutree(cbd, k = 4)
#'
cluster_redwalk <- function(graph, nodes = V(graph), diss = NULL)
{
  check_graph(graph);
  nv <- vcount(graph);
  edglst <- as_edgelist(graph, names = FALSE) - 1;
  storage.mode(edglst) <- "integer";
  ns0 <- length(nodes);
  nodes <- sort(unique(as.integer(nodes)));
  ns <- length(nodes);
  stopifnot(ns0 == ns, ns >= 2, all(nodes >= 1), all(nodes <= nv));
  all_nodes <- (ns == nv);

  # Check if diss is provided, calculate if not
  # TODO: really bad that shortest paths is a matrix rather than dist
  if(is.null(diss)) {
    if(all_nodes) {
      D <- dissimilarity_c(nv, edglst);
    } else {
      D <- dissimilarity_subsets_c(nv, edglst, nodes-1);
    }
  } else {
    # if given shortest paths as dist, convert to matrix
    if("dist" %in% class(diss)) {
      diss <- as.matrix(diss);
    }
    # need diss to be a matrix
    if(!("matrix" %in% class(diss))) {
      stop("diss must be of class matrix or dist");
    }
    # check square
    dim_sp <- dim(diss);
    if(dim_sp[1] != dim_sp[2])
      stop("diss has incorrect dimensions"); #need square

    if(all_nodes) {
      if(dim_sp[1] != nv)
        stop("diss has incorrect dimensions");
      D <- dissimilarity_sp_c(nv, edglst, diss);
    } else {
      if(dim_sp[1] != nv & dim_sp[1] != ns)
        stop("diss has incorrect dimensions");
      if(dim_sp[1] == nv)
        diss <- diss[nodes,nodes];
      D <- dissimilarity_subsets_sp_c(nv, edglst, nodes-1, diss);
    }
  }

  # return the hclust object
  if(requireNamespace("fastcluster", quietly = TRUE)) {
    return(fastcluster::hclust(D, method = "average"));
  } else {
    return(stats::hclust(D, method = "average"));
  }
}
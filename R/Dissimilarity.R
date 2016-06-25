#' Redwalk Dissimilarity Measure
#'
#' Compute the dissimilarity to be used in the redwalk clustering.
#'
#' @param graph an igraph object.
#' @param nodes a subset of the nodes in the graph to cluster, defaults to all
#' nodes (full community detection)
#' @param short_paths either a \eqn{|V|x|V|} symmetric dissimilarity matrix of
#' the vertices in \code{graph} or a \code{\link{dist}} object from the
#' \pkg{\link{stats}} package. If \code{NULL} (default), the shortest paths will
#' be calculated. Note: much faster if \code{diss} is provided and much faster
#' if \code{diss} is of class "matrix" rather than "dist".
#' @return \code{dissimilarity} returns a dist object.
#'
#' @export
#' @keywords cluster graph dissimilarity redwalk
#'
#' @examples
#' require(stats, quietly = TRUE)
#' D <- dissimilarity(karate)
#' D
#'
dissimilarity <- function(graph, nodes = V(graph), short_paths = NULL)
{
  # check the graph
  check_graph(graph)
  nv <- vcount(graph)

  # check the nodes
  nodes <- check_nodes(vcount(graph), nodes)
  ns <- length(nodes)

  # get shortest paths if needed, or check provided
  if(is.null(short_paths)) {
    short_paths = distances(graph, v = nodes, to = nodes, weights = NA)
    rownames(short_paths) <- NULL
    colnames(short_paths) <- NULL
  } else {
    # check short_paths arg
    if("dist" %in% class(short_paths)) {
      # check correct size
      nvd <- attr(short_paths, "Size")
      stopifnot(attr(short_paths, "Size") == ns)
      # convert to matrix
      short_paths = as.matrix(short_paths)
    } else if("matrix" %in% class(short_paths)) {
      # check square and correct size
      nm = dim(short_paths)
      stopifnot(nm[0] == nm[1], nm[0] == ns)
      # check symmetric
      stopifnot(all(short_paths == t(short_paths)))
    } else {
      # not a matrix or dist, error
      # TODO: might be coercible to matrix
      stop("short_paths must be of class 'dist' or 'matrix'")
    }
  }
  # call appropriate function
  if(ns == nv) {
    return(dissimilarity_full(graph, short_paths))
  } else {
    return(dissimilarity_subsets(graph, nodes, short_paths))
  }
}

# compute the dissimilarity for all the nodes in the graph
dissimilarity_full <- function(graph, short_paths)
{
  # for all nodes in the graph, D_ij is simply the average of the shortest paths
  # of the neighbors of i to j.
  # To make symmetric, D_ij = D_ji = min(D_ij, D_ji)
  # Can do adjacency matrix times shortest paths matrix then divide rows by the
  # degree of the node.
  A <- as_adj(graph, type = "both", names = FALSE)
  deg <- degree(graph, mode = "out", loops = FALSE)
  D <- as.matrix(A %*% short_paths / deg)
  return(as.dist(pmin(D, t(D))))
}

# compute the dissimilarity for a subset of the nodes in the graph
dissimilarity_subsets <- function(graph, nodes, short_paths)
{
  nv <- vcount(graph)
  nm <- length(nodes)
  L <- graph.laplacian(graph, sparse = TRUE) / degree(graph, mode = "out",
                                                      loops = FALSE)
  L[nodes,] <- 0
  diag(L) <- 1
  b <- Matrix(data = 0, nrow = nv, ncol = nm, sparse = FALSE)
  b[nodes,] <- short_paths
  D <- Matrix::solve(L, b)
  D2 <- matrix(0, nrow = nm, ncol = nm)
  for(v in 1:nm) {
    nbrs <- as.integer(neighbors(graph,nodes[v]))
    if(length(nbrs) <= 1) {
      D2[v,] <- D[nbrs,];
    } else {
      D2[v,] = apply(D[nbrs,], 2, mean);
    }
  }
  diag(D2) <- 0;
  return(as.dist(pmin(D2, t(D2)), diag = FALSE))
}

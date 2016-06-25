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

  # check the nodes
  nv <- vcount(graph)
  ns0 <- length(nodes);
  nodes <- sort(unique(as.integer(nodes)))
  ns <- length(nodes);
  stopifnot(ns0 == ns, ns >= 2, all(nodes >= 1), all(nodes <= nv))

  # get shortest paths if needed, or check provided
  if(is.null(short_paths)) {
    short_paths = as.dist(shortest_path_lengths(graph, nodes))
  } else {
    # check short_paths arg
    if("dist" %in% class(short_paths)) {
      # check correct size
      nvd <- attr(short_paths, "Size")
      stopifnot(attr(short_paths, "Size") == ns)
    } else if("matrix" %in% class(short_paths)) {
      # check square and correct size
      nm = dim(short_paths)
      stopifnot(nm[0] == nm[1], nm[0] == ns)
      # check symmetric
      stopifnot(all(short_pahts == t(short_paths)))
      # convert to dist object
      short_paths = as.dist(short_paths)
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
  stop("Not yet implemented")
}

# compute the dissimilarity for a subset of the nodes in the graph
dissimilarity_subsets <- function(graph, nodes, short_paths)
{
  stop("Not yet implemented")
}

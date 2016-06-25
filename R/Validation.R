#' Check Graph
#'
#' Checks whether a graph is okay for input to redwalk. The graph must be an
#' igraph graph object, be simple, connected, undirected. If \code{graph} has
#' weighted edges, \code{check_graph} allows it as all functions in
#' \link{\pkg{redwalk}} ignore edge weights.
#'
#' @param graph An igraph object
#' @return returns \code{TRUE} invisibly if the graph is valid. Otherwise,
#' \code{stop} is called, thowing an error and displaying the reason the graph
#' did not pass the check.
#'
#' @noRd
#' @keywords methods error
#'
#' @examples
#'  check_graph(karate)
#'
check_graph <- function(graph)
{
  if(!is.igraph(graph))
    stop("graph must be an igraph object");

  if(is.directed(graph))
    stop("graph must be undirected");

  if(!is.connected(graph))
    stop("graph must be connected");

  if(!is.simple(graph))
    stop("graph must be simple (cannot have multiple edges or loops)");

  invisible(TRUE);
}

#' Check Nodes
#'
#' Checks whether a set of nodes is valid based on \code{nv}.
#'
#' @param nv an integer representing the number of vertices in a graph.
#' @param nodes either a vector of numerics representing vertex IDs in a graph
#' or an \code{\link{igraph.vs}} object.
#' @return returns \code{nodes} if the original input is valid. The returned
#' \code{nodes} are sorted, unique, and integer.
#'
#' @noRd
#' @keywords methods error
#'
#' @examples
#'  check_nodes(12, c(1,4,6,11,12))
#'
check_nodes <- function(nv, nodes) {
  n0 <- length(nodes)
  nodes <- sort(unique(as.integer(nodes)))
  # check no duplicates and at least 2
  stopifnot(length(nodes) == n0, length(nodes) >= 2)
  # check nodes are in proper range
  stopifnot(nodes[1] >= 1, nodes[length(nodes)] <= nv)
  return(nodes)
}

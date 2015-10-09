#' Check Graph
#'
#' Checks whether a graph is okay for input to gclust. The graph must be an
#' igraph graph object, be simple, connected, undirected. If \code{graph} has
#' weighted edges, \code{check_graph} allows it as all functions in
#' \link{\pkg{gclust}} ignore edge weights.
#'
#' @noRd
#'
#' @param graph An igraph object
#' @return \code{check_graph} does not return a value. Instead, it will call
#' \code{stop} if all necessary properties are not satisfied.
#'
#' @keywords methods error
#'
#' @examples
#'  check_graph(karate)
#'
check_graph <- function(graph)
{
  if(class(graph) != "igraph")
    stop("graph must be an igraph object");

  if(is.directed(graph))
    stop("graph must be undirected");

  if(!is.connected(graph))
    stop("graph must be connected");

  if(!is.simple(graph))
    stop("graph must be simple (cannot have multiple edges or loops)");
}

# Alias for check_graph
is_undirected_connected_simple <- check_graph
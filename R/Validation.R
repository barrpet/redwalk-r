#' Check Graph
#'
#' Checks whether a graph is okay for input to gclust. The graph must be an
#' igraph graph object, be simple, connected, undirected. If \code{graph} has
#' weighted edges, \code{check_graph} allows it as all functions in
#' \link{\pkg{gclust}} ignore edge weights.
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
  if(class(graph) != "igraph")
    stop("graph must be an igraph object");

  if(is.directed(graph))
    stop("graph must be undirected");

  if(!is.connected(graph))
    stop("graph must be connected");

  if(!is.simple(graph))
    stop("graph must be simple (cannot have multiple edges or loops)");

  invisible(TRUE);
}

# Alias for check_graph
is_undirected_connected_simple <- check_graph
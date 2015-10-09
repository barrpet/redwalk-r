#' All Pairs Shortest Paths (Unweighted & Undirected)
#'
#' Computes all pairs shortest paths for an unweighted, undirected graph. For
#' graphs with these properties, this function is much faster than the function
#' \code{\link{distances}} in the \pkg{\link{igraph}} package.
#'
#' @aliases distances_uwud_fast
#' @param graph an igraph object.
#' @return returns a symmetric "matrix" object where the \eqn{ij}th entry is the
#' shortest path distance from \eqn{v_i} to \eqn{v_j}, \eqn{d(v_i,v_j)}, for all
#' \eqn{v_i,v_j \in V}.
#' @export
#' @keywords graphs
#'
#' @examples
#' sp <- all_shortest_paths_uwud_fast(dolphins)
#' sp
#' all(sp == igraph::distances(dolphins))
#'
all_shortest_paths_uwud_fast <- function(graph)
{
  check_graph(graph);
  nv <- vcount(graph);
  edglst <- as_edgelist(graph, names = FALSE) - 1;
  storage.mode(edglst) <- "integer";
  return(all_shortest_paths_c(nv, edglst));
}
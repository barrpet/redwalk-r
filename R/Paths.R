#' All Pairs Shortest Paths (Unweighted & Undirected)
#'
#' Computes all pairs shortest paths for an unweighted and undirected graph.
#' For graphs matching this description, this function is much faster than the
#' function \code{\link{distances}} in the igraph package.
#'
#' @param graph an igraph object.
#' @return returns a symmetric "matrix" object where the i,jth entry is the
#' shortest path distance from v_i to v_j for all vertices in \code{graph}.
#' @export
#' @keywords graphs
#'
#' @examples
#' require(stats)
#' cb <- cluster_berenhaut(karate)
#' plot(cb)
#'
all_shortest_paths_uwud_fast <- function(graph)
{
  check_graph(graph);
  nv <- vcount(graph);
  edglst <- as_edgelist(graph, names = FALSE) - 1;
  storage.mode(edglst) <- "integer";
  return(all_shortest_paths_c(nv, edglst));
}

# Alias for all_shortest_paths_uwud_fast
#' @describeIn all_shortest_paths_uwud_fast
#' @export distances_uwud_fast
distances_uwud_fast <- all_shortest_paths_uwud_fast
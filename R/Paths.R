#' All Pairs Shortest Paths (Unweighted & Undirected)
#'
#' Computes all pairs shortest paths for an unweighted, undirected graph. For
#' graphs with these properties, this function is much faster than the function
#' \code{\link{distances}} in the \pkg{\link{igraph}} package.
#'
#' @param graph an igraph object.
#' @param nodes a subset of the nodes of the graph to find the distances between.
#' defaults to all nodes (all pairs shortest path lengths).
#' @return returns a symmetric "matrix" object where the \eqn{ij}th entry is the
#' shortest path distance from \eqn{v_i} to \eqn{v_j}, \eqn{d(v_i,v_j)}, for all
#' \eqn{v_i,v_j \in V}.
#' @export
#' @keywords graphs
#'
#' @examples
#' sp <- all_pairs_shortest_paths(dolphins);
#' sp[1:10,1:10];
#' ## Check against igraphs distance function
#' all(sp == igraph::distances(dolphins, algorithm = "unweighted"));
#'
all_pairs_shortest_paths <- function(graph)
{
  check_graph(graph);
  nv <- vcount(graph);
  edglst <- as_edgelist(graph, names = FALSE) - 1;
  storage.mode(edglst) <- "integer";
  return(all_pairs_shortest_paths_c(nv, edglst));
}

#' @export subsets_shortest_paths
#' @describeIn all_pairs_shortest_paths
subsets_shortest_paths <- function(graph, nodes = V(graph))
{
  check_graph(graph);
  nv <- vcount(graph);
  edglst <- as_edgelist(graph, names = FALSE) - 1; # sub 1 for C++ functions
  storage.mode(edglst) <- "integer";
  ns0 <- length(nodes);
  nodes <- sort(unique(as.integer(nodes)));
  ns <- length(nodes);
  stopifnot(ns0 == ns, ns >= 2, all(nodes >= 1), all(nodes <= nv));
  if(ns == nv) {
    return(all_pairs_shortest_paths_c(nv, edglst));
  }
  return(subsets_shortest_paths_c(nv, edglst, nodes - 1));
}
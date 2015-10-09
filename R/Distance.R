#' Mean Neighbor Distances (Minimum)
#'
#' For each vertex \eqn{v_i} in \code{graph}, this function calculates the average of
#' the shortest paths from each neighbor of \eqn{v_i} to each \eqn{v_j} in
#' \code{graph}, but the matrix that is returned will be the minimum value of
#' the \eqn{ij}th entry and the \eqn{ji}th entry, resulting in a symmetric
#' matrix. e.g.,
#' \deqn{D_{ij} = \min{\{\frac{1}{deg(v_i)}\sum_{v_k \in N(v_i)}^{} d(v_k, v_j),
#' \frac{1}{deg(v_i)}\sum_{v_k \in N(v_j)}^{} d(v_k, v_i)\}}}
#'
#' @param graph an igraph object.
#' @param short_paths either a \eqn{|V|x|V|} symmetric matrix containing the shortest
#' path distances between the vertices in \code{graph} or a \code{\link{dist}}
#' object from the \code{\link{stats}} package. If \code{NULL} (default), the
#' shortest paths will be calculated. If shortest paths are available, this
#' functions runs at least 50\% faster. Counterintuitively, this function will
#' be faster if \code{short_paths} is of class "matrix" instead of class "dist".
#' \code{TRUE}, will return a "\code{\link{dist}}" object from \pkg{stats}.
#' @return \code{mean_neighbor_dist_min} returns either a "matrix" or a "dist"
#' object representing dissimilarities between vertices in a graph.
#' @export
#' @keywords graphs
#'
#' @examples
#' ## standard matrix representation
#' mean_neighbor_dist_min(karate)
#'
#' ## using precomputed shortest paths
#' sp <- all_shortest_paths_uwud_fast(dolphins)
#' mean_neighbor_dist_min(dolphins, short_paths = sp)
#'
mean_neighbor_dist_min <- function(graph, short_paths = NULL)
{
  # Check that the graph is valid
  check_graph(graph);

  # Get number of vertices and edges, and create edgelist
  nv <- vcount(graph);
  ne <- ecount(graph);
  edglst <- as_edgelist(graph, names = FALSE) - 1; # sub 1 for C++ functions
  storage.mode(edglst) <- "integer";

  # Check if short_paths is provided, calculate if not
  if(is.null(short_paths)) {
    D2 <- d2_berenhaut_c(nv, edglst);
  } else {
    # if given shortest paths as dist, convert to matrix
    if(class(short_paths) == "dist") {
      short_paths <- as.matrix(short_paths);
    }
    # need short_paths to be a matrix
    if(class(short_paths) != "matrix") {
      stop("short_paths must be of class matrix or dist");
    }

    # check correct dimensions
    dim_sp <- dim(short_paths);
    if(!all.equal(dim_sp, c(nv, nv))) {
      stop("short_paths has incorrect dimensions");
    }
    # TODO: check symmetric? will eat up time

    # Finally get D2
    D2 <- d2_berenhaut_sp_c(nv, edglst, short_paths);
  }

  # Make D2 a dist object
  attr(D2, "Diag") <- FALSE;
  attr(D2, "Upper") <- FALSE;
  attr(D2, "Size") <- nv;
  attr(D2, "class") <- "dist";

  # Return the dist object
  return(D2);
}

# Alias for mean_neighbor_dist_min
#' @describeIn mean_neighbor_dist_min
#' @export get_d2_min
get_d2_min <- mean_neighbor_dist_min
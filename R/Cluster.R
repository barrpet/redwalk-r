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
#' @param short_paths either a \eqn{|V|x|V|} symmetric matrix containing the shortest
#' path distances between the vertices in \code{graph} or a \code{\link{dist}}
#' object from the \pkg{\link{stats}} package. If \code{NULL} (default), the
#' shortest paths will be calculated. Note: much faster if \code{short_paths} is
#' provided and much faster if \code{short_paths} is of class "matrix" rather
#' than "dist".
#' @return \code{cluster_berenhaut} returns a \code{\link{hclust}} object from
#' the \pkg{\link{stats}} package.
#' @export
#' @keywords cluster graphs
#'
#' @examples
#' require(stats)
#' cb <- cluster_berenhaut(karate)
#' ## cb is an hclust object
#' plot(cb)
#' ## membership for different community sizes (k)
#' cutree(cb, k = 2)
#' cutree(cb, k = 3)
#'
#' ## using precomputed shortest paths
#' sp <- all_pairs_shortest_paths(dolphins)
#' cbd <- cluster_berenhaut(dolphins, short_paths = sp)
#' plot(cbd)
#' cutree(cbd, k = 2)
#' cutree(cbd, k = 4)
#'
cluster_berenhaut <- function(graph, short_paths = NULL)
{
  # Check that the graph is valid
  check_graph(graph);

  # Get number of vertices and edges, and create edgelist
  nv <- vcount(graph);
  edglst <- as_edgelist(graph, names = FALSE) - 1; # sub 1 for C++ functions
  storage.mode(edglst) <- "integer";

  # Check if short_paths is provided, calculate if not
  # TODO: really bad that shortest paths is a matrix rather than dist
  if(is.null(short_paths)) {
    D <- dissimilarity_c(nv, edglst);
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
    # TODO: check symmetric? won't be fast

    # Finally get D
    D <- dissimilarity_sp_c(nv, edglst, short_paths);
  }

  # Make D a dist object
  attr(D, "Diag") <- FALSE;
  attr(D, "Upper") <- FALSE;
  attr(D, "Size") <- nv;
  attr(D, "class") <- "dist";

  # return the hclust object
  return(hclust_avg(D));
}

# Alias for cluster_berenhaut
#' @export berenhaut.community
#' @describeIn cluster_berenhaut
berenhaut.community <- cluster_berenhaut
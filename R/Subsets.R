#' @export cluster_berenhaut_subsets
#' @describeIn cluster_berenhaut
cluster_berenhaut_subsets <- function(graph, nodes = V(graph), short_paths = NULL)
{
  # Check the graph
  nv <- vcount(graph);
  stopifnot(is.igraph(graph), is.connected(graph), is.simple(graph), !is.directed(graph));
  if(is.weighted(graph)) {
    warning("graph has edge weights which will be ignored");
  }

  # Check the nodes
  ns0 <- length(nodes);
  nodes <- sort(unique(as.integer(nodes)));
  ns <- length(nodes);
  stopifnot(ns0 == ns, ns >= 2, all(nodes >= 1), all(nodes <= nv));

  # TODO: if S == V, call full version

  edglst <- as_edgelist(graph, names = FALSE) - 1; # sub 1 for C++ functions
  storage.mode(edglst) <- "integer";

  # Check the short_paths
  if(missing(short_paths)) {
    short_paths <- subsets_shortest_paths_c(nv, edglst, nodes - 1);
  } else {
    stopifnot(class(short_paths) == "matrix");
    stopifnot(dim(short_paths)[1] == dim(short_paths)[2]);
    stopifnot(dim(short_paths)[1] == ns || dim(short_paths)[1] == nv);
    if(dim(short_paths)[1] == nv) {
      short_paths <- short_paths[nodes,];
    }
  }
  D <- dissimilarity_subsets_sp_c(nv, edglst, nodes-1, short_paths);
  return(hclust_avg(D));
}


cluster_berenhaut_subsets_slow <- function(graph, nodes = V(graph), short_paths = NULL)
{
  # Check the graph
  nv <- vcount(graph);
  stopifnot(is.igraph(graph), is.connected(graph), is.simple(graph), !is.directed(graph));
  if(is.weighted(graph)) {
    warning("graph has edge weights which will be ignored");
  }

  # Check the nodes
  ns0 <- length(nodes);
  nodes <- sort(unique(as.integer(nodes)));
  ns <- length(nodes);
  stopifnot(ns0 == ns, ns >= 2, all(nodes >= 1), all(nodes <= nv));

  # TODO: if S == V, call full version

  # Check the short_paths
  if(missing(short_paths)) {
    short_paths <- distances(graph, v = nodes, to = nodes, weights = NA);
  } else {
    stopifnot(class(short_paths) == "matrix");
    stopifnot(dim(short_paths)[1] == dim(short_paths)[2]);
    stopifnot(dim(short_paths)[1] == ns || dim(short_paths)[1] == nv);
    if(dim(short_paths)[1] == nv) {
      short_paths <- short_paths[nodes,];
    }
  }

  # Get RW Laplacian
  L <- graph.laplacian(graph, normalized = FALSE, weights = NA, sparse = TRUE);
  L <- L / degree(graph, mode = "out");

  # Modify RW Laplacian
  L[nodes,] <- 0;
  diag(L) <- 1;

  # Set up b
  b <- matrix(0, nrow = nv, ncol = ns);
  b[nodes,] <- short_paths;

  # Solve
  x <- solve(L, b);

  # Set up D2
  D2 <- matrix(0, nrow = ns, ncol = ns);

  # Do average of neighbors
  for(vi in 1:ns) {
    nbrs <- neighbors(graph, nodes[vi], mode = "out");
    for(n in nbrs) {
      D2[vi,] <- D2[vi,] + x[n,];
    }
    D2[vi,] <- D2[vi,] / length(nbrs);
  }

  # Get min of D2 (make symmetric), then make dist
  D2 <- as.dist(pmin(D2, t(D2)));

  # Do the clustering
  return(hclust(D2, method = "average"));
}
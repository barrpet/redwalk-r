# Does D2 for either subsets or full
# G is the graph, S is a subset of the vertices of G
# Returns an |S|x|S| distance matrix
d1fast <- function(G, S = V(G))
{
  require("Matrix", quietly = TRUE, warn.conflicts = FALSE);
  stopifnot(is.igraph(G), is.numeric(S) | class(S) == "igraph.vs");
  nv <- vcount(G);
  S <- sort(unique(as.integer(S)));
  nm <- length(S);
  stopifnot(length(S) > 2, min(S) >= 1, max(S) <= nv);

  if(nm == nv) # This for for the full case
  {
    deg <- as.vector(degree(G, mode = "out"));
    P <- as_adj(G, type = "both", names = FALSE, sparse = TRUE) * (1 / deg);
    W <- Matrix(deg, nrow = nv, ncol = nv, byrow = TRUE) / sum(deg);
    N <- solve(Diagonal(nv) - P - W);
    NJ <- Matrix(diag(N), nrow = nv, ncol = nv, byrow = TRUE);
    D1 <- (NJ - N) / W;
    diag(D1) <- 0;
    return(as.matrix(D1)); # NOT TRANSPOSED, USE '1 / colSums' FOR SVALS
  }
  else # This is for the subsets case
  {
    L <- graph.laplacian(G, sparse = TRUE) / degree(G, mode = "out");
    rownames(L) <- NULL; colnames(L) <- NULL;
    UN <- setdiff(1:nv,S); # Unoccupied
    L[UN,] <- L[UN,] * 0.5; # 1/2 chance for unoccupied to go to target
    diag(L) <- 1;
    Li <- solve(L); # invert, bad, slow # 31% of total time
    b <- Matrix(data = 0, nrow = nv, ncol = nm, sparse = FALSE); # free walk
    b[S,] <- 1;
    diag(b[S,]) <- 0;
    Z <- Li %*% b; # 52% of total time
    U <- Matrix(data = 0, nrow = nv, ncol = nm, sparse = TRUE); # allocate U
    diag(U[S,]) <- 1;
    Y <- Li %*% U;
    ab2 <- diag(Z[S,]) / diag(Y[S,]);
    D1 <- Z - (Y * ab2[col(Y)]);
    return(as.matrix(D1[S,])); # NOT TRANSPOSED, USE '1 / colSums' FOR SVALS
  }
}

svfast <- function(G, S = V(G))
{
  return(1 / colSums(fastd1(G,S)));
}

# Does D2 for either subsets or full
# G is the graph, S is a subset of the vertices of G
# Returns an |S|x|S| distance matrix
d2fast <- function(G, S = V(G))
{
  require("Matrix", quietly = TRUE, warn.conflicts = FALSE);
  stopifnot(is.igraph(G), is.numeric(S) | class(S) == "igraph.vs");
  nv <- vcount(G);
  S <- sort(unique(as.integer(S)));
  nm <- length(S);
  stopifnot(length(S) > 2, min(S) >= 1, max(S) <= nv);
  D <- distances(G, v = S, to = S, mode = "out", weights = NA);
  rownames(D) <- NULL; colnames(D) <- NULL;
  if(nm < nv) {
    L <- graph.laplacian(G, sparse = TRUE) / degree(G, mode = "out", loops = FALSE);
    L[S,] <- 0; diag(L) <- 1;
    b <- Matrix(data = 0, nrow = nv, ncol = nm, sparse = TRUE);
    b[S,] <- D;
    D <- solve(L, b);
  }
  D2 <- matrix(0, nrow = nm, ncol = nm);
  for(v in 1:nm) {
    nbrs <- as.integer(neighbors(G,S[v]));
    if(length(nbrs) <= 1) {
      D2[v,] <- D[nbrs,];
    } else {
      D2[v,] = apply(D[nbrs,], 2, mean);
    }
  }
  diag(D2) <- 0;
  return(D2);
}
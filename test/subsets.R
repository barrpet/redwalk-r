# Defaults are for D2
# For D1, T = setdiff(1:vcount(G),S), s = 0, a = 1
finddist3 <- function (G,S,T=S,s=1,a=0,SP=NULL)
{
  ################### SETUP ###################
  nv <- vcount(G) # get number of vertices
  nm <- length(S) # get number of targets
  D <- matrix(0,nv,nm) # allocate the final D distance matrix, nv x nm

  # if distances between vertices are not passed in, use shortest paths
  if(is.null(SP)) {
    SP <- as.matrix(distances(G, mode = "out", weights = NA));
    rownames(SP) <- NULL;
    colnames(SP) <- NULL;
  }

  # Set up random walk normalized laplacian matrix
  M <- as_adj(G, names = FALSE, sparse = FALSE) # get adjacency matrix of A
  deg <- as.vector(degree(G, mode = "out", loops = FALSE)) # get degree of each vertex
  M <- (-M) / matrix(deg,nv,nv); # create transition matrix (if A_ij = 1, M_ij = -1/deg(i))
  diag(M) <- 1 # set diagonal of transition matrix to 1

  ################### WORK ###################
  for (v in 1:nv) # for each vertex in G
  {
    # If v is not in T, Su = T
    Su <- setdiff(T,v) # remove current vertex from T
    M2 <- M; # get transition matrix
    M2[setdiff(Su,v),] <- 0 # zero out rows of M for each vertex in Su
    M2[cbind(Su,Su)] <- 1 # put 1 back on diagonal for zeroed out rows
    for (m in 1:nm) # for all targets
    {
      M3 <- M2; # get transition matrix with rows of Su zeroed out
      M3[S[m],] <- 0 # zero out the row of transition matrix for current target
      M3[S[m],S[m]] <- 1 # put 1 back on the diagonal for zeroed out row

      b <- rep(a,nv) # set be to all a (0 or 1)
      b[Su] <- s*SP[S[m],Su] # set rows of b for Su to s * shortest path current target to all SU
      b[v] <- a # b for current vertex is a
      b[S[m]] <- 0 # b for current target is 0

      e <- solve(M3,b) # solve Me=b
      D[v,m] <- e[v] # set D_it to e_i, (distance from vertex i to target t is e_i)
    }
  }
  return(D);
}

slow_d1 <- function(G, S)
{
  T = setdiff(1:vcount(G), S);
  D1 <- finddist3(G = G, S = S, T = T, a = 1, s = 0, SP = NULL);
  return(D1);
}

slow_sv <- function(G, S)
{
  T = setdiff(1:vcount(G), S);
  D1 <- finddist3(G = G, S = S, T = T, a = 1, s = 0, SP = NULL);
  sv <- 1 / colSums(D1);
  return(sv);
}

slow_d2 <- function(G, S)
{
  D2 <- finddist3(G = G, S = S, T = S, a = 0, s = 1, SP = NULL);
  return(D2);
}
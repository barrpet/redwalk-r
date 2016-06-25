library(redwalk, quietly = TRUE, warn.conflicts = FALSE)
context("dissimilarity")

get_avg_nbr_shrt_path_min <- function(graph, v1, v2)
{
  nbrs_v1 <- neighbors(graph, v = v1, mode = "out");
  nbrs_v2 <- neighbors(graph, v = v2, mode = "out");
  sp_v1 <- distances(graph, v = nbrs_v1, to = v2, weights = NA,
                     algorithm = "unweighted");
  sp_v2 <- distances(graph, v = nbrs_v2, to = v1, weights = NA,
                     algorithm = "unweighted");
  return(min(mean(sp_v1), mean(sp_v2)));
}

# Does D2 for either subsets or full
# G is the graph, S is a subset of the vertices of G
# Returns an |S|x|S| distance matrix
d2rversion <- function(G, S)
{
  nv <- vcount(G);
  stopifnot(is.igraph(G), is.numeric(S) | class(S) == "igraph.vs");
  stopifnot(length(S) > 2, length(S) <= nv, min(S) >= 1, max(S) <= nv);
  S <- sort(unique(as.integer(S)));
  nv <- vcount(G); nm <- length(S);
  D <- distances(G, v = S, to = S, mode = "out", weights = NA);
  rownames(D) <- NULL; colnames(D) <- NULL;
  if(nm < nv) {
    L <- graph.laplacian(G, sparse = TRUE) / degree(G, mode = "out", loops = FALSE);
    L[S,] <- 0; diag(L) <- 1;
    b <- Matrix(data = 0, nrow = nv, ncol = nm, sparse = FALSE);
    b[S,] <- D;
    D <- Matrix::solve(L, b);
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

test_that("dissimilarity (karate) works", {
  g <- karate;
  edg <- as_edgelist(g, F) - 1L
  storage.mode(edg) <- "integer"
  nv <- vcount(g)
  d2 <- as.matrix(round(redwalk:::dissimilarity(g), 8));
  d_1_34 <- round(get_avg_nbr_shrt_path_min(g, 1, 34), 8);
  d_6_10 <- round(get_avg_nbr_shrt_path_min(g, 6, 10), 8);
  expect_that(d2[1,34], equals(d_1_34));
  expect_that(d2[34,1], equals(d_1_34));
  expect_that(d2[6,10], equals(d_6_10));
  expect_that(d2[10,6], equals(d_6_10));
  expect_that(d2[7,10], equals(d_6_10)); # 6 & 7 should be same
  expect_that(d2[10,7], equals(d_6_10)); # 6 & 7 should be same
  expect_that(d2[26,10] == d_6_10, is_false()); # 26 & 6 should be diff
  expect_that(d2[10,26] == d_6_10, is_false()); # 26 & 6 should be diff
})

test_that("dissimilarity subsets (dolphins) works", {
  g <- dolphins;
  s <- c(2L:12L, 40L:62L);
  edg <- as_edgelist(g, F) - 1L;
  storage.mode(edg) <- "integer";
  nv <- vcount(g);
  d2 <- as.matrix(round(dissimilarity(g, s), 8));
  d2g <- d2rversion(g, s);
  d2g <- round(pmin(d2g, t(d2g)), 8);
  expect_that(as.vector(d2), equals(as.vector(d2g)));
})

unloadNamespace('redwalk')
#unloadNamespace('igraph')

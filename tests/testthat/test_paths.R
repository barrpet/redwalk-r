library(redwalk, quietly = TRUE, warn.conflicts = FALSE)
context("paths")

make_simple_network <- function() {
  G <- make_graph(c(1,2,1,3,1,4,2,3,3,4,2,5,5,6,5,7,5,8,5,9,6,8,6,9,7,8,7,9,9,10,
                    10,11,10,13,10,14,10,15,11,12,11,13,11,15,12,14,12,15,13,14,
                    14,15,14,3,15,4), directed = FALSE)
  V(G)$membership <- c(1,1,1,1,2,2,2,2,2,3,3,3,3,3,3)
  V(G)$x <- c(-0.50,-0.35,-0.31,-0.44,0.11,0.24,0.44,0.50,0.31,0.15,0.06,-0.19,
              -0.28,-0.10,-0.05)
  V(G)$y <- c(0.32,0.46,0.27,0.13,0.30,0.50,0.48,0.33,0.17,-0.25,-0.50,-0.50,
              -0.28,-0.10,-0.34)
  V(G)$color <- 'blue'
  V(G)$color[5:9] <- 'green'
  V(G)$color[10:15] <- 'red'
  V(G)$label.color <- 'white'
  V(G)$label.color[5:9] <- 'black'
  G$name <- 'Simple Network'
  return(G)
}

test_that("all pairs shortest paths (simple_network) works", {
  g <- make_simple_network()
  my_sp <- shortest_path_lengths(g)
  ig_sp <- distances(g, weights = NA, algorithm = "unweighted")
  expect_is(my_sp, "matrix")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

test_that("subsets shortest paths (simple_network) works", {
  g <- make_simple_network()
  s <- c(1:4, 6:8, 10:12)
  my_sp <- shortest_path_lengths(g, s)
  ig_sp <- distances(g, v = s, to = s, weights = NA, algorithm = "unweighted")
  expect_is(my_sp, "matrix")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

test_that("all pairs shortest paths (karate) works", {
  g <- karate
  my_sp <- shortest_path_lengths(g)
  ig_sp <- distances(g, weights = NA, algorithm = "unweighted")
  expect_is(my_sp, "matrix")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

test_that("subsets shortest paths (karate) works", {
  g <- karate
  s <- c(2L:8L, 22L:30L)
  my_sp <- shortest_path_lengths(g, s)
  ig_sp <- distances(g, v = s, to = s, weights = NA, algorithm = "unweighted")
  expect_is(my_sp, "matrix")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

unloadNamespace('redwalk')
#unloadNamespace('igraph')

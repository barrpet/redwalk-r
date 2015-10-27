context("paths")

test_that("all pairs shortest paths (karate) works", {
  library("igraph")
  library("gclust")
  g <- karate
  my_sp <- all_pairs_shortest_paths(g)
  ig_sp <- distances(g, weights = NA, algorithm = "unweighted")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

test_that("all pairs shortest paths (dolphins) works", {
  library("igraph")
  library("gclust")
  g <- dolphins
  my_sp <- all_pairs_shortest_paths(g)
  ig_sp <- distances(g, weights = NA, algorithm = "unweighted")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})
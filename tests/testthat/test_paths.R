context("paths")

test_that("all pairs shortest paths (karate) works", {
  library("igraph")
  library("redwalk")
  g <- karate
  my_sp <- shortest_path_lengths(g)
  ig_sp <- distances(g, weights = NA, algorithm = "unweighted")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

test_that("subsets shortest paths (karate) works", {
  library("igraph")
  library("redwalk")
  g <- karate
  s <- c(2L:8L, 22L:30L)
  my_sp <- shortest_path_lengths(g, s)
  ig_sp <- distances(g, v = s, to = s, weights = NA, algorithm = "unweighted")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

test_that("all pairs shortest paths (dolphins) works", {
  library("igraph")
  library("redwalk")
  g <- dolphins
  my_sp <- shortest_path_lengths(g)
  ig_sp <- distances(g, weights = NA, algorithm = "unweighted")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})

test_that("subsets shortest paths (dolphins) works", {
  library("igraph")
  library("redwalk")
  g <- dolphins
  s <- 2L:vcount(g)
  my_sp <- shortest_path_lengths(g, s)
  ig_sp <- distances(g, v = s, to = s, weights = NA, algorithm = "unweighted")
  expect_that(as.vector(my_sp), equals(as.vector(ig_sp)))
})
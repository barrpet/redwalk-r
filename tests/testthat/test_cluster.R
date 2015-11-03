context("cluster")

test_that("cluster_berenhaut (karate) works", {
  library("gclust");
  g <- karate;
  sp <- distances(g, mode = "out", weights = NA, algorithm = "unweighted");
  cb <- cluster_berenhaut(g);
  cbsp <- cluster_berenhaut(g, short_paths = sp);
  gtru <- V(g)$membership;

  expect_that(igraph::compare(stats::cutree(cb, 5), stats::cutree(cbsp, 5),
    method = "nmi"), equals(1),
    label = "same with and without provided shortest paths for 5 communities");

  expect_that(igraph::compare(stats::cutree(cb, 2), V(g)$membership, method =
    "nmi"), equals(1), label = "clustering matches ground truth");

  expect_that(igraph::compare(stats::cutree(cbsp, 2), V(g)$membership, method =
    "nmi"), equals(1),
    label = "clustering with supplied shortest paths matches ground truth");
})

test_that("cluster_berenhaut (dolphins) works", {
  library("gclust");
  g <- dolphins;
  sp <- distances(g, mode = "out", weights = NA, algorithm = "unweighted");
  cb <- cluster_berenhaut(g);
  cbsp <- cluster_berenhaut(g, short_paths = sp);
  gtru <- V(g)$membership;

  expect_that(igraph::compare(stats::cutree(cb, 5), stats::cutree(cbsp, 5),
    method = "nmi"), equals(1),
    label = "same with and without provided shortest paths for 5 communities");

  expect_equal(igraph::compare(stats::cutree(cb, 2), V(g)$membership, method =
    "nmi"), expected = 0.88835, tolerance = 0.001, scale = 1,
    label = "nmi close to 0.888359 (miss 1)");

  expect_equal(igraph::compare(stats::cutree(cbsp, 2), V(g)$membership, method =
    "nmi"), expected = 0.88835, tolerance = 0.001, scale = 1,
    label = "nmi close to 0.888359 (miss 1) for provided shortest paths");
})
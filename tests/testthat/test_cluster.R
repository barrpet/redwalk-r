library(redwalk, quietly = TRUE, warn.conflicts = FALSE)
context("cluster")

test_that("cluster_redwalk (karate) works", {
  g <- karate;
  sp <- distances(g, mode = "out", weights = NA, algorithm = "unweighted");
  cb <- cluster_redwalk(g);
  cbsp <- cluster_redwalk(g, short_paths = sp);
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

test_that("cluster_redwalk (dolphins) works", {
  g <- dolphins;
  sp <- distances(g, mode = "out", weights = NA, algorithm = "unweighted");
  cb <- cluster_redwalk(g);
  cbsp <- cluster_redwalk(g, short_paths = sp);
  gtru <- V(g)$membership;

  expect_that(igraph::compare(stats::cutree(cb, 5), stats::cutree(cbsp, 5),
    method = "nmi"), equals(1),
    label = "same with and without provided shortest paths for 5 communities");

  # expected nmi (different for fastcluster due to tiebreaks)
  exp_nmi <- 0.8888359
  if(requireNamespace("fastcluster", quietly = TRUE)) { exp_nmi <- 0.8082501; }
  expect_equal(igraph::compare(stats::cutree(cb, 2), V(g)$membership, method =
    "nmi"), expected = exp_nmi, tolerance = 0.001, scale = 1,
    label = "no short_paths provided, testing against known nmi");

  expect_equal(igraph::compare(stats::cutree(cbsp, 2), V(g)$membership, method =
    "nmi"), expected = exp_nmi, tolerance = 0.001, scale = 1,
    label = "short_paths provided, testing against known nmi");
})

unloadNamespace('redwalk')
#unloadNamespace('igraph')

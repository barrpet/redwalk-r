context("validation")

test_that("check_graph works", {
  # Good graph
  g_good <- karate;

  # Bad graph, not a graph
  g_bad <- karate;
  class(g_bad) <- NULL;

  # Directed graph
  g_dir <- graph.ring(10, directed = TRUE);

  # Loop graph
  g_loop <- karate;
  g_loop[1,1] <- 1;

  # Multiedge graph
  g_multi <- make_graph(c(1,2,1,2,2,3,3,4,4,1), directed = FALSE);

  # Disconnected graph
  g_dis <- make_graph(c(1,2,2,3,3,4,4,1,5,6,6,7,7,5), directed = FALSE);

  expect_error(redwalk:::check_graph(g_bad), label = "not an igraph");
  expect_error(redwalk:::check_graph(g_dir), label = "directed igraph");
  expect_error(redwalk:::check_graph(g_loop), label = "igraph with loop");
  expect_error(redwalk:::check_graph(g_multi), label = "igraph with multiedge");
  expect_error(redwalk:::check_graph(g_dis), label = "disconnected igraph");
  expect_true(redwalk:::check_graph(g_good), label = "good igraph (karate)");
})
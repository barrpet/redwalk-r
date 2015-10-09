context("distance")

get_avg_nbr_shrt_path_min <- function(graph, v1, v2)
{
  library("igraph");
  library("gclust");
  nbrs_v1 <- neighbors(graph, v = v1, mode = "out");
  nbrs_v2 <- neighbors(graph, v = v2, mode = "out");
  sp_v1 <- distances(graph, v = nbrs_v1, to = v2, weights = NA,
                     algorithm = "unweighted");
  sp_v2 <- distances(graph, v = nbrs_v2, to = v1, weights = NA,
                     algorithm = "unweighted");
  return(min(mean(sp_v1), mean(sp_v2)));
}

test_that("mean_neighbor_dist_min (karate) works", {
  library("igraph");
  library("gclust");
  g <- karate;
  d2 <- as.matrix(round(mean_neighbor_dist_min(g), 8));
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

test_that("mean_neighbor_dist_min (dolphins) works", {
  library("igraph");
  library("gclust");
  g <- dolphins;
  d2 <- as.matrix(round(mean_neighbor_dist_min(g), 8));
  d_4_40 <- round(get_avg_nbr_shrt_path_min(g, 4, 40), 8);
  expect_that(d2[4,40], equals(d_4_40));
  expect_that(d2[40,4], equals(d_4_40));
  expect_that(sum(d2[23,] == d2[32,]), equals(60));
})
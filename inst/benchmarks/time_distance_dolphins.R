time_group("distance_dolphins")

time_that(desc = "D2", replications = 1000,
  init = {
    library("redwalk"); data("dolphins"); g <- dolphins;
    sp <- all_shortest_paths_uwud_fast(g);
  },
  code = {
    mean_neighbor_dist_min(g, short_paths = sp);
  }
)

time_that(desc = "D2 as dist", replications = 1000,
  init = {
    library("redwalk"); data("dolphins"); g <- dolphins;
    sp <- all_shortest_paths_uwud_fast(g);
  },
  code = {
    mean_neighbor_dist_min(g, short_paths = sp, as_dist = TRUE);
  }
)

time_that(desc = "D2 w/ SP", replications = 1000,
  init = { library("redwalk"); data("dolphins"); g <- dolphins },
  code = {
    mean_neighbor_dist_min(g);
  }
)
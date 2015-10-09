time_group("distance_polblogs")

time_that(desc = "D2 w/ SP", replications = 5,
  init = {
    library("gclust");
    data("polblogs");
    g <- polblogs;
  },
  code = {
    mean_neighbor_dist_min(g);
  }
)

time_that(desc = "D2", replications = 5,
  init = {
    library("gclust");
    data("polblogs");
    g <- polblogs;
    sp <- all_shortest_paths_uwud_fast(g);
  },
  code = {
    mean_neighbor_dist_min(g, short_paths = sp);
  }
)
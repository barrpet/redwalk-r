time_group("cluster_polblogs")

time_that(desc = "berenhaut w/ SP", replications = 5,
  init = { library("gclust"); data("polblogs"); g <- polblogs; },
  code = {
    cluster_berenhaut(g);
  }
)

time_that(desc = "berenhaut", replications = 5,
  init = {
    library("gclust"); data("polblogs"); g <- polblogs;
    sp <- all_shortest_paths_uwud_fast(g);
  },
  code = {
    cluster_berenhaut(g, short_paths = sp);
  }
)

time_that(desc = "walktrap 3", replications = 5,
  init = { library("gclust"); data("polblogs"); g <- polblogs; },
  code = {
    cluster_walktrap(g, steps = 3);
  }
)

time_that(desc = "walktrap 4", replications = 5,
  init = { library("gclust"); data("polblogs"); g <- polblogs; },
  code = {
    cluster_walktrap(g, steps = 4);
  }
)

time_that(desc = "walktrap 5", replications = 5,
  init = { library("gclust"); data("polblogs"); g <- polblogs; },
  code = {
    cluster_walktrap(g, steps = 5);
  }
)

time_that(desc = "walktrap 6", replications = 5,
  init = { library("gclust"); data("polblogs"); g <- polblogs; },
  code = {
    cluster_walktrap(g, steps = 6);
  }
)

time_that(desc = "leading eigen", replications = 5,
  init = { library("gclust"); data("polblogs"); g <- polblogs; },
  code = {
    cluster_leading_eigen(g);
  }
)

time_that(desc = "fast greedy", replications = 5,
  init = { library("gclust"); data("polblogs"); g <- polblogs; },
  code = {
    cluster_fast_greedy(g);
  }
)
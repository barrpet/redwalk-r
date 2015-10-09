time_group("paths")

time_that(desc = "gclust polbooks", replications = 100,
  init = { library("gclust"); data(polbooks); g <- polbooks },
  code = {
    all_shortest_paths_uwud_fast(g)
  }
)

time_that(desc = "igraph polbooks", replications = 100,
  init = { library("gclust"); data(polbooks); g <- polbooks },
  code = {
    distances(g, weights = NA, algorithm = "unweighted")
  }
)

time_that(desc = "gclust polblogs", replications = 20,
  init = { library("gclust"); data(polblogs); g <- polblogs },
  code = {
    all_shortest_paths_uwud_fast(g)
  }
)

time_that(desc = "igraph polblogs", replications = 20,
  init = { library("gclust"); data(polblogs); g <- polblogs },
  code = {
    distances(g, weights = NA, algorithm = "unweighted")
  }
)
library('rbenchmark', quietly = TRUE, warn.conflicts = FALSE)
library('redwalk', quietly = TRUE, warn.conflicts = FALSE)

comparison_benchmark <- function(graph_name, reps = 5) {
  g <- get(as.character(graph_name))
  sp <- distances(g, weights = NA, algorithm = "unweighted")

  res <- rbenchmark::benchmark(cluster_redwalk(g),
                               cluster_redwalk(g, short_paths = sp),
                               cluster_walktrap(g, steps = 3),
                               cluster_walktrap(g, steps = 4),
                               cluster_walktrap(g, steps = 5),
                               cluster_walktrap(g, steps = 6),
                               cluster_leading_eigen(g),
                               cluster_fast_greedy(g),
                               order = "elapsed", replications = reps)

  print(paste("+++ ", graph_name, " +++"))
  print(res)
  print('')
}

comparison_benchmark('dolphins', 100)
comparison_benchmark('polbooks', 50)
comparison_benchmark('polblogs', 5)
comparison_benchmark('power', 2)

unloadNamespace('rbenchmark')

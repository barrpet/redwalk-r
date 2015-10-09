\name{dolphins}
\alias{dolphins}
\docType{data}
\title{
  Dolphin Social Network
}
\description{
  Contains an undirected social network of frequent associations between 62
  dolphins in a community living off Doubtful Sound, New Zealand, as compiled by
  Lusseau et al. (2003).
}
\usage{data("dolphins")}
\format{
  An undirected \code{\link{igraph}} graph object with 62 vertices and 159
  edges.

  Graph attributes: \sQuote{name}, \sQuote{Description}, \sQuote{Author},
  \sQuote{Citation}, \sQuote{URL}.

  Vertex attributes: \sQuote{name}, \sQuote{membership}, \sQuote{color}.

  Edge attributes: NA
}
\source{
  http://www-personal.umich.edu/~mejn/netdata/
}
\references{
  D. Lusseau, K. Schneider, O. J. Boisseau, P. Haase, E. Slooten, and S. M.
  Dawson, The bottlenose dolphin community of Doubtful Sound features a large
  proportion of long-lasting associations, Behavioral Ecology and Sociobiology
  54, 396-405 (2003).

  D. Lusseau, The emergent properties of a dolphin social network, Proc. R. Soc.
  London B (suppl.) 270, S186-S188 (2003).

  D. Lusseau, Evidence for social role in a dolphin social network, Preprint
  q-bio/0607048 (http://arxiv.org/abs/q-bio.PE/0607048)
}
\keyword{datasets}
\keyword{networks}
\name{santafe}
\alias{santafe}
\docType{data}
\title{
  The Santa Fe Institute Collaboration Network
}
\description{
  A collaboration network of scientists at the Santa Fe Institute, an
  interdisciplinary research center in Santa Fe, New Mexico.
}
\usage{data("santafe")}
\format{
  An undirected \code{\link{igraph}} graph object with 118 vertices and 200
  edges.

  Graph attributes: \sQuote{name}, \sQuote{Description}, \sQuote{Author},
  \sQuote{Citation}, \sQuote{URL}.

  Vertex attributes: \sQuote{name}, \sQuote{membership}.

  Edge attributes: NA
}
\source{
  http://www.pnas.org/content/99/12/7821.abstract
}
\references{
  M. Girvan and M. E. J. Newman, "Community Structure in Social and Biological
  Networks," Proceedings of the National Academy of Sciences of the United
  States of America, vol. 99, pp. 7821-7826, 2002.
}
\keyword{datasets}
\keyword{networks}
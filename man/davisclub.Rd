\name{davisclub}
\alias{davisclub}
\docType{data}
\title{
  Davis's Southern Club Women
}
\description{
  These data were collected by Davis et al in the 1930s. They represent observed
  attendance at 14 social events by 18 Southern women.
}
\usage{data("davisclub")}
\format{
  An undirected \code{\link{igraph}} graph object with 32 vertices and 93 edges.

  Graph attributes: \sQuote{name}, \sQuote{Description}, \sQuote{Author},
  \sQuote{Citation}, \sQuote{URL}.

  Vertex attributes: \sQuote{name}, \sQuote{type}.

  Edge attributes: NA
}
\source{
  http://vlado.fmf.uni-lj.si/pub/networks/data/ucinet/ucidata.htm
}
\references{
  Allison Davis, Burleigh Bradford Gardner, Mary R. Gardner, Deep south: a
  social anthropological study of caste and class, University of Chicago Press
  (1941).
}
\examples{
  data(davisclub)
}
\keyword{datasets}
\keyword{networks}
\name{football}
\alias{football}
\docType{data}
\title{
  American College Football Games Network
}
\description{
  A network of American football games between Division IA colleges during
  regular season Fall 2000, as compiled by M. Girvan and M. Newman. The vertices
  have "membership" and "color" attributes that indicate to which conferences
  they belong. The "membership" values are as follows:
  \enumerate{
    \item Atlantic Coast
    \item Big East
    \item Big Ten
    \item Big Twelve
    \item Conference USA
    \item Independents
    \item Mid-American
    \item Mountain West
    \item Pacific Ten
    \item Southeastern
    \item Sun Belt
    \item Western Athletic
  }
  The graph is not simple. Three sets of teams played each other twice during
  the season and therefore have two edges between them. They are as follows:
  \enumerate{
    \item Kansas State <-> Oklahoma
    \item Western Michigan <-> Marshall
    \item Auburn <-> Florida
  }
  Note: There is reason to believe that the conferences and games played were
  taken from different years and the membership values may not be accurate.
}
\usage{
  data("football")
}
\format{
  An undirected \code{\link{igraph}} graph object with 115 vertices and 616
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
  M. Girvan and M. E. J. Newman, Community structure in social and biological
  networks, Proc. Natl. Acad. Sci. USA 99, 7821-7826 (2002).
}
\keyword{datasets}
\keyword{networks}
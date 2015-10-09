\name{polbooks}
\alias{polbooks}
\docType{data}
\title{
  US Political Books Network
}
\description{
  A network of books about US politics published around the time of the 2004
  presidential election and sold by the online bookseller Amazon.com. Edges
  between books represent frequent copurchasing of books by the same buyers. The
  network was compiled by V. Krebs and is unpublished, but can found on Krebs'
  web site.
  
  Vertex "membership" and "color" attributes indicate political leaning
  according to:
  
  1 : "blue" (left or liberal)
  
  2 : "red" (right or conservative)
  
  3 : "gray" (neutral or moderate)
}
\usage{polbooks}
\format{
  An undirected \code{\link{igraph}} graph object with 105 vertices and 441
  edges.

  Graph attributes: \sQuote{name}, \sQuote{Author}, \sQuote{URL}.

  Vertex attributes: \sQuote{id}, \sQuote{name}, \sQuote{label},
  \sQuote{membership}, \sQuote{color}.

  Edge attributes: NA
}
\source{
  See reference below.
}
\references{
  http://www.orgnet.com/divided2.html
}
\keyword{datasets}
\keyword{networks}
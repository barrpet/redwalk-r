\name{polblogs}
\alias{polblogs}
\docType{data}
\title{
  US Political Blogs Network
}
\description{
  Political blogosphere Feb. 2005.
  Data compiled by Lada Adamic and Natalie Glance
  
  Vertex "membership" and "color" attributes indicate political leaning
  according to:
  
  1 : "blue" (left or liberal)
  
  2 : "red" (right or conservative)
  
  Data on political leaning comes from blog directories as indicated. Some blogs
  were labeled manually, based on incoming and outgoing links and posts around
  the time of the 2004 presidential election.  Directory-derived labels are
  prone to error; manual labels even more so.
  
  Links between blogs were automatically extracted from a crawl of the front
  page of the blog.
}
\section{Modifications}{
  This is a modified version of the original network.
  
  From the original graph, the largest connected component was selected,
  lowering the number of vertices by 268 from 1490 to 1222. All 268 vertices
  that were removed were isolated in the original graph.
  
  All edges, which were orignally directed, were made undirected.
  
  Finally, the graph was made simple by removing all multiedges and loops.
  This lowered the number of edges by 11 from 16725 to 16714.
}
\usage{polblogs}
\format{
  An undirected \code{\link{igraph}} graph object with 1222 vertices and 16714
  edges.

  Graph attributes: \sQuote{name}, \sQuote{Author}, \sQuote{Citation},
  \sQuote{URL}, \sQuote{Description}.

  Vertex attributes: \sQuote{name}, \sQuote{Source}, \sQuote{membership},
  \sQuote{color}, \sQuote{label}.

  Edge attributes: NA
}
\source{
  http://www-personal.umich.edu/~mejn/netdata/
}
\references{
  L. A. Adamic and N. Glance, The political blogosphere and the 2004 US
  Election, in Proceedings of the WWW-2005 Workshop on the Weblogging Ecosystem
  (2005).
}
\keyword{datasets}
\keyword{networks}
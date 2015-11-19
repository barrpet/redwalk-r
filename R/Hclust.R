## Hierarchical clustering, on raw input data; we will use Euclidean
## distance.  A range of criteria are supported; also there is a
## storage-economic option.
##
## We use the general routine, `hc', which caters for 7 criteria,
## using a half dissimilarity matrix; (BTW, this uses the very efficient
## nearest neighbor chain algorithm, which makes this algorithm of
## O(n^2) computational time, and differentiates it from the less
## efficient -- i.e. O(n^3) -- implementations in all commercial
## statistical packages -- as far as I am aware -- except Clustan.)
##
## Original author: F. Murtagh, May 1992
## R Modifications: Ross Ihaka, Dec 1996
##		    Friedrich Leisch, Apr 1998, Jun 2000

hclust_avg <- function(d, members = NULL)
{
  # Get number of vertices and check if valid
  nv <- as.integer(attr(d, "Size"));
  if(is.null(nv)) stop("invalid dissimilarities");
  if(is.na(nv) || nv > 65536L) stop("size cannot be NA nor exceed 65536");
  if(nv < 2) stop("must have at least two vertices to be able to cluster");

  # Check that d has proper number of elements
  len <- as.integer(nv*(nv-1)/2)
  if(length(d) != len) {
    if(length(d) < len) { stop; } else {
      warning("dissimilarities of improper length");
    }
  }

  # If not continuing from a cut, set all vertices to membership 1
  if(is.null(members))
    members <- rep(1, nv)
  else if(length(members) != nv)
    stop("invalid length of members");

  # FORTRAN
  storage.mode(d) <- "double";
  hcl <- .Fortran("hclustpb", PACKAGE = "redwalk",
    n = nv,
    len = len,
    ia = integer(nv),
    ib = integer(nv),
    crit = double(nv),
    members = as.double(members),
    nn = integer(nv),
    disnn = double(nv),
    flag = logical(nv),
    diss = d);

  ## 2nd step: interpret the information that we now have
  ## as merge, height, and order lists.
  hcass <- .Fortran("hcass2pb", PACKAGE = "redwalk",
    n = nv, # checked above.
    ia = hcl$ia,
    ib = hcl$ib,
    order = integer(nv),
    iia = integer(nv),
    iib = integer(nv));

  # Return an hclust object
  structure(list(merge = cbind(hcass$iia[1L:(nv-1)], hcass$iib[1L:(nv-1)]),
    height = hcl$crit[1L:(nv-1)],
    order = hcass$order,
    labels = attr(d, "Labels"),
    method = "average",
    call = match.call(),
    dist.method = attr(d, "method")),
    class = "hclust");
}

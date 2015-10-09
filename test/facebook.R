edgesfile <- "/home/peter/Downloads/facebook/107.edges";
el <- read.table(edgesfile, header = FALSE);
el <- matrix(as.character(as.matrix(el)), ncol = 2);
fb <- graph_from_edgelist(el, directed = FALSE);
fb <- simplify(fb, remove.multiple = TRUE, remove.loops = TRUE);

circlesfile <- "/home/peter/Downloads/facebook/107.circles";
nc<-max(count.fields(circlesfile, sep=""))
x<-read.table(circlesfile,sep="",col.names=paste("v",1:nc,sep="."),fill=T);
x <- matrix(unlist(x),ncol=nc)[,-1];
x <- split(x, row(x));
x <- lapply(x, function(x) { x[!is.na(x)]})
circleslist <- lapply(x, as.character);
numcircles <- length(circleslist);
circleslist <- circleslist[order(sapply(circleslist,length),decreasing=TRUE)];
memship <- rep(0, vcount(fb));
names(memship) <- V(fb)$name;
for(i in 1:min(length(circleslist), 7))
{
  memship[as.character(circleslist[[i]])] <- i;
}
memship <- memship[intersect(names(memship), V(fb)$name)];
V(fb)$membership <- memship;
clr <- memship + 1;
nclr <- length(unique(clr));
V(fb)$color <- clr;


fb_layout <- layout_with_fr(fb);
fb_no_edges <- delete_edges(fb, E(fb)[-1]);

tkplot(fb_no_edges, layout = fb_layout, vertex.size = log(degree(fb)) + 1, vertex.label = "");

featnamesfile <- "/home/peter/Downloads/facebook/107.featnames";
y <- read.table(featnamesfile, header = FALSE);
y <- y[,c(-1,-3)];
colnames(y) <- c("featname", "featid");
y[,1] <- sub("(.*?);anonymized", "\\1", y[,1]);
y[,1] <- gsub("(;)", ":", y[,1]);
y[,2] <- as.character(y[,2]);
featslong <- y;

featfile = "/home/peter/Downloads/facebook/107.feat";
z <- read.table(featfile, header=FALSE);
rownames(z) <- z[,1];
z <- z[,-1];
colnames(z) <- 1:length(featslong[,1]);

fnl <- rep(list(list()), length(levels(factor(featslong[["featname"]]))));
names(fnl) <- levels(factor(featslong[["featname"]]));
vertfeats <- rep(list(fnl), vcount(fb));
names(vertfeats) <- V(fb)$name;

for(fi in 1:10)#length(featslong[,1]))
{
  featnm <- featslong[,1][fi];
  featid <- featslong[,2][fi];
  for(vid in V(fb)$name)
  {
    if(z[vid,fi] == 1)
    {
      append(vertfeats[[vid]][[featnm]], featslong[,2][fi]);
    }
  }
}

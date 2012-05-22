options(echo=FALSE)
library(igraph)

input.file <- tail(commandArgs(trailingOnly=TRUE), 2)

cat(input.file)

## g <- read.graph(input.file, format="edgelist")

## pr <- page.rank(g, directed=TRUE, damping=0.85)

## i <- 0
## for (e in pr$vector) {
##   cat(i, e, sep=" = ", digits=20)
##   i <- i + 1
## }

## cat(s, pr$value, sep=" = ", digist=20)

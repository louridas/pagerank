library(igraph)
 
for (num.vertices in seq(10000, 100000, 10000)) {
  g <- erdos.renyi.game(num.vertices, 1 / num.vertices)
  write.graph(g,  sprintf("%s-%d.%s", "erdos", num.vertices, "txt"))
}

library(igraph)
 
for (num.vertices in seq(10000, 100000, 10000)) {
  g <- barabasi.game(num.vertices)
  write.graph(g,  sprintf("%s-%d.%s", "barabasi", num.vertices, "txt"))
}

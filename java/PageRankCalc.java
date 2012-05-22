import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;
import edu.uci.ics.jung.algorithms.scoring.PageRank;

import java.util.Set;
import java.util.TreeSet;

import edu.uci.ics.jung.graph.DirectedGraph;
import edu.uci.ics.jung.graph.DirectedSparseGraph;

public class PageRankCalc {

    DirectedGraph<Integer, String> g =
        new DirectedSparseGraph<Integer, String>();
    
    private void readFile(String filename, String delim) throws IOException {

        FileReader fr = new FileReader(filename);
        BufferedReader br = new BufferedReader(fr);

        String line;

        while ((line = br.readLine()) != null) {
            String[] result = line.split(delim);
            g.addEdge(result[0] + " " + result[1],
                Integer.parseInt(result[0]),
                Integer.parseInt(result[1]));
        }

        br.close();
    }
    
    public static void main(String args[]) throws IOException {
        
        PageRankCalc prc = new PageRankCalc();
        if (args.length != 2) {
            System.err.println("Usage: java PageRankCalc <delimiter> <input_file>");
            System.exit(1);
        }
        prc.readFile(args[1], args[0]);
        PageRank<Integer, String> pr =
            new PageRank<Integer, String>(prc.g, 0.15);
        pr.evaluate();
        double sum = 0;
        Set<Integer> sortedVerticesSet =
            new TreeSet<Integer>(prc.g.getVertices());
        for (Integer v : sortedVerticesSet) {
            double score = pr.getVertexScore(v);
            sum += score;
            System.out.println(v + " = " + score);
        }
        System.out.println("s = " + sum);
    }
}

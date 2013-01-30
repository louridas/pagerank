/* Copyright (c) 2010-2011, Panos Louridas, GRNET S.A.
 
   All rights reserved.
  
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
 
   * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 
   * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.
 
   * Neither the name of GRNET S.A, nor the names of its contributors
   may be used to endorse or promote products derived from this
   software without specific prior written permission.
  
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
   OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <vector>
#include <set>
#include <map>
#include <string>
#include <list>

using namespace std;

const double DEFAULT_ALPHA = 0.85;
const double DEFAULT_CONVERGENCE = 0.00001;
const unsigned long DEFAULT_MAX_ITERATIONS = 10000;
const bool DEFAULT_NUMERIC = false;
const string DEFAULT_DELIM = " => ";

/*
 * A PageRank calculator. It is responsible for reading data, performing
 * the algorithmic calculations, and outputing the results.
 *
 * The calculations are carried out in a straightforward
 * implementation of the algorithm description given in the American
 * Mathematical Society's Feature Column "How Google Finds Your Needle in
 * the Web's Haystack", by David Austing:
 * http://www.ams.org/samplings/feature-column/fcarc-pagerank
 */
class Table {
private:

    bool trace; // enabling tracing output
    double alpha; // the pagerank damping factor
    double convergence;
    unsigned long max_iterations;
    string delim;
    bool numeric; // input graph has numeric, zero-based indexed vertices
    vector<size_t> num_outgoing; // number of outgoing links per column
    vector< vector<size_t> > rows; // the rowns of the hyperlink matrix
    map<string, size_t> nodes_to_idx; // mapping from string node IDs to numeric
    map<size_t, string> idx_to_nodes; // mapping from numeric node IDs to string
    vector<double> pr; // the pagerank table

    /*
     * Trims leading and trailing \t and " " characters from str.
     */
    void trim(string &str);
    template <class Vector, class T> bool insert_into_vector(Vector& v,
                                                             const T& t);

    /*
     * Clears all internal data structures so that the table can be used
     * for new input and calculations.
     */
    void reset();
    
    /*
     * Adds a mapping from a node string ID (key) to a numeric one to the
     * internal mapping tables.
     *
     * Returns the mapped value of the node; if the node has already
     * been mapped, the already mapped index.
     */
    size_t insert_mapping(const string &key);

    /*
     * Adds an arc to the hyperlink matrix between from and to.
     */
    bool add_arc(size_t from, size_t to);
    
public:
    Table(double a = DEFAULT_ALPHA, double c = DEFAULT_CONVERGENCE,
          size_t i = DEFAULT_MAX_ITERATIONS, bool t = false,
          bool n = DEFAULT_NUMERIC,
          string d = DEFAULT_DELIM);
    
    /*
     * Reserves space for the internal tables used for the PageRank calculation.
     * It is not necessary to call the method; space will be reserved as needed;
     * however, if the size of the internal tables is known beforehand and is
     * used to initialise them, all space will be allocated at the method call
     * (instead of during calculations) resulting in faster operation.
     *
     * The size parameter passed refers to the number of rows of the link
     * matrix.
     */
    void reserve(size_t size);

    /*
     * Returns the number of rows of the link matrix.
     */
    const size_t get_num_rows();

    /*
     * Sets the number of rows of the link matrix.
     */
    void set_num_rows(size_t num_rows);

    const void error(const char *p,const char *p2 = "");

    /*
     * Reads the graph described in filename.
     */
    int read_file(const string &filename);

    /*
     * Calculates the pagerank of the hyperlink matrix.
     */
    void pagerank();

    /*
     * Returns the pagerank vector of the hyperlink matrix.
     */
    const vector<double>& get_pagerank();

    /*
     * Returns the name of the node with the given index. If the nodes are
     * numeric the name is the string representation of the number. If the
     * nodes are not numeric, the name is the original node name as it was
     * input from read_file(string&)
     */
    const string get_node_name(size_t index);

    const map<size_t, string>& get_mapping();
    
    /*
     * Returns the pagerank damping factor.
     */
    const double get_alpha();

    /*
     * Sets the pagerank damping factor.
     */
    void set_alpha(double a);

    /*
     * Returns the maximum number of iterations that the pagerank algorithm
     * will perform.
     */
    const unsigned long get_max_iterations();

    /*
     * Sets the maximum number of iterations that the pagerank algorithm
     * will perform.
     */
    void set_max_iterations(unsigned long i);

    /*
     * Returns the value that is used to determine convergence of the
     * pagerank calculation algorithm.
     */
    const double get_convergence();

    /*
     * Sets the value that is used to determine convergence of the
     * pagerank calculation algorithm.
     */
    void set_convergence(double c);

    /*
     * Returns true when tracing output is enabled, false otherwise.
     */
    const bool get_trace();

    /*
     * Sets tracing output.
     */
    void set_trace(bool t);

    /*
     * Returns true if the graph data to be read by read_file(sting) are in
     * numeric form (e.g., integer values starting from zero) or in string form.
     */
    const bool get_numeric();

    /*
     * Specifies whether the graph data to be read by read_file(sting)
     * are in numeric form (e.g., integer values starting from zero)
     * or in string form.
     */
    void set_numeric(bool n);

    /*
     * Returns the delimeter used in the graph data file. The data
     * file is composed of lines with the following format:
     * <from><delim><to>
     * where from and to are the two graph vertices (can be either strings or
     * integers) and delim is the delimiter.
     */
    const string get_delim();

    /*
     * Sets the delimited to be used for reading the graph data file.
     */
    void set_delim(string d);

    /*
     * Outputs the parameters of the pagerank algorithm to the
     * given output stream. The parameters are:
     * - the damping factor (alpha)
     * - the convergence criterion (convergence)
     * - the maximum number of iterations (max iterations)
     * - whether numeric or string input is expected (numeric)
     * - the delimiter for separating the two vertices in each line of the
     *   input file (delim)
     */
    const void print_params(ostream &out);

    /*
     * Outputs the hyperlink table.
     */
    const void print_table();

    /*
     * Outputs the number of outgoing links for each vertex of the
     * hyperlink table.
     */
    const void print_outgoing();

    /*
     * Prints the pagerank vector to cout. The output format is a
     * series of lines:
     * <node> = <pagerank value> followed by a line:
     * s = <sum> where <sum> is the sum of the pagerank values, which
     * should be equal to one.
     */
    const void print_pagerank();

    /*
     * Outputs the pageranks vector in a more verbose way than print_pagerank():
     * it substitutes string vertex names for numeric IDs, if available,
     * and also outputs the index number of each vector, starting from zero.
     */
    const void print_pagerank_v();
};

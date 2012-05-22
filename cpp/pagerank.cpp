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

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>

using namespace std;

#include "table.h"

const char *TRACE_ARG = "-t";
const char *NUMERIC_ARG = "-n";
const char *ALPHA_ARG = "-a";
const char *CONVERGENCE_ARG = "-c";
const char *SIZE_ARG = "-s";
const char *DELIM_ARG = "-d";
const char *ITER_ARG = "-m";

void usage() {
    cerr << "pagerank [-tn] [-a alpha ] [-s size] [-d delim] "
         << "[-m max_iterations] <graph_file>" << endl
         << " -t enable tracing " << endl
         << " -n treat graph file as numeric; i.e. input comprises "
         << "integer vertex names" << endl
         << " -a alpha" << endl
         << "    the dumping factor " << endl
         << " -c convergence" << endl
         << "    the convergence criterion " << endl
         << " -s size" << endl
         << "    hint for internal tables " << endl
         << " -d delim" << endl
         << "    delimiter for separating vertex names in each input"
         << "line " << endl
         << " -m max_iterations" << endl
         << "    maximum number of iterations to perform" << endl;
}

int check_inc(int i, int max) {
    if (i == max) {
        usage();
        exit(1);
    }
    return i + 1;
}

int main(int argc, char **argv) {

    Table t;
    char *endptr;
    string input = "stdin";

    int i = 1;
    while (i < argc) {
        if (!strcmp(argv[i], TRACE_ARG)) {
            t.set_trace(true);
        } else if (!strcmp(argv[i], NUMERIC_ARG)) {
            t.set_numeric(true);
        } else if (!strcmp(argv[i], ALPHA_ARG)) {
            i = check_inc(i, argc);
            double alpha = strtod(argv[i], &endptr);
            if ((alpha == 0 || alpha > 1) && endptr) {
                cerr << "Invalid alpha argument" << endl;
                exit(1);
            }
            t.set_alpha(alpha);
        } else if (!strcmp(argv[i], CONVERGENCE_ARG)) {
            i = check_inc(i, argc);
            double convergence = strtod(argv[i], &endptr);
            if (convergence == 0 && endptr) {
                cerr << "Invalid convergence argument" << endl;
                exit(1);
            }
            t.set_convergence(convergence);
        } else if (!strcmp(argv[i], SIZE_ARG)) {
            i = check_inc(i, argc);
            size_t size = strtol(argv[i], &endptr, 10);
            if (size == 0 && endptr) {
                cerr << "Invalid size argument" << endl;
                exit(1);
            }
            t.set_num_rows(size);
        } else if (!strcmp(argv[i], ITER_ARG)) {
            i = check_inc(i, argc);
            size_t iterations = strtol(argv[i], &endptr, 10);
            if (iterations == 0 && endptr) {
                cerr << "Invalid iterations argument" << endl;
                exit(1);
            }
            t.set_max_iterations(iterations);
        } else if (!strcmp(argv[i], DELIM_ARG)) {
            i = check_inc(i, argc);
            t.set_delim(argv[i]);
        } else if (i == argc-1) {
            input = argv[i];
        } else {
            usage();
            exit(1);
        }
        i++;
    }

    t.print_params(cerr);
    cerr << "Reading input from " << input << "..." << endl;
    if (!strcmp(input.c_str(), "stdin")) {
            t.read_file("");
    } else {
        t.read_file(input);
    }
    cerr << "Calculating pagerank..." << endl;
    t.pagerank();
    cerr << "Done calculating!" << endl;
    t.print_pagerank_v();
}

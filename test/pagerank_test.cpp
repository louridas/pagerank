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

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <dirent.h>

#include "table.h"

using namespace std;

const double EPSILON = 0.000001;

void error(const char *p,const char *p2) {
    cerr << p <<  ' ' << p2 <<  '\n';
    exit(1);
}

void error(const string p,const string p2) {
    cerr << p <<  ' ' << p2 <<  '\n';
    exit(1);
}

void usage() {
    cerr << "Usage: pagerank_test [-jp] <test_suite>" << endl
         << " -j use Java test results" << endl
         << " -p use Python test results (default)" << endl;
}

int main(int argc, char *argv[]) {

    Table t;
    bool java_test = false;
    bool python_test = true;

    if (argc == 3) {
        if (!strcmp(argv[1], "-j")) {
            java_test = true;
            python_test = false;
        } else if (!strcmp(argv[1], "-p")) {
            java_test = false;
            python_test = true;
        } else {
            usage();
            exit(1);
        } 
    } else if (argc < 2) {
        usage();
        exit(1);
    }
    
    string tests_filename = argv[argc - 1];

    ifstream tests_file(tests_filename.c_str());

    if (!tests_file.is_open()) {
        error("Cannot open file", tests_filename);
    }

    cout.precision(numeric_limits<double>::digits10);
    
    while (!tests_file.eof()) {
        string test_line;
        getline(tests_file, test_line);
        if (test_line.empty()) {
            continue;
        }
        /* Get graph file and pagerank file names */
        string graph_filename = test_line + ".txt";
        string pagerank_filename;
        if (java_test) {
            pagerank_filename = test_line + "-pr-j.txt";
        }
        if (python_test) {
            pagerank_filename = test_line + "-pr-p.txt";
        }
        /* Open pagerank file */
        ifstream pagerank_file(pagerank_filename.c_str());
        if (!pagerank_file.is_open()) {
            cerr << endl;
            error("Cannot open pagerank file", pagerank_filename);
        }
        cout << "testing against " << pagerank_filename << "... ";
        /* Read graph file */
        t.set_numeric(true);
        t.set_delim(" ");
        t.set_trace(false);
        t.read_file(graph_filename);
        /* Calculate pagerank */
        t.pagerank();

        /* Read pagerank test results file */
        vector<double> pagerank_test_values;
        while (!pagerank_file.eof()) {
            string pagerank_line;
            getline(pagerank_file, pagerank_line);
            if (pagerank_line.find("s = ") != string::npos) {
                break;
            }
            string::size_type sep = pagerank_line.find(" = ");
	    if (sep == string::npos) {
		    break;
	    }
            string index_str = pagerank_line.substr(0, sep);
            size_t index = strtol(index_str.c_str(), NULL, 10);
            string value_str = pagerank_line.substr(sep + 3);
            double value;
            value = strtod(value_str.c_str(), NULL);
            if (index >= pagerank_test_values.size()) {
                pagerank_test_values.resize(index + 1);
            }
            pagerank_test_values[index] = value;
        }
        pagerank_file.close();

        /* Compare test results with calculated results */
        vector<double> pagerank_results = t.get_pagerank();

        bool test_ok = true;
        for (unsigned int i = 0; i < pagerank_results.size(); i++) {
            double result = pagerank_results[i];
            string name = t.get_node_name(i);
            size_t test_index = strtol(name.c_str(), NULL, 10);
            double test_result = pagerank_test_values[test_index];
            double diff = abs(result - test_result);
            if (diff > EPSILON) {
                cout << " error in calculation for " << name << ": "
                     << "result=" << result << " "
                     << "expected=" << test_result << " "
                     << "diff=" << diff;
                test_ok = false;
                break;
            }
        }
        if (test_ok) {
            cout << " OK" << endl;
        } else {
            cout << " Failed" << endl;
        }
    }
    
}

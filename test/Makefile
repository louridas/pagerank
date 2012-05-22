INC = ../cpp
VPATH = $(INC) 

pagerank_test: pagerank_test.cpp table.cpp table.h
	g++ -Wall -o pagerank_test -I$(INC) pagerank_test.cpp $(INC)/table.cpp

run-tests-p: pagerank_test
	./pagerank_test -p all-tests.txt

run-tests-j: pagerank_test
	./pagerank_test -j all-tests.txt

run-tests: run-tests-p run-tests-j

create-tests-p:
	./pagerank_calc.sh -p all-tests.txt

create-tests-j:
	./pagerank_calc.sh -j all-tests.txt

create-tests: create-tests-p create-tests-j
	./pagerank_calc.sh all-tests.txt

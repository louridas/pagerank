#!/bin/bash

function usage {
    echo "pagerank_calc.sh [-jp] <test_suite>"
    echo " -j create java test files only"
    echo " -p create python test files only"
    echo "Default is to create all test files"
    exit
}    

if [ $# -ne 1 -a $# -ne 2 ]
then
    usage
    exit
fi    

if [ $# -eq 2 ]
then
    if [ $1 == "-j" ]
    then
	do_java=1
    elif [ $1 == "-p" ]
    then
	do_python=1
    else
	usage
	exit
    fi
else
    do_java=1
    do_pytnon=1
fi

shift

while read graph_name 
do
    infilename="${graph_name}.txt"
    if [ $do_java ]
    then
	echo "java processing" $infilename
	outfilename="${graph_name}-pr-j.txt"
	java -classpath "../java:../java/lib/*" PageRankCalc " " $infilename > $outfilename
    fi
    if [ $do_python ]
    then 
	echo "python processing" $infilename
	outfilename="${graph_name}-pr-p.txt"
	python ../python/pagerank_test.py $infilename > $outfilename
    fi
done < $1
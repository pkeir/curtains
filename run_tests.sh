#!/bin/bash

# Tested with GCC 7.3.0, 7.2.0
 
g++ --version

g++ -std=c++1z                     tests.hpp # default is -DCURTAINS_V
res1=$?
if [ $res1 != 0 ]
then
echo "error: g++ -std=c++1z tests.hpp"
fi

g++ -std=c++1z -DCURTAINS_V_SIMPLE tests.hpp
res2=$?
if [ $res2 != 0 ]
then
echo "error: g++ -std=c++1z -DCURTAINS_V_SIMPLE tests.hpp"
fi

g++ -std=c++1z -DCURTAINS_N        tests.hpp
res3=$?
if [ $res3 != 0 ]
then
echo "error: g++ -std=c++1z -DCURTAINS_N tests.hpp"
fi

exit $((res1 || res2 || res3))

#!/bin/bash

# Tested with:
# GCC 9.0.0, 8.1.1, 7.3.0, 7.2.0
# Clang 6.0.0, 4.0.1-6
 
function test()
{
  local CC=$2
  $CC --version

  $CC -std=c++1z                     tests.hpp # default is -DCURTAINS_V

  local res1=$?
  if [ $res1 != 0 ]
  then
  echo "error: $CC -std=c++1z tests.hpp"
  fi

  $CC -std=c++1z -DCURTAINS_V_SIMPLE tests.hpp
  local res2=$?
  if [ $res2 != 0 ]
  then
  echo "error: $CC -std=c++1z -DCURTAINS_V_SIMPLE tests.hpp"
  fi

  $CC -std=c++1z -DCURTAINS_N        tests.hpp
  local res3=$?
  if [ $res3 != 0 ]
  then
  echo "error: $CC -std=c++1z -DCURTAINS_N tests.hpp"
  fi

  eval "$1=$((res1 || res2 || res3))"
}

test ret1 g++
test ret2 clang++

exit $((ret1 || ret2))

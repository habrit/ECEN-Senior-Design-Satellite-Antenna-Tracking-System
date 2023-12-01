#!/bin/sh

for file in "$(dirname $0)"/*.cpp ; do
    basename="$(basename $file)"
    if [ "$basename" != "Functions.cpp" -a "$basename" != "GPIOfunctions.cpp" -a "$basename" != "TLEpredctFunctions.cpp" ]; then
        g++ -pthread -o "$(echo $basename | head -c -5)" $file
    fi
done

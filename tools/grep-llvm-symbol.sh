#!/bin/bash
# Rask
#
# Copyright (c) 2010 Rafal Przywarski
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
if [ $# -ne 1 ]
then
    echo "usage: grep-llvm-symbol.sh <symbol>"
    exit
fi
LIBS=`llvm-config --libnames all`
TARGETS=`llvm-config --components`
LIBPATH=`llvm-config --libdir`
for LIB in $LIBS
do
    FULLNAME="$LIBPATH/$LIB"
    SYMBOLS=`nm "$FULLNAME" | grep $1 | grep ' T '`
    if [ "$SYMBOLS" != "" ]
    then
        for TARGET in $TARGETS
        do
            G=`llvm-config --libnames "$TARGET" | grep -w "$LIB"`
            if [ "$G" != "" ]
            then
                echo "$TARGET"
            fi
        done
        exit
    fi
done

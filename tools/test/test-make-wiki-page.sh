#!/bin/bash

OUTPUT=""

testPage()
{
    local EXPECTECED=`cat $3`
    local ACTUAL=`../make-wiki-page.sh $2 $1 $4 > actual.tmp`
    local DIFF=`diff $3 actual.tmp`
    if [ "$DIFF" == "" ]
    then
        echo -n "."
    else
        echo -n "F"
        OUTPUT="${OUTPUT}$1:\n${DIFF}\n\n"
    fi
    rm actual.tmp
}

echo -n "running: "
testPage Simple1 list simple1.wiki SimpleTest1
testPage Simple2 list simple2.wiki SimpleTest2
testPage Simple3 list simple3.wiki SimpleTest3

echo -e "\n\n$OUTPUT"

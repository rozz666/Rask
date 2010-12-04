#!/bin/bash
# Rask
#
# Copyright (c) 2010 Rafal Przywarski
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
CURRENT_DIR=`pwd`
RASKC="$CURRENT_DIR/../bin/Release/raskc"
RASK="$CURRENT_DIR/../bin/Release/rask"
SUITE_LIST=`ls -d test*`
OK_COUNT=0
FAILED_COUNT=0
ERROR_COUNT=0

OLD_IFS="$IFS"

for SUITE in $SUITE_LIST
do
    SUITE_DIR="$CURRENT_DIR/$SUITE"
    cd "$SUITE_DIR"

    ERROR_LOG=""
    
    if [ ! -e "list" ]
    then
        echo "$SUITE: Missing $SUITE/list"
        continue;
    fi

    LIST=`cat list`

    if [ "$LIST" == "" ]
    then
        continue;
    fi

    echo -n "$SUITE:"

    IFS=$'\n'

    for TEST in $LIST
    do
        IFS="$OLD_IFS"

        TEST="${TEST%%#*}"

        if [ "$TEST" == "" ]
        then
            continue;
        fi

        SOURCE="${TEST##*:}"
        TEST_NAME="${TEST%%:*}"
        EXPECTED_OUTPUT_PATH="${TEST_NAME}.out"
        EXPECTED_INPUT_PATH="${TEST_NAME}.in"

        if [ ! -e "$EXPECTED_OUTPUT_PATH" ]
        then
            ERROR_LOG="${ERROR_LOG}ERROR: Missing $SUITE/$EXPECTED_OUTPUT_PATH\n"
            ((ERROR_COUNT+=1))
            continue;
        fi

        if [ ! -e "$EXPECTED_INPUT_PATH" ]
        then
            EXPECTED_INPUT_PATH="/dev/null"
        fi

        EXPECTED_OUTPUT=`cat $EXPECTED_OUTPUT_PATH`
        TEST_TYPE=${TEST%:*}
        TEST_TYPE=${TEST_TYPE#*:}
        case "$TEST_TYPE" in
            "error") 
                OUTPUT=`$RASKC $SOURCE -n 2>&1 1>/dev/null`;;
            "test")
                COUTPUT=`$RASKC $SOURCE -o prog.out 2>&1 1>/dev/null`;

                if [ "$COUTPUT" == "" ]
                then
                    OUTPUT=`$RASK prog.out < $EXPECTED_INPUT_PATH 2>/dev/null`;
                    rm -f prog.out;
                else
                    OUTPUT="$COUTPUT";
                fi;;

            *)
                ((ERROR_COUNT+=1));
                echo -n "E";
                continue;;
        esac
	
        OUTPUT=${OUTPUT//$'\r'/""};

        if [ "$OUTPUT" == "$EXPECTED_OUTPUT" ]
        then
            ((OK_COUNT+=1))
            echo -n "."
        else
            ERROR_LOG="${ERROR_LOG}FAILED $TEST\nexpected:\n$EXPECTED_OUTPUT\ngot:\n$OUTPUT\n"
            ((FAILED_COUNT+=1))
            echo -n "F"
        fi
    done
    echo
    echo -ne "$ERROR_LOG"
done
if [ "$OK_COUNT" -eq 0 ]
then
    OK_MSG=""
else
    OK_MSG=" ok:$OK_COUNT"
fi
if [ "$FAILED_COUNT" -eq 0 ]
then
    FAILED_MSG=""
else
    FAILED_MSG=" failed:$FAILED_COUNT"
fi
if [ "$ERROR_COUNT" -eq 0 ]
then
    ERROR_MSG=""
else
    ERROR_MSG=" errors:$ERROR_COUNT"
fi
echo
echo "Summary:$OK_MSG$FAILED_MSG$ERROR_MSG"


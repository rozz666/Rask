#!/bin/bash
CURRENT_DIR=`pwd`
RASKC="$CURRENT_DIR/../bin/release/raskc"
SUITE_LIST=`ls -d test*`
OK_COUNT=0
FAILED_COUNT=0

for SUITE in $SUITE_LIST
do
    echo "Running $SUITE..."
    SUITE_DIR="$CURRENT_DIR/$SUITE"
    cd "$SUITE_DIR"
    LIST=`cat list`
    for TEST in $LIST
    do
        SOURCE="${TEST%:*}"
        EXPECTED_OUTPUT=`cat ${TEST#*:}`
        OUTPUT=`$RASKC $SOURCE 2>&1 1>/dev/null`
        OUTPUT=${OUTPUT//$'\r'/""}
        if [ "$OUTPUT" == "$EXPECTED_OUTPUT" ]
        then
            ((OK_COUNT+=1))
        else
            echo "FAILED $TEST"
            echo "expected:"
            echo "$EXPECTED_OUTPUT"
            echo "got:"
            echo "$OUTPUT"
            ((FAILED_COUNT+=1))
        fi
    done
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
echo
echo "Summary:$OK_MSG$FAILED_MSG"


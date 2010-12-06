#!/bin/bash
# Rask
#
# Copyright (c) 2010 Rafal Przywarski
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#

if [[ $# != 3 ]]
then
    echo "usage: make-wiki-page.sh list feature_name title"
    exit
fi

OLD_IFS="$IFS"

if [ ! -e "$1" ]
then
    echo "make-wiki-page.sh: Missing $1"
    exit;
fi

echo -e "++ $3"
echo

LIST=`grep "#$2" $1`

IFS=$'\n'

prettyCode()
{
    local CODE=`cat $1`
    CODE=${CODE//" "/"&#xA0;"}
    CODE=${CODE//$'\n'/$'<br/>\n'}
    CODE=${CODE//"var"/"<span style=\"color:#000080; font-weight:bold\">var</span>"}
    CODE=${CODE//"void"/"<span style=\"color:#000080; font-weight:bold\">void</span>"}
    CODE=${CODE//"int32"/"<span style=\"color:#000080; font-weight:bold\">int32</span>"}
    echo "[[table style=\"width: 80%;\"]][[row]][[cell style=\"border: 1px solid silver; padding: 10px; background-color: #f7f7ff\"]]"
    echo "[[html]]"
    echo "<code>"
    echo -n "<span style=\"font-size: 10pt; color:#000000;\">"
    echo -n "$CODE"
    echo "<br/></span></code>"
    echo "[[/html]]"
    echo "[[/cell]][[/row]][[/table]]"
}

prettyBlock()
{
    local TEXT=`cat $1`
    echo "[[table style=\"width: 80%;\"]][[row]][[cell style=\"border: 1px solid silver; padding: 10px; background-color: #f7f7ff\"]]"
    echo "[[html]]"
    echo "<code>"
    echo "${TEXT//$'\n'/$'<br/>\n'}<br/>"
    echo "</code>"
    echo "[[/html]]"
    echo "[[/cell]][[/row]][[/table]]"
}

LAST_SOURCE=""

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
    TEST_TYPE=${TEST%:*}
    TEST_TYPE=${TEST_TYPE#*:}

    case "$TEST_TYPE" in
        "error")
            echo -e "The following code:"
            for S in $SOURCE
            do
                echo "//$S//"
                echo
                prettyCode $S
                echo
            done
            echo "should cause:"
            echo
            prettyBlock $EXPECTED_OUTPUT_PATH
            echo
            ;;
        "test")
            if [ "$SOURCE" != "$LAST_SOURCE" ]
            then
                echo -e "The compiler should generate code for:"
                for S in $SOURCE
                do
                    echo "//$S//"
                    echo
                    prettyCode $S
                    echo
                done
            fi
            echo "The program should output:"
            echo
            prettyBlock $EXPECTED_OUTPUT_PATH
            echo
            if [ -e "$EXPECTED_INPUT_PATH" ]
            then
                echo "with input:"
                echo
                prettyBlock $EXPECTED_INPUT_PATH
                echo
            fi
            ;;
    esac

    LAST_SOURCE=$SOURCE

done


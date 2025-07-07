#!/bin/sh

# Header field
echo '"ID","STD ABBR","STD NAME","DST ABBR","DST NAME","GMT offset","DST adjustment","DST Start Date rule","Start time","DST End date rule","End time"'

# We ignore POSIX and leap-second files, Boost does not use them.

for tz in $(find /usr/share/zoneinfo \
    -path /usr/share/zoneinfo/right -prune -o \
    -path /usr/share/zoneinfo/posix -prune -o \
    -type f \
    | grep -v "\.tab" \
    | grep -v posix \
    | grep -v right \
    | grep -v leapseconds \
    | grep -v tzdata \
    | sed -e 's|/usr/share/zoneinfo/||g' \
    | sort);
do
    perl tzinfo.pl $tz;
done

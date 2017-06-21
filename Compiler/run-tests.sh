#!/bin/bash

die()
{
	echo "$@"
	exit 1
}

for f in testcase/test_*.go; do
	echo "[*] Running file $f"
	yes 9487 | ./go $f || die "Error in file $f"
done

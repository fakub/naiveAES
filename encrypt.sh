#!/bin/bash

die () {
	echo >&2 "$@"
	exit 1
}

[ "$#" -eq 1 ] || die "One hex-formatted argument of 32 characters required, $# provided"
echo $1 | grep -E -q '^[0-9a-fA-F]{32}$' || die "Hex-formatted argument of 32 characters required, $1 provided"

./main $1
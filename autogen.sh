#!/bin/sh

set -e

: > kprintfm/start.d
: > kprintfm/kprintf.d

find . -name '*.cpp' | while read SOURCE; do
	: > "${SOURCE%.*}.d"
done

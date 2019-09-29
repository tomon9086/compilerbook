#!/bin/bash
try() {
	input="$1"
	expected="$2"

	./9cc "$input" > tmp.s
	gcc -o tmp tmp.s
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$input => $expected expected, but got $actual"
		exit 1
	fi
}

echo '- one integer'
try 0 0
try 42 42

echo '- add and subtract calculation'
try '2+3' 5
try '13-7' 6
try '5+20-4' 21

echo OK

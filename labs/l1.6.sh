#!/bin/bash

cat a.txt | tr "[:upper:]" "[:lower:]" | tr "[:space:][:punct:]" "\n" | sort | uniq -c | sort -nr

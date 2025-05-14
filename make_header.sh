#!/usr/bin/env bash
for f in srcs/*.c; do
echo '/* '$f' */' && grep -E '^\w+' $f|sed 's/$/;/'
done
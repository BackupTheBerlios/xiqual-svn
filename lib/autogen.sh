#!/bin/sh

if [ -e configure ]; then
 make distclean
fi

aclocal
automake --foreign --include-deps --add-missing --copy
autoconf
./configure

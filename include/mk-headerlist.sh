#!/bin/sh
echo '#Automatically generated list of header files by mk-headerlist.sh, v0' > headerlist
echo 'HEADER_LIST = \' >> headerlist
find mosh/cgi -type f \( -name '*.hpp' -o -name '*.tcc' \) -a \! -path '*/_/*' | sed '$!s/$/ \\/' >> headerlist
echo >> headerlist


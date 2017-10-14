#!/bin/bash

if [ "$LD_LIBRARY_PATH" = "" ]; then
    export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
fi

echo -e '\033[1;33m'
echo "'Testing get_product_names'" 

tput sgr0
echo ""

# tput setaf 2 && echo -en 'Name: ' && tput sgr0 && echo ""

#./get_item_titles shopjimmy_example.html
./get_product_names shopjimmy_example.html


./driver

exit 0

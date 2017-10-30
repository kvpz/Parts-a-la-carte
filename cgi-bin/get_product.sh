#!/bin/bash

# get_product.sh
# Get products from online stores and parse the responses.
#
#
export LD_LIBRARY_PATH=/usr/local/lib

echo "Content-type: text/html"
echo ""
echo "<html>"
echo "<body>"
echo "Your query: $QUERY_STRING<br/>"
tvpart=`echo "$QUERY_STRING" | sed -n 's/^.*query=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
#tvpart="056.04167.0001"

echo "<p>"
if [ $tvpart != "" ]; then
    request_file="$(./request_shopjimmy $tvpart)"
else
    echo "<h3>You need to provide a query as an input!</h3>"
    exit 0;
fi

echo "</p>"

echo "<p>"
echo "get_product_names $request_file"
./get_product_names "$request_file"

echo "</p>"

echo "</body>"
echo "</html>"

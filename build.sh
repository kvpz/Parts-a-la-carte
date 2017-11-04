
MYHTML_HEADERS=/usr/local/include/myhtml
LIB=/usr/local/lib
LIBHTML=/usr/local/lib/myhtml_static

echo 'gcc -g -I/usr/local/include/myhtml -L/usr/local/lib get_product_names.c -o get_product_names -lmyhtml'
gcc -g -I$MYHTML_HEADERS -L$LIB get_product_names.c -o get_product_names -lmyhtml 
#gcc -g -static -L/usr/lib64 -L$LIB -lc -lmyhtml_static -I$MYHTML_HEADERS get_product_names.c -o get_product_names 

#echo 'gcc -g -I$MYHTML_HEADERS -L$LIB driver.c -o driver -lmyhtml -lcurl'
#gcc -g -I$MYHTML_HEADERS -L$LIB api.c driver.c -o driver -lmyhtml -lcurl
#gcc -g $LIBHTML -I$MYHTML_HEADERS api.c driver.c -o driver -lcurl

echo 'gcc -g -I$MYHTML_HEADERS -L$LIB request_shopjimmy.c -o request_shopjimmy -lmyhtml -lcurl'
gcc -g -I$MYHTML_HEADERS -L$LIB request_shopjimmy.c -o request_shopjimmy -lmyhtml -lcurl

echo "gcc -g -I$MYHTML_HEADERS -L$LIB request_encompass.c -o request_encompass -lmyhtml -lcurl"
gcc -g -I$MYHTML_HEADERS -L$LIB request_encompass.c -o request_encompass -lmyhtml -lcurl

echo "gcc -g -I$MYHTML_HEADERS -L$LIB get_product_names_enc.c -o get_product_names_enc -lmyhtml"
gcc -g -I$MYHTML_HEADERS -L$LIB get_product_names_enc.c -o get_product_names_enc -lmyhtml

# If executable emits an error related to shared libraries, make sure
# the environment variable $LD_LIBRARY_PATH is set correctly.
# The $LD_LIBRARY_PATH variable should be set to the location of the
# libraries: export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
# in order to make it available at runtime.

# RESOURCE
# https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

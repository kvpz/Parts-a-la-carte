
MYHTML_HEADERS=/usr/local/include/myhtml
LIB=/usr/local/lib

echo 'gcc -g -I/usr/local/include/myhtml -L/usr/local/lib get_product_names.c -o get_product_names -lmyhtml'
gcc -g -I$MYHTML_HEADERS -L$LIB get_product_names.c -o get_product_names -lmyhtml 

echo 'gcc -g -I$MYHTML_HEADERS -L$LIB driver.c -o driver -lmyhtml -lcurl'
gcc -g -I$MYHTML_HEADERS -L$LIB api.c driver.c -o driver -lmyhtml -lcurl

# If executable emits an error related to shared libraries, make sure
# the environment variable $LD_LIBRARY_PATH is set correctly.
# The $LD_LIBRARY_PATH variable should be set to the location of the
# shared libraries: export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH.


# RESOURCE
# https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html

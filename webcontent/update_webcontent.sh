#!/bin/bash

# update_webcontent.sh
#
# all the files from /var/www/html/partsalacarte or anything related to the
# interface for Partsalacarte should be placed within this folder. The main
# content should go in the subdirectory: /webcontent/partsalacarte.
# 

cp -r /var/www/html/partsalacarte/ .


# This should probably check for differences before copying the content from
# /var/www/html/partsalacarte, but as long as good practices are followed,
# there should be no issues. The best practice is to develop at /var/www/html/
# and copy the content over here before a git commit. Reason: it would be a
# pain to be copying content from here to the server's html bin for every test.

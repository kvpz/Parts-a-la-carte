/*
 Copyright (C) 2015-2016 Alexander Borisov
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 
 Author: lex.borisov@gmail.com (Alexander Borisov)

 Modifications by Kevin Perez:

 This code will be used to extract products from a ShopJimmy webpage containing
 search results. The products will be output to stdout or some other file
 descriptor.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <myhtml/api.h>

#include "example.h"

struct res_html {
    char  *html;
    size_t size;
};

char* get_title(myhtml_tree_t* tree)
{
    myhtml_collection_t *titleCollection =
      myhtml_get_nodes_by_tag_id(tree, NULL, MyHTML_TAG_TITLE, NULL);
    
    if(titleCollection && titleCollection->list && titleCollection->length) {
      printf("titleCollection:\n");
      printf("length: %d\n", titleCollection->length);
      
      myhtml_tree_node_t *text_node =
	  myhtml_node_child(titleCollection->list[0]);
      
        if(text_node) {
	  //printf("text_node is valid\n");
            const char* text = myhtml_node_text(text_node, NULL);

            if(text){
	      printf("text: %s", text);
	      myhtml_collection_destroy(titleCollection);
	      return text;
	    }
	      //fwrite(text, 1, strlen(text), outptr);
	    
        }
    }
    myhtml_collection_destroy(titleCollection);
}

struct res_html load_html_file(const char* filename)
{
    FILE *fh = fopen(filename, "rb");
    if(fh == NULL) {
        fprintf(stderr, "Can't open html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    if(fseek(fh, 0L, SEEK_END) != 0) {
        fprintf(stderr, "Can't set position (fseek) in file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    long size = ftell(fh);
    
    if(fseek(fh, 0L, SEEK_SET) != 0) {
        fprintf(stderr, "Can't set position (fseek) in file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    if(size <= 0) {
        fprintf(stderr,
		"Can't get file size or file is empty: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    char *html = (char*)malloc(size + 1);
    if(html == NULL) {
        fprintf(stderr, "Can't allocate mem for html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    size_t nread = fread(html, 1, size, fh);
    if (nread != size) {
        fprintf(stderr,
		"could not read %ld bytes (" MyCORE_FMT_Z " bytes done)\n",
		size, nread);
        exit(EXIT_FAILURE);
    }

    fclose(fh);

    struct res_html res = {html, (size_t)size};
    return res;
}

struct product {
  const char* price;
  const char* title;
} typedef product;

const char* product_price(myhtml_tree_t *tree, myhtml_tree_node_t *node, int i)
{
  const char* price;
  myhtml_collection_t *productPrice =
    myhtml_get_nodes_by_attribute_value(tree, NULL,NULL,false, NULL,0,
				    "price", 5, NULL);
  if(productPrice){
    myhtml_tree_node_t *priceNode =
      myhtml_node_child(productPrice->list[i]);
    price = myhtml_node_text(priceNode, NULL);
  }

  return price;
}

product* get_products(myhtml_tree_t* tree, FILE** outfile)
{
  product* _products;
  // get nodes by the attribute value "product-name"
  myhtml_collection_t *products =
    myhtml_get_nodes_by_attribute_value(tree,
					NULL,
					NULL,
					false,
					NULL,
					0,
					"product-name",
					12,
					NULL);

  if(!products && !products->list && !products->length)
    return NULL;
 
  if(products){
    _products = malloc(sizeof(product) * products->length);
    fprintf(*outfile, "\t(total: %d): \n", products->length);
    
    for(int i = 0; i < products->length; ++i){
      myhtml_tree_node_t *p1 =
	myhtml_node_child(products->list[i]);

      myhtml_collection_t * productTitle =
	myhtml_get_nodes_by_attribute_key(tree, NULL, p1,
					  "title", 5, NULL);
      if(productTitle){
	fprintf(*outfile, "<br/>%d) ", i);
	myhtml_tree_node_t *titleNode =
	  myhtml_node_child(productTitle->list[0]);
	_products[i].title = myhtml_node_text(titleNode, NULL);
	_products[i].price = product_price(tree, p1, i);
	  
	fprintf(*outfile, "%s", _products[i].title);
	fprintf(*outfile, " %s", _products[i].price);
	fprintf(*outfile, "\n\n");
      }
    }
    
    myhtml_collection_destroy(products);
    return _products;
  }

  return NULL;
}

int main(int argc, const char * argv[])
{
    const char* path;
    const char* outfile_name; 
    FILE* outptr;
    
    if (argc == 2) {
        path = argv[1];
	outptr = stdout;
    }
    else if(argc == 3){
      path = argv[1];
      outfile_name = argv[2];
      outptr = fopen(outfile_name, "w");
    }
    else {
      printf("Void execution!\n");
      printf("Use: get_product_names <path_to_html_file> <output file>\n");
      exit(EXIT_FAILURE);
    }
    
    struct res_html res = load_html_file(path);
    
    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    
    // init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    
    // parse html
    myhtml_parse(tree, MyENCODING_UTF_8, res.html, res.size);
    
    // parse html for title elements
    char* title = get_title(tree);;
    printf("The title returned is: %s\n", title);
    // Get the products
    product* products = get_products(tree, &outptr);
    
    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
    
    free(res.html);
    fclose(outptr);
    free(products);
    
    return 0;
}





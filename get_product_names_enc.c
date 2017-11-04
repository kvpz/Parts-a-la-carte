/*
 This code will be used to extract products from a Encompass webpage containing
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

struct Product {
  const char* price;
  char* title;
  char* availability;
} typedef Product;

char* get_product_availability(myhtml_tree_t* tree,
			       myhtml_tree_node_t* product)
{
  const char* text = myhtml_node_text(product, NULL);
  if(text){
    printf("text: %s\n", text);
  }
}

/*
  Extract product name and its details from the html element.
 */
Product get_table_row_data(myhtml_tree_t* tree, myhtml_tree_node_t* tr)
{
  Product product;
  // This should have a length of 3 (3 <tr> elements);
  myhtml_collection_t* row_data =
    myhtml_get_nodes_by_name_in_scope(tree,
				      NULL,
				      tr,
				      "td",
				      2,
				      NULL);

  // print the product name and its price
  for(int d = 0; d < row_data->length; ++d){
    myhtml_tree_node_t* text_node =
      myhtml_node_child(row_data->list[d]);
    const char* text = myhtml_node_text(text_node, NULL);
    if(text && d == 1){
      product.title = malloc(strlen(text)+1);
      memmove(product.title, text, strlen(text));
      //strncpy(product.title, text, strlen(text)+1);
      product.title[strlen(text)] = '\0';
    }
    else if(text && d == 2){
      product.availability = malloc(strlen(text)+1);
      memmove(product.availability, text, strlen(text));
      //strncpy(product.availability, text, strlen(text)+1);
      product.availability[strlen(text)] = '\0';
    }
    
    //free(text);
  }

  return product;
}

char* get_title(myhtml_tree_t* tree)
{
  char* title;
  myhtml_collection_t *titleCollection =
    myhtml_get_nodes_by_tag_id(tree, NULL, MyHTML_TAG_TITLE, NULL);
    
  if(titleCollection && titleCollection->list && titleCollection->length) {    
    myhtml_tree_node_t *text_node =
      myhtml_node_child(titleCollection->list[0]);
      
    if(text_node) {
      title = myhtml_node_text(text_node, NULL);

      if(title){
	myhtml_collection_destroy(titleCollection);
	return title;
      }
      //fwrite(title, 1, strlen(title), outptr);	    
    }
  }
  
  myhtml_collection_destroy(titleCollection);
  return NULL;
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

Product* get_products(myhtml_tree_t* tree, FILE** outfile, int* total_products)
{
  char parts_table_id[] = "datatable-part";
  Product* _products;
  
  // Get table representing parts
  myhtml_collection_t *products =
    myhtml_get_nodes_by_attribute_value(tree,
					NULL,
					NULL,
					false,
					NULL,
					0,
					parts_table_id,
					strlen(parts_table_id),
					NULL);
  if(!products && !products->list && !products->length)
    return NULL;

  // Get table body (tbody child elements, tr) from parts table
  myhtml_collection_t* products_tbody =
    myhtml_get_nodes_by_name_in_scope(tree,
				      NULL,
				      products->list[0],
				      "tr",
				      2,
				      NULL);

  *total_products = products_tbody->length - 1;
  _products = malloc(sizeof(Product)*(products_tbody->length - 1)); 
  for(int r = 1; r < products_tbody->length; ++r){
    Product _product = get_table_row_data(tree, products_tbody->list[r]);
    _products[r-1] = _product;
  }

  //free(parts_table_id);
  return _products;
  //return NULL;
}

void print_product_html(Product product)
{
  
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
    char* title = get_title(tree);
    //printf("The title of the page is: %s\n\n", title);
    
    // Get the products
    int total_products;
    Product* products = get_products(tree, &outptr, &total_products);
    printf("Total products: %d<br/><br/>\n", total_products);
    for(int i = 0; i < total_products; ++i){
      printf("Product: %s<br/>\n",products[i].title);
      printf("Availability: %s<br/>\n", products[i].availability);
      printf("<br/>\n");
    }
    
    // release resources
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
    
    free(res.html);
    fclose(outptr);
    free(products);
    
    return 0;
}





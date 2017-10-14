/**
   driver.c 
   TV parts lookup
   
   The sources of information will be shopjimmy.com, encompass.com, and, 
   ebay.com.
   
   by Kevin Perez
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <myhtml/api.h>
#include <curl/curl.h>
#define INLINE
#include "api.h"

#define SHOPJIMMY "https://shopjimmy.com"
#define ENCOMPASS "https://encompass.com"
#define EBAY      "https://ebay.com"

#define SJ_SEARCH "https://shopjimmy.com/catalogsearch/result/?q="

// lookup options
enum LOOKUP_OPTIONS
{
  TV_BRAND = 0x00,
  TV_MODEL = 0x01,
  TV_PARTS = 0x02
};

enum LOOKUP_OPTIONS lookup_options[] = {TV_BRAND};

void set_search_options()
{
  char input_options[] = {};
  bool input_correct = false;
  
  menu_search_options();
  char options[] = {'a', 'b', 'c', '\n' };
  char options_to_set[] = {};
  printf("sizeof options to set[]: %d", sizeof(options_to_set));
  do {
    scanf("%s", &input_options);
    for(int c = 0; c < strlen(input_options); ++c){
      for(int o = 0; o < sizeof(options); ++o){
	if(input_options[c] == options[o]){
	  
	}
	  
      }
    }
  }while(!input_correct);
}

void search_encompass(const char* query)
{
  CURL * curl;
  CURLcode res;
  curl = curl_easy_init();
  if(curl){  
  //curl_easy_setopt(curl, CURLOPT_URL,
    //"https://encompass.com/model/ZEN105UC9UA");
  }
}

void search_everywhere()
{
  printf("Enter your query: ");
  char *query;
  scanf("%ms", &query);

  search_shopjimmy(query);
  //search_encompass(query);
  //search_ebay(query);
  free(query);
}

void search()
{ 
  menu_search();
  char menu_option;
  printf(">> ");
  do{
    int scan_result;
    scanf("%%"); // get rid of new line
    scanf("%c", &menu_option);
    switch(menu_option){
    case 'a': case 'A':
      search_everywhere();
      printf("Search complete.\n");
      break;
    case 'b': case 'B':
      set_search_options();
      break;
    case 'q': case 'Q':
      break;
    default:
      printf("\n>> ");
      break;
    }
    menu_search();
  }while(menu_option != 'q' && menu_option != 'Q');
}

int main(int argc, const char *argv[])
{
    bool program_active = true;
    char menu_option;
    menu_main();
    
    do{
	int scanf_result = scanf("%c", &menu_option);
	//printf("main scan_result = %d with value: %c\n",
	//     scanf_result, menu_option);
	switch(menu_option)
	{
	case 'a': case 'A':
	  search(); //lookup();
	  menu_main();
	  break;
	case 'b': case 'B':
	  //history();
	  menu_history();
	  break;
	case 'm': case 'M':
	  menu_main();
	  break;
	case 'q': case 'Q':
	  prog_active = false;
	  break;
	}
    }while(program_active);
    
    return 0;
}


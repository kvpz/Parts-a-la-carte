#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"

size_t shopjimmy_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
  size_t realsize = size * nmemb;  
  time_t current_time = time(NULL);
  char time_str[11];
  sprintf(time_str, "%d", current_time);
  char filename[strlen(time_str) + 4];
  sprintf(filename, "%s_%s", time_str, "sj");
  printf("creating file: %s\n", filename);
  
  // write downloaded content to local file
  FILE * file = fopen(filename, "w");
  if(file != NULL){
    printf("Write to file(%s)? ", filename);
    char w;
    scanf("%c", &w);
    if(w == 'y' || w == 'Y'){
      int fw_elements = fwrite(ptr, sizeof(char), strlen(ptr), file);
      printf("Total elements written to %s: %d\n",
	   filename, fw_elements);
    }
    
    fclose(file);
    // NOTE: the file should be reviewed afterwards to determine if an exact
    // copy already exists because of a previous download. If so, the oldest
    // should be deleted; the time of the search should however be saved.
    
    // execute get_product_names program
    pid_t proc_id = fork();
    if(proc_id != 0){
      printf("The parent process id(this) is: %d\n", proc_id);
      return;
    }
    else{
      if(w == 'y' || w == 'Y')
      char* argv[] = { "get_product_names", filename, NULL };
      else
	char* argv[] = {"get_product_names", "shopjimmy_example.html", NULL };
      execvp("./get_product_names", argv);
      fprintf(stderr, "Error with execvp\n");
      abort();
    }
    
  }
  
  return nmemb;
}

void search_shopjimmy(const char* query)
{
  // this is just for testing
  const char shopjimmy_url[] =
    "https://www.shopjimmy.com/catalogsearch/result/?q=";
  char search_url[strlen(shopjimmy_url) + strlen(query) + 1];
  strcpy(search_url, shopjimmy_url);
  strcpy(&search_url[strlen(shopjimmy_url)], query);
  printf("Searching: %s\n", search_url);
  
  CURL * curl;
  CURLcode res;
  curl = curl_easy_init();
  if(curl){  
    curl_easy_setopt(curl, CURLOPT_URL, "localhost"); // local testing
    //curl_easy_setopt(curl, CURLOPT_URL, search_url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, shopjimmy_callback);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() search_shopjimmy() failure: %s\n",
	      curl_easy_strerror(res));

      // offline test
      printf("Performing offline test with shopjimmy_example.html\n");
      FILE* file = fopen("shopjimmy_example.html", "r");
      char* file_content = malloc(sizeof(char) * 65614);
      fread(file_content, 1, 65614, file);
      shopjimmy_callback(file_content, 1, 65614, NULL);
      free(file_content);
      fclose(file);
    }
    curl_easy_cleanup(curl);
  }
}


/*
  Requests for Encompass
  
  This program is designed to make requests to Shop Jimmy using
  libcurl. The request is logged and the response is printed to a
  file identified as <time>_enc.

  The program is not always be trusted when it mentions that a file
  is cached because it relies on the log file and not necessarily if 
  the file actually exists. 

  The program should only print one line to stdout during production. 
  This line of text will represent the location where the response body
  was saved. Note that the location the file is saved should be accessible
  by the http server for reading and writing.

  Updated 10/29/2017
  Written by Kevin Perez
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include <time.h>
#include <string.h>

#define shopjimmy_uri "http://www.shopjimmy.com/catalogsearch/result/?q="
#define REQUEST_CACHED 1
//#define FILE_LOCATION "/home/kperez/Documents/TV_Parts_and_More/"
//#define FILE_LOCATION "/var/www/html/~kp/"
#define FILE_LOCATION "/usr/tmp/"

/*
  A single log's format: 
  <time> <query> <provider> <response file> \n
  1234567890 55lnx10 ENC
  Rules for logging requests. 
  If a unique request, one that hasn't been made in the past 12 hours, is to
  be made, it should be appended to the end of the log file.
  If an identical request was made less than 12 hours ago, an additional
  request should be prevented. The query string should be logged to the file
  along with the current time and the file associated with it.

  Complexity: high
 */
int log_request(const char* query_string, const char* time,
		const char* provider, char* response_file)
{
  int status = 0;
  FILE* log = fopen("request_log", "r+");
  if(!log){
    perror("There was an error opening request_log\n");
    return -1;
  }

  char* time_str = (char*)malloc(10*sizeof(char));
  char* line = NULL;
  size_t llength = 0;
  
  // Search for the first date that is 12 hours or less to today
  while(getline(&line, &llength, log) != -1){
    snprintf(time_str, 11, "%s", line);
    int tdiff = atoi(time) - atoi(time_str); 
    if(tdiff < 43000){
      char* log_query = strtok(line+11, " ");
      if(strcmp(log_query, query_string) == 0){
	// the request has been made and shouldn't be repeated.
	fclose(log);
	//printf("Request (cached) saved to: %s_enc\n", time_str);
	sprintf(response_file, "%s_enc", time_str);
	free(time_str);
	return REQUEST_CACHED;
      }
    }
  }
  
  // Write to log
  fgetc(log);
  fprintf(log, "%s %s %s /usr/tmp/%s\n", time, query_string, provider,
	  response_file);
  printf("%s", response_file);
  
  fclose(log);
  free(time_str);
  return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
  return written;
}

int main(int argc, char* argv[])
{  
  if(argc <= 1 || argc > 3){
    printf("The search term should be provided as an argument.\n");
    return -1;
  }
  
  char* save_file_location;
  if(argc == 3){
    save_file_location = malloc(sizeof(char) * strlen(argv[2]));
    save_file_location = argv[2];
  }

  time_t timer = time(NULL);
  time(&timer);
  char time_str[11];
  sprintf(time_str, "%d", timer);
  char* response_file = malloc(30);

  int status = log_request(argv[1], time_str, "ENC", response_file);
  if(status == REQUEST_CACHED){
    //printf("The request has already been made within the last 12 hours.\n");
    printf("%s%s", FILE_LOCATION, response_file);
    return 0;
  }

  sprintf(response_file, "%s_enc", time_str);
  FILE* outfile;

  char* request_uri = malloc(strlen(shopjimmy_uri) + strlen(argv[1]));
  sprintf(request_uri, "https://encompass.com/search?searchTerm=%s",
	  argv[1]);
  
  //printf("The requested uri is: %s\n", request_uri);
  
  if(curl_global_init(CURL_GLOBAL_ALL) != 0){
    fprintf(stderr, "There is an issue with curl. Global init error.\n");
    return -1;
  }

  CURL* curl;
  curl = curl_easy_init();
  if(curl == 0){
    fprintf(stderr, "curl_easy_init() error\n");
    return -1;
  }
  struct curl_slist* slist = NULL;
  slist = curl_slist_append(slist, "Accept: text/html, application/xhtml+xml");
  slist = curl_slist_append(slist, "Accept-Language: en-US,en;q=0.5");
  slist = curl_slist_append(slist, "DNT: 1");
  curl_easy_setopt(curl, CURLOPT_URL, request_uri);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  
  char* response_file_location =
    malloc(strlen(FILE_LOCATION) + strlen(response_file) + 1);
  sprintf(response_file_location, "%s%s", FILE_LOCATION, response_file);
  
  // Open file to write response body
  outfile = fopen(response_file_location, "wb");
  if(!outfile){
    printf("opening file failed\n");
    fprintf(stderr, "Opening file %s failed\n", response_file_location);
    curl_easy_cleanup(curl);
    return -1;
  }
  printf("%s", response_file_location);
  
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);

  curl_easy_perform(curl);

  fclose(outfile);

  curl_easy_cleanup(curl);

  free(request_uri);
  free(response_file);
  free(response_file_location);
  
  return 0;
}

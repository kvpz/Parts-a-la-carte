/*
  Requests for Shop Jimmy
  
  This program is designed to make requests to Shop Jimmy using
  libcurl.
  

  10/17/2017
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
/*
  A single log's format: 
  <time> <query> <provider> <response file> \n
  1234567890 55lnx10 SJ
  Rules for logging requests. 
  If a unique request, one that hasn't been made in the past 12 hours, is to
  be made, it should be appended to the end of the log file.
  If an identical request was made less than 12 hours ago, an additional
  request should be prevented. The query string should be logged to the file
  along with the current time and the file associated with it.

  Complexity: high
 */
int log_request(const char* query_string, const char* time,
		const char* provider, const char* response_file)
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
	free(time_str);
	return REQUEST_CACHED;
      }
    }
  }
  
  // Write to log
  fgetc(log);
  fprintf(log, "%s %s %s %s\n", time, query_string, provider,
	  response_file);
  
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
  
  if(argc != 2){
    printf("The search term should be provided as an argument.\n");
    return -1;
  }
  
  CURL* curl;

  time_t timer = time(NULL);
  time(&timer);
  char time_str[11];
  sprintf(time_str, "%d", timer);

  char* response_file = malloc(30);
  sprintf(response_file, "%s_%s", time_str, "sj");

  log_request(argv[1], time_str, "SJ", response_file);

  char body_filename[19];
  sprintf(body_filename, "%s_%s", time_str, response_file);
  FILE* bodyFile;

  char* request_uri = malloc(strlen(shopjimmy_uri) + strlen(argv[1]));
  sprintf(request_uri, "http://www.shopjimmy.com/catalogsearch/result/?q=%s",
  argv[1]);
  
  printf("The requested uri is: %s\n", request_uri);
  
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, request_uri);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

  // Open file to write response body
  bodyFile = fopen(body_filename, "wb");
  if(!bodyFile){
    curl_easy_cleanup(curl);
    return -1;
  }

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, bodyFile);

  //curl_easy_perform(curl);

  fclose(bodyFile);

  curl_easy_cleanup(curl);

  free(request_uri);
  
  return 0;
}

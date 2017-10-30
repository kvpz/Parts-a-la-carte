#include <signal.h>
#include <iostream>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

using namespace cgicc;

int spawn(char* program, char** arg_list){
  pid_t child_pid;
  /* Duplicate this process. */
  child_pid = fork ();
  if (child_pid != 0)
    /* This is the parent process. */
    return child_pid;
  else {
    /* Now execute PROGRAM, searching for it in the path. */
    execv(program, arg_list);
    /* The execvp function returns only if an error occurs. */
    fprintf(stderr, "an error occurred in execvp\n");
    abort();
  }
}

void* spawn_thread(void* unused)
{
  while(1)
    fputc('x', stderr);
  return NULL;
}

int main(int argc, char* argv[])
{
  int child_status;

  pthread_attr_t attr;
  pthread_t thread;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  Cgicc cgi;
  /*
  cgicc::form_iterator name = cgi.getElement("name");
  if(name != cgi.getElements().end()){
    printf("iterators refer to invalid element\n");
  }
  */

  //std::string name = cgi("name");

  std::cout << cgicc::HTTPHTMLHeader() << std::endl;
  std::cout << html() << "hi from cgi" << std::endl;

  char* arg_list[] = {
    "/var/www/cgi-bin/get_product_names",
    "shopjimmy_example.html",
    NULL
  };
  spawn("get_product_names", arg_list);

  pthread_create(&thread, &attr, &thread_function, NULL);
  pthread_attr_destroy(&attr);
  
  /* Wait for the child process to complete 
  wait(&child_status);
  if(WIFEXITED (child_status))
    printf("The child process exited normally, with exit code %d\n",
	   WEXITSTATUS (child_status));
  else
    printf("The child process exited abnormally\n");
  */
  std::cout << "Done with main cgi\n" << html() << std::endl;
  
  return 0;
}

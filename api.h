#ifndef TVPARTS_API_H
#define TVPARTS_API_H

#ifndef INLINE
# if __GNUC__ && !__GNUC_STDC_INLINE__
#  define INLINE extern inline
# else
#  define INLINE inline
# endif
#endif

/*
  Interactive Menus
 */

INLINE void menu_main(){
  printf("\n** Main Menu **\n");
  printf("A. Lookup\n");
  printf("B. History\n");
  printf("M. Main Menu\n");
  printf("Q, quit\n");
  printf(">> ");

}

INLINE void menu_search(){
  printf("\n** Lookup Menu **\n");
  printf("A. New Search (default: all websites)\n");
  printf("B. Advanced Search\n");
  printf("Q. Back to Main Menu\n");
}

INLINE void menu_search_options()
{
  printf("Lookup Options to Search by Menu\n");
  printf("A. TV Brand\n");
  printf("B. TV Model\n");
  printf("C. Part Number\n");
  printf("(Enter all desired options in one line witout spaces)\n");
  printf(">> ");
}


INLINE void menu_history(){
  printf("\nHello from menu_history()\n");
}


void search_shopjimmy(const char* query);

/*
  This should handle verifying if the request is desirable, i.e. that ShopJimmy
   did not deny the request and that the request actually went through. If this
   is the case, then the webpage should be cached for a limited amount of time.
   
 */
size_t
shopjimmy_callback(char* ptr, size_t size, size_t nmemb, void* userdata);


#endif // TVPARTS_API_H

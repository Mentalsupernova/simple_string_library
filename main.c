#define SIMPLE_STRING_IMPLEMENTATION
#include "simple_string.h"

int main(void){
  const char * cstr = "test,set"; 
  

  simple_string_t string =  ss_from_cstr(cstr, cstr_length(cstr));
  printf("string - %s\n",string.string);

  split_result_t split_res =  ss_split(&string, ",");
  for(size_t i =0;i<split_res.count;i++){
    printf("splited part %ld - %s\n",i,split_res.parts[i].string);
  }

  return 0;
}


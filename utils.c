#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

int INDEX(int line, int col, int width){
  return line * width + col;
}

size_t get_file_size(const char * name){
  FILE * f = fopen(name, "r");
  fseek(f, 0, SEEK_END); // seek to end of file
  size_t size = ftell(f); // get current file pointer
  fclose(f);
  return size;
}

void read_file(const char * name, char * buffer, size_t size){
  FILE * f = fopen(name, "r");
  fread(buffer, size, 1, f);
  fclose(f);
}

char * consume_until(char l, char * buffer, char * stream){
  while(*stream != l){
    *buffer = *stream;
    buffer++;
    stream++;
  }
  *buffer = '\0';
  return stream;
}

/* Includes tabs and end of line */
char * consume_until_whitespace(char * buffer, char * stream){
  while(*stream != ' ' && *stream != '\t' && *stream != '\n'){
    *buffer = *stream;
    buffer++;
    stream++;
  }
  *buffer = '\0';
  return stream;
}

char * ignore_until(char l, char * stream){
  while(*stream != l){
    stream++;
  }
  return stream;
}

/* Includes tabs and end of line */
char * ignore_until_whitespace(char * stream){
  while(*stream != ' ' && *stream != '\t' && *stream != '\n'){
    stream++;
  }
  return stream;
}

size_t get_file_lines(char * input){
  size_t lines = 0;
  while(*input != '\0'){
    input = ignore_until('\n', input);
    input++;
    lines++;
  }
  return lines;
}

size_t get_file_cols(char * input){
  size_t columns = 0;
  while(*input != '\n' && *input != '\0'){
    input++;
    columns++;
  }
  return columns;
}

typedef struct match_pair {
  char * key;
  int value;
} match_pair;

int match(char * field, match_pair * pairs, int len){
  for(int i=0; i < len; i++){
    if(strcmp(field, pairs[i].key) == 0){
      return pairs[i].value;
    }
  }

  printf("Unknown field read %s\n", field);
  return -1;
}

bool is_hexadecimal(char l){
  return l >= '0' && l <= '9' || l >= 'a' && l <= 'f' || l >= 'A' && l <= 'F';
}

bool is_one_of(char * field, char * possible[], int len){
  for(int i=0; i < len; i++){
    if(strcmp(field, possible[i]) == 0){
      return true;
    }
  }
  return false;
}

unsigned int count_set_bits(int num){
  unsigned int count = 0;
  while (num) {
    count += num & 1;
    num = num >> 1;
  }
  return count;
}

bool in_array(int v, int * arr, int len){
  for(int i=0; i < len; i++){
    if(v == arr[i]){
      return true;
    }
  }
  return false;
}

void switch_values(int * i, int val1, int val2){
  if(*i == val1){
    *i = val2;
  }
  else if(*i == val2){
    *i = val1;
  }
  else {
    printf("The received value is not one of the two %d\n", *i);
  }
}

char * consume_int(int * num, char * input){
  char number_text[12] = "";
  int idx = 0;
  if(*input == '-' || *input == '+'){
    number_text[0] = *input;
    idx++;
    input++;
  }

  while(*input != '\0' && *input >= '0' && *input <= '9' ){
    number_text[idx] = *input;
    input++;
    idx++;
  }
  number_text[idx] = '\0';
  *num = atoi(number_text);
  return input;
}

char * consume_int64(int64_t * num, char * input){
  char number_text[20] = "";
  int idx = 0;
  if(*input == '-' || *input == '+'){
    number_text[0] = *input;
    idx++;
    input++;
  }

  while(*input != '\0' && *input >= '0' && *input <= '9'){
    number_text[idx] = *input;
    input++;
    idx++;
  }
  number_text[idx] = '\0';
  *num = atoll(number_text);
  return input;
}

int max(int * arr, int len){
  int max = INT_MIN;
  for(int i=0; i < len; i++){
    if(arr[i] > max){
      max = arr[i];
    }
  }
  return max;
}

int min(int * arr, int len){
  int min = INT_MAX;
  for(int i=0; i < len; i++){
    if(arr[i] < min){
      min = arr[i];
    }
  }
  return min;
}

void swap(int *a, int *b){
  int tmp = *b;
  *b = *a;
  *a = tmp;
}

void sort_int_array(int * arr, int len){
  bool all_sorted = false;
  while(!all_sorted){
    all_sorted = true;
    for(int i=0; i < len - 1; i++){
      if(arr[i] > arr[i+1]){
        swap(&arr[i], &arr[i+1]);
        all_sorted = false;
      }
    }
  }
}


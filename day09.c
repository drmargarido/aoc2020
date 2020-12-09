#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.c"

#define PREAMBLE_LEN 25
#define INPUT_FILE "day9_input.txt"

char * process_preamble(char * input, int64_t * preamble){
  for(int i=0; i < PREAMBLE_LEN; i++){
    char num[15] = "";
    input = consume_until_whitespace(num, input);
    input++;
    preamble[i] = atoi(num);
  }
  return input;
}

bool is_valid_number(int num, int64_t * preamble){
  for(int i=0; i < PREAMBLE_LEN; i++){
    for(int j=0; j < PREAMBLE_LEN; j++){
      if(i == j) continue;
      if(preamble[i] + preamble[j] == num){
        return true;
      }
    }
  }
  return false;
}

int64_t process_until_invalid(char * input){
  int64_t preamble[PREAMBLE_LEN] = {0};
  input = process_preamble(input, preamble);
  while(*input != '\0'){
    char num[15] = "";
    input = consume_until_whitespace(num, input);
    input++;

    int64_t val = atoll(num);
    if(!is_valid_number(val, preamble)){
      return val;
    }

    /* Shift all preamble data one position and insert the new value */
    memcpy(preamble, &preamble[1], sizeof(int64_t) * PREAMBLE_LEN - 1);
    preamble[PREAMBLE_LEN - 1] = val;
  }
  return -1;
}

int64_t find_encription_weakness(char * input, int64_t invalid_num){
  int lines = get_file_lines(input);
  int64_t numbers[lines];
  memset(numbers, 0, sizeof(int64_t) * lines);

  /* Parse all numbers to array */
  for(int i=0; i < lines; i++){
    char num[15] = "";
    input = consume_until_whitespace(num, input);
    input++;
    numbers[i] = atoll(num);
  }

  for(int i=0; i < lines; i++){
    for(int len=2; len < lines - i; len++){
      int64_t min = 999999999999999;
      int64_t max = 0;
      int64_t sum = 0;
      for(int it=i; it < len; it++){
        if(numbers[it] < min){
          min = numbers[it];
        }
        if(numbers[it] > max){
          max = numbers[it];
        }
        sum += numbers[it];
      }
      if(sum == invalid_num){
        return min + max;
      }
    }
  }
  return -1;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int64_t invalid_num = process_until_invalid(input);
  printf("Invalid number found %lld\n", invalid_num);

  int64_t weakness_num = find_encription_weakness(input, invalid_num);
  printf("Encryption weakness is %lld\n", weakness_num);

  free(input);
  return 0;
}

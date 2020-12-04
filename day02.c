#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "utils.c"

#define INPUT_FILE "day2_input.txt"

char * parse_line(int * min, int * max, char * letter, char * password, char * input){
  char char_min[3], char_max[3];

  input = consume_until('-', char_min, input);
  *min = atoi(char_min);
  input++;

  input = consume_until(' ', char_max, input);
  *max = atoi(char_max);
  input++;

  *letter = *input;
  input++; input++; input++;

  input = consume_until('\n', password, input);
  input++;

  return input;
}

bool is_password_valid_rule1(int min, int max, char letter, char * password){
  size_t matches = 0;
  while(*password != '\0'){
    if(*password == letter){
      matches++;
    }
    password++;
  }
  return matches >= min && matches <= max;
}

bool is_password_valid_rule2(int min, int max, char letter, char * password){
  if(password[min-1] == letter && password[max-1] == letter){
    return false;
  }
  return password[min-1] == letter || password[max-1] == letter;
}

int check_passwords(char * input){
  int min = 0, max = 0;
  char letter;
  char password[40] = "";
  int valid_count = 0;

  while(*input != '\0'){
    input = parse_line(&min, &max, &letter, password, input);
    if(is_password_valid_rule2(min, max, letter, password)){
      valid_count++;
    }
  }

  return valid_count;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);
  int valid_count = check_passwords(input);
  free(input);
  printf("%d\n", valid_count);
  return 0;
}

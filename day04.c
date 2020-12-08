#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "utils.c"

#define INPUT_FILE "day4_input.txt"

#define BYRTH 1
#define ISSUE_YEAR 2
#define EXPIRE_YEAR 4
#define HEIGHT 8
#define HAIR_COLOR 16
#define EYE_COLOR 32
#define PASSPORT_ID 64
#define COUNTRY_ID 128

bool passport_id_is_valid(char * value){
  if(strlen(value) != 9) {
    return false;
  }
  return atoi(value) != 0;
}

bool eye_color_is_valid(char * value){
  char * colors[] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
  return is_one_of(value, colors, 7);
}

bool hair_color_is_valid(char * value){
  if(*value != '#' || strlen(value) != 7){
    return false;
  }
  for(int i=1; i < 7; i++){
    if(!is_hexadecimal(value[i])){
      return false;
    }
  }
  return true;
}

bool height_is_valid(char * value){
  int len = strlen(value);
  char * scale = &value[len-2];
  if(strcmp(scale, "cm") != 0 && strcmp(scale, "in") != 0){
    return false;
  }

  int min = 150;
  int max = 193;
  if(strcmp(scale, "in") == 0){
    min = 59;
    max = 76;
  }

  value[len-2] = '\0';
  int num = atoi(value);
  return num >= min && num <= max;
}

bool year_is_valid(char * value, int min, int max){
  if(strlen(value) != 4){
    return false;
  }
  int year = atoi(value);
  return year >= min && year <= max;
}

bool is_value_valid(int field, char * value){
  switch(field){
    case BYRTH:
      return year_is_valid(value, 1920, 2002);
    case ISSUE_YEAR:
      return year_is_valid(value, 2010, 2020);
    case EXPIRE_YEAR:
      return year_is_valid(value, 2020, 2030);
    case HEIGHT:
      return height_is_valid(value);
    case HAIR_COLOR:
      return hair_color_is_valid(value);
    case EYE_COLOR:
      return eye_color_is_valid(value);
    case PASSPORT_ID:
      return passport_id_is_valid(value);
    case COUNTRY_ID:
      return true;
  }

  printf("Type: %d - Value %s\n", field, value);
  return true;
}

bool is_passport_valid(int fields){
  int mask = BYRTH | ISSUE_YEAR | EXPIRE_YEAR | HEIGHT | HAIR_COLOR | EYE_COLOR
    | PASSPORT_ID;
  return (fields & mask) == mask;
}

static match_pair pairs[] = {
  {"byr", BYRTH}, {"iyr", ISSUE_YEAR}, {"eyr", EXPIRE_YEAR}, {"hgt", HEIGHT},
  {"hcl", HAIR_COLOR}, {"ecl", EYE_COLOR}, {"pid", PASSPORT_ID}, {"cid", COUNTRY_ID}
};
int identify_field(char * field){
  return match(field, pairs, 8);
}

char * parse_passport(int * fields, char * input){
  char field_name[4] = "";
  char field_value[12] = "";
  while(*input != '\n' && *input != '\0'){
    input = consume_until(':', field_name, input);
    input++;
    input = consume_until_whitespace(field_value, input);
    input++;

    int field = identify_field(field_name);
    if(is_value_valid(field, field_value)){
      *fields |= field;
    }
  }
  if(*input == '\n'){
    input++;
  }
  return input;
}

int check_passports(char * input){
  int valid_count = 0;
  while(*input != '\0'){
    int fields = 0;
    input = parse_passport(&fields, input);
    if(is_passport_valid(fields)){
      valid_count++;
    }
  }
  return valid_count;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int valid_passports = check_passports(input);
  printf("Total Valid Passports %d\n", valid_passports);

  free(input);
  return 0;
}

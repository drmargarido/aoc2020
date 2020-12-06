#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "utils.c"

#define A 1
#define B 2
#define C 4
#define D 8
#define E 16
#define F 32
#define G 64
#define H 128
#define I 256
#define J 512
#define K 1024
#define L 2048
#define M 4096
#define N 8192
#define O 16384
#define P 32768
#define Q 65536
#define R 131072
#define S 262144
#define T 524288
#define U 1048576
#define V 2097152
#define W 4194304
#define X 8388608
#define Y 16777216
#define Z 33554432

#define INPUT_FILE "day6_input.txt"

static match_pair pairs[] = {
  {"a", A}, {"b", B}, {"c", C}, {"d", D}, {"e", E}, {"f", F}, {"g", G},
  {"h", H}, {"i", I}, {"j", J}, {"k", K}, {"l", L}, {"m", M}, {"n", N},
  {"o", O}, {"p", P}, {"q", Q}, {"r", R}, {"s", S}, {"t", T}, {"u", U},
  {"v", V}, {"w", W}, {"x", X}, {"y", Y}, {"z", Z}
};
void detect_person_answers(char * input, int * answers){
  while(*input != '\0'){
    char l[2] = "\0\0";
    l[0] = *input;
    int val = match(l, pairs, 26);
    *answers |= val;
    input++;
  }
}

char * get_group_answers1(char * input, int * answers){
  char person_answers[27] = "";
  while(*input != '\n' && *input != '\0'){
    input = consume_until('\n', person_answers, input);
    input++;
    detect_person_answers(person_answers, answers);
  }
  if(*input == '\n'){
    input++;
  }
  return input;
}

char * get_group_answers2(char * input, int * answers){
  bool is_first_person = true;
  char person_answers[27] = "";
  while(*input != '\n' && *input != '\0'){
    input = consume_until('\n', person_answers, input);
    input++;

    int p_answers = 0;
    detect_person_answers(person_answers, &p_answers);
    if(is_first_person){
      is_first_person = false;
      *answers |= p_answers;
    } else {
      *answers &= p_answers;
    }
  }
  if(*input == '\n'){
    input++;
  }
  return input;
}

int check_groups_answers(char * input){
  int total = 0;
  while(*input != '\0'){
    int group_answers = 0;
    input = get_group_answers2(input, &group_answers);
    total += count_set_bits(group_answers);
  }
  return total;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int total = check_groups_answers(input);
  printf("Total %d\n", total);

  free(input);
  return 0;
}

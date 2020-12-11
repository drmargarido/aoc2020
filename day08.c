#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.c"

#define INPUT_FILE "day8_input.txt"

typedef enum Operation {
  ACC=1, JMP=2, NOP=4
} Operation;

typedef struct Instruction {
  Operation operation;
  int argument;
} Instruction;


static match_pair operation_pairs[] = {
  {"acc", ACC}, {"jmp", JMP}, {"nop", NOP}
};
Operation get_operation(char * op){
  return match(op, operation_pairs, 3);
}

void parse_instructions(char * input, Instruction * instructions){
  char op[4] = "";
  int line = 0;
  while(*input != '\0'){
    input = consume_until_whitespace(op, input);
    input++;
    input = consume_int(&instructions[line].argument, input);
    input++;

    instructions[line].operation = get_operation(op);
    line++;
  }
}

int run_until_loop(Instruction * instructions, int len, bool * reached_end){
  int acc=0;
  int instruction_idx = 0;
  bool already_run[len];
  memset(already_run, 0, sizeof(bool) * len);

  do {
    Instruction i = instructions[instruction_idx];
    already_run[instruction_idx] = true;
    switch(i.operation){
      case ACC:
        acc += i.argument;
        instruction_idx++;
        break;
      case JMP:
        instruction_idx += i.argument;
        break;
      case NOP:
        instruction_idx++;
        break;
    }
    if(instruction_idx >= len){
      *reached_end = true;
    }
  } while(instruction_idx < len && !already_run[instruction_idx]);
  return acc;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int lines = get_file_lines(input);
  Instruction * instructions = malloc(sizeof(Instruction) * lines);
  parse_instructions(input, instructions);
  free(input);

  bool reached_end = false;
  int result = run_until_loop(instructions, lines, &reached_end);
  printf("Accumulator value was %d before loop\n", result);

  Instruction * patched_instructions = malloc(sizeof(Instruction) * lines);
  for(int i=0; i < lines; i++){
    /* Reset to base instructions */
    memcpy(patched_instructions, instructions, sizeof(Instruction) * lines);

    /* Patch instruction on current line and see if it solved the loop */
    Instruction * it = &patched_instructions[i];
    if(it->operation == ACC){
      continue;
    }

    switch_values((int *) &it->operation, JMP, NOP);
    result = run_until_loop(patched_instructions, lines, &reached_end);
    if(reached_end){
      printf("Result when reaching the end %d\n", result);
      break;
    }
  }

  return 0;
}

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.c"

#define INPUT_FILE "day14_input.txt"

typedef enum Operation {SET_MASK, SET_MEMORY} Operation;
typedef struct Instruction {
  Operation op;
  uint64_t val1;
  uint64_t val2;
} Instruction;

#define MEMORY_SIZE 1000000
typedef struct MemoryEntry {
  uint64_t address;
  uint64_t value;
} MemoryEntry;

/* Time to implement an hashmap */
void set_address(uint64_t address, uint64_t value, MemoryEntry * memory){
  for(int i=0; i < MEMORY_SIZE; i++){
    if(memory[i].address == address){
      memory[i].value = value;
      return;
    }

    if(memory[i].address == 0 && memory[i].value == 0){
      memory[i].address = address;
      memory[i].value = value;
      return;
    }
  }
  printf("Not enough memory!\n");
}

match_pair pairs[] = {{"mask", SET_MASK}, {"mem", SET_MEMORY}};
uint64_t one = 1;
void parse_instructions(char * input, Instruction * instructions){
  int idx = 0;
  char op[5];
  char mask[37];
  while(*input != '\0'){
    consume_until_cond_m(op, input, (*input != ' ' && *input != '['));
    instructions[idx].op = match(op, pairs, 2);
    if(instructions[idx].op == SET_MASK){
      input = &input[3];
      input = consume_until_whitespace(mask, input);
      input++;

      instructions[idx].val1 = 0;
      instructions[idx].val2 = UINT64_MAX;
      for(int i=0; i < 36; i++){
        if(mask[35-i] == '1')  instructions[idx].val1 |= (one << i); // set mask
        if(mask[35-i] == '0')  instructions[idx].val2 &= ~(one << i); // clear mask
      }
    }
    else if(instructions[idx].op == SET_MEMORY) {
      input++;
      input = consume_int64(&instructions[idx].val1, input);
      input = &input[4];
      input = consume_int64(&instructions[idx].val2, input);
      input++;
    }
    else {
      printf("Unkonw operation read %s\n", op);
    }
    idx++;
  }
}

void run_instructions(Instruction * instructions, int len, MemoryEntry * memory){
  uint64_t set_mask;
  uint64_t clear_mask;
  for(int i=0; i < len; i++){
    Instruction in = instructions[i];
    if(in.op == SET_MASK){
      set_mask = in.val1;
      clear_mask = in.val2;
    }
    else if(in.op == SET_MEMORY){
      set_address(in.val1, (in.val2 | set_mask) & clear_mask, memory);
    }
  }
}

int find_floating_positions(uint64_t set_mask, uint64_t clear_mask, uint64_t * floating_positions){
  int size = 0;
  uint64_t floating_mask = set_mask ^ clear_mask;
  for(int i=0; i < 36; i++){
    if(floating_mask & (one << i)){
      floating_positions[size] = i;
      size++;
    }
  }
  return size;
}

typedef struct Node {
  uint64_t value;
  int      index;
} Node;
void run_instructions2(Instruction * instructions, int len, MemoryEntry * memory){
  Node * next = malloc(sizeof(Node) * 100);
  uint64_t set_mask = 0;
  uint64_t clear_mask = UINT64_MAX;
  uint64_t floating[36] = {};
  int size = 0;
  for(int i=0; i < len; i++){
    Instruction in = instructions[i];
    if(in.op == SET_MASK){
      set_mask = in.val1;
      clear_mask = in.val2;
      size = find_floating_positions(set_mask, clear_mask, floating);
    }
    else if(in.op == SET_MEMORY){
      next[0].value = in.val1 | set_mask;
      next[0].index = 0;
      next[1].value = (in.val1 | set_mask) ^ (one << floating[0]);
      next[1].index = 0;

      int next_len = 2;
      while(next_len > 0){
        Node n = next[next_len-1];
        next_len--;

        if(n.index >= size){
          set_address(n.value, in.val2, memory);
          continue;
        }

        Node base = {n.value, n.index+1};
        next[next_len] = base;
        next_len++;

        Node different = {n.value ^ (one << floating[n.index]), n.index + 1};
        next[next_len] = different;
        next_len++;
      }
    }
  }
  free(next);
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int lines = get_file_lines(input);
  Instruction * instructions = malloc(sizeof(Instruction) * lines);
  parse_instructions(input, instructions);
  free(input);

  MemoryEntry * memory = malloc(sizeof(MemoryEntry) * MEMORY_SIZE);
  memset(memory, 0, sizeof(MemoryEntry) * MEMORY_SIZE);
  run_instructions(instructions, lines, memory);
  uint64_t result = 0;
  for(int i=0; i < MEMORY_SIZE; i++){
    result += memory[i].value;
  }
  printf("Part1 result: %ld\n", result);

  memset(memory, 0, sizeof(MemoryEntry) * MEMORY_SIZE);
  run_instructions2(instructions, lines, memory);
  result = 0;
  for(int i=0; i < MEMORY_SIZE; i++){
    result += memory[i].value;
  }
  printf("Part2 result: %ld\n", result);

  free(memory);
  free(instructions);
  return 0;
}


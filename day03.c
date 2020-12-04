#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "utils.c"

#define INPUT_FILE "day3_input.txt"

#define IS_EMPTY false
#define HAS_TREE true

void generate_map(bool * map, char * input, int lines, int cols){
  for(int i=0; i < lines; i++){
    for(int j=0; j < cols;){
      if(*input != '.' && *input != '#'){
        input++;
        continue;
      }

      int idx = INDEX(i, j, cols);
      if(*input == '.') map[idx] = IS_EMPTY;
      else map[idx] = HAS_TREE;

      input++;
      j++;
    }
  }
}

#define SLOPE1 1
#define SLOPE2 2
#define SLOPE3 3
#define SLOPE4 4
#define SLOPE5 5

void get_next_position11(int * line, int * col){
  *line = *line + 1;
  *col = *col + 1;
}

void get_next_position13(int * line, int * col){
  *line = *line + 1;
  *col = *col + 3;
}

void get_next_position15(int * line, int * col){
  *line = *line + 1;
  *col = *col + 5;
}

void get_next_position17(int * line, int * col){
  *line = *line + 1;
  *col = *col + 7;
}

void get_next_position21(int * line, int * col){
  *line = *line + 2;
  *col = *col + 1;
}

void get_next_position(int * line, int * col, int slope){
  switch(slope){
    case SLOPE1:
      get_next_position11(line, col);
      break;
    case SLOPE2:
      get_next_position13(line, col);
      break;
    case SLOPE3:
      get_next_position15(line, col);
      break;
    case SLOPE4:
      get_next_position17(line, col);
      break;
    case SLOPE5:
      get_next_position21(line, col);
      break;
    default:
      printf("Invalid slope received!\n");
  }
}

int check_tree_hits(bool * map, int lines, int cols, int slope){
  int count = 0;
  int line = 0;
  int col = 0;
  while(line < lines){
    int idx = INDEX(line, col, cols);
    if(map[idx]){
      count++;
    }
    get_next_position(&line, &col, slope);
    if(col >= cols) col = col % cols;
  }
  return count;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int lines = get_file_lines(input);
  int cols = get_file_cols(input);
  printf("Lines %d | Cols %d\n", lines, cols);

  bool * map = malloc(lines*cols*sizeof(bool));
  generate_map(map, input, lines, cols);
  free(input);

  int hits1 = check_tree_hits(map, lines, cols, SLOPE1);
  int hits2 = check_tree_hits(map, lines, cols, SLOPE2);
  int hits3 = check_tree_hits(map, lines, cols, SLOPE3);
  int hits4 = check_tree_hits(map, lines, cols, SLOPE4);
  int hits5 = check_tree_hits(map, lines, cols, SLOPE5);
  printf("Tree Hits %d | %d | %d | %d | %d \n", hits1, hits2, hits3, hits4, hits5);
  free(map);

  int mul = 1;
  float result = hits1 * hits2 * hits3 * hits4;
  mul = mul * 10;
  result = result * (hits5 / 10.0);
  printf("Final Result %f * %d\n", result, mul);
  return 0;
}

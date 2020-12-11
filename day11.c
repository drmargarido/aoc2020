#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utils.c"

#define INPUT_FILE "day11_input.txt"

#define FLOOR    '.'
#define EMPTY    'L'
#define OCCUPIED '#'

void parse_initial_state(char * input, char * state){
  while(*input != '\0'){
    if(*input != '\n'){
      *state = *input;
      state++;
    }
    input++;
  }
}

int count_adjacent_seats(int line, int col, int lines, int cols, char * state){
  int count = 0;
  for(int i=max(line - 1, 0); i <= min(line + 1, lines-1); i++){
    for(int j=max(col - 1, 0); j <= min(col + 1, cols-1); j++){
      if(i == line && j == col)  continue; /* Ignore center */

      int pos = INDEX(i, j, cols);
      if(state[pos] == OCCUPIED)  count++;
    }
  }
  return count;
}

int count_visible_seats(int line, int col, int lines, int cols, char * state){
  int count = 0;
  for(int i=-1; i <= 1; i++){
    for(int j=-1; j <= 1; j++){
      if(i == 0 && j == 0)  continue; /* Ignore center */
      int next_l = line + i;
      int next_c = col + j;
      do {
        bool in_map = next_l >= 0 && next_l < lines && next_c >= 0 && next_c < cols;
        if(!in_map)  break;

        int pos = INDEX(next_l, next_c, cols);
        if(state[pos] != FLOOR){
          if(state[pos] == OCCUPIED)  count++;
          break;
        }

        next_l += i;
        next_c += j;
      } while(true);
    }
  }
  return count;
}

void run_state_step(char * state, char * tmp_state, int lines, int cols){
  memcpy(tmp_state, state, lines * cols);
  for(int i=0; i < lines; i++){
    for(int j=0; j < cols; j++){
      int pos = INDEX(i, j, cols);
      int occupied = count_adjacent_seats(i, j, lines, cols, tmp_state);
      if(tmp_state[pos] == EMPTY && occupied == 0)     state[pos] = OCCUPIED;
      if(tmp_state[pos] == OCCUPIED && occupied >= 4)  state[pos] = EMPTY;
    }
  }
}

void run_state_step2(char * state, char * tmp_state, int lines, int cols){
  memcpy(tmp_state, state, lines * cols);
  for(int i=0; i < lines; i++){
    for(int j=0; j < cols; j++){
      int pos = INDEX(i, j, cols);
      int occupied = count_visible_seats(i, j, lines, cols, tmp_state);
      if(tmp_state[pos] == EMPTY && occupied == 0)     state[pos] = OCCUPIED;
      if(tmp_state[pos] == OCCUPIED && occupied >= 5)  state[pos] = EMPTY;
    }
  }
}

int count_seats(char * state, char seat_type, int len){
  int count = 0;
  for(int i=0; i < len; i++){
    if(state[i] == seat_type)  count++;
  }
  return count;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int lines = get_file_lines(input);
  int cols = get_file_cols(input);

  int state_size = lines * cols;
  char * state = malloc(state_size);
  parse_initial_state(input, state);
  free(input);

  char * tmp_state = malloc(state_size);
  do {
    run_state_step2(state, tmp_state, lines, cols);
  }
  while(memcmp(state, tmp_state, state_size) != 0);
  free(tmp_state);

  int occupied_seats = count_seats(state, OCCUPIED, state_size);
  printf("Occupied Seats %d\n", occupied_seats);
  free(state);
  return 0;
}

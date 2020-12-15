#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LAST_POS 30000000

int run_turns(int turns, int last_num, int * last_positions, int start_turn){
  for(int i=start_turn; i < turns; i++){
    int last_pos = last_positions[last_num];
    last_positions[last_num] = i-1;
    if(last_pos == -1)  last_num = 0;
    else                last_num = (i-1) - last_pos;
  }
  return last_num;
}

void setup_last_positions(int * last_positions){
  memset(last_positions, ~(0), sizeof(int) * MAX_LAST_POS);
  last_positions[14] = 0;
  last_positions[8] = 1;
  last_positions[16] = 2;
  last_positions[0] = 3;
  last_positions[1] = 4;
  /* No last number in list yet so we can check if it existed before */
}

int main(){
  int last_num = 17;
  int * last_positions = malloc(sizeof(int) * MAX_LAST_POS);

  setup_last_positions(last_positions);
  int result1 = run_turns(2020, last_num, last_positions, 6);
  printf("Part1 Number on turn 2020 -> %d\n", result1);

  setup_last_positions(last_positions);
  int result2 = run_turns(30000000, last_num, last_positions, 6);
  printf("Part2 Number on turn 30000000 -> %d\n", result2);

  free(last_positions);
  return 0;
}

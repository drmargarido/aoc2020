#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "utils.c"

#define INPUT_FILE "day10_input.txt"

void parse_numbers(char * input, int * numbers){
  for(int idx=0; *input != '\0'; idx++){
    input = consume_int(&numbers[idx], input);
    input++;
  }
}

int count_differences(int * arr, int len, int diff){
  int count = 0;
  for(int i=0; i < len - 1; i++){
    if(arr[i+1] - arr[i] == diff){
      count++;
    }
  }
  return count;
}

int64_t calc_possible_arrangements(int * arr, int len){
  int64_t total = 1;
  /*
    After a sequence of 3 or more 1s we start to have invalid states, so
    instead of doing just a power of 2 to get the possible paths we need
    to take into account the invalid states.
  */
  int sequence_possibilities[6] = {0, 1, 2, 4, 7, 13}; // Tribonacci sequence
  int sequence_size = 0;
  for(int i=0; i < len-1; i++){
    /* We know the distance between elements is always 1 or 3 */
    int diff = arr[i+1] - arr[i];
    if(diff == 1){
      sequence_size++;
    }
    else if(diff == 3){
      if(sequence_size > 0){
        total = total * sequence_possibilities[sequence_size];
        sequence_size = 0;
      }
    }
  }
  return total;
}


int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int lines = get_file_lines(input);
  int len = lines + 2;
  int * numbers = malloc(sizeof(int) * len);
  parse_numbers(input, &numbers[1]);
  numbers[0] = 0;
  numbers[len-1] = arr_max(numbers, len-1) + 3;
  free(input);

  sort_int_array(numbers, len);
  int one_differences = count_differences(numbers, len, 1);
  int three_differences = count_differences(numbers, len, 3);
  printf("1-Jolt diff %d | 3-Jolt diff %d\n", one_differences, three_differences);
  printf("Part1 result %d\n", one_differences * three_differences);

  int64_t arrangements = calc_possible_arrangements(numbers, len);
  printf("Part2 possible arrangements %ld\n", arrangements);

  free(numbers);
  return 0;
}

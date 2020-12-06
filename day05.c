#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "utils.c"

#define INPUT_FILE "day5_input.txt"
#define ROWS 128
#define COLS 8

int binary_search_value(
  char lowerc, char upperc, int min, int max, char * input, int len
){
  int result = 0;
  int current_min = min;
  int current_max = max;
  for(int i=0; i < len; i++, input++){
    int diff = ceil((current_max - current_min) / 2.0);
    if(*input == lowerc){
      current_max -= diff;
      if(diff == 1){
        result = current_min;
      }
    }
    else if(*input == upperc){
      current_min += diff;
      if(diff == 1){
        result = current_max;
      }
    }
  }

  return result;
}

int get_boarding_pass_seat(char * pass){
  int line = binary_search_value('F', 'B', 0, ROWS - 1, pass, 7);
  int col = binary_search_value('L', 'R', 0, COLS - 1, &pass[7], 3);
  return INDEX(line, col, 8);
}

int check_seats(char * input, bool * seats){
  int highest_id = 0;
  char boarding_pass[11] = "";
  while(*input != '\0'){
    input = consume_until('\n', boarding_pass, input);
    input++;
    int seat_id = get_boarding_pass_seat(boarding_pass);
    seats[seat_id] = true;
    if(seat_id > highest_id){
      highest_id = seat_id;
    }
  }
  return highest_id;
}

int find_my_seat(bool * seats){
  int start=0;
  int end=ROWS*COLS-1;
  while(seats[start] != true){
    start++;
  }
  while(seats[end] != true){
    end--;
  }
  for(; start < end; start++){
    if(!seats[start]){
      return start;
    }
  }
  return -1;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  bool seats[ROWS * COLS] = {false};
  int highest_id = check_seats(input, seats);
  free(input);
  printf("Highest seat id: %d\n", highest_id);

  int my_seat = find_my_seat(seats);
  printf("My Seat %d\n", my_seat);
  return 0;
}

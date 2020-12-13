#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include "utils.c"

#define INPUT_FILE "day13_input.txt"

typedef struct Bus {
  int id;
  int offset;
} Bus;

int parse_notes(char * input, int * arrival, Bus * buses){
  int buses_count = 0;
  input = consume_int(arrival, input);
  input++;
  int offset = 0;
  while(*input != '\0' && *input != '\n'){
    if(*input != 'x'){
      input = consume_int(&buses[buses_count].id, input);
      buses[buses_count].offset = offset;
      buses_count++;
    } else {
      input++;
    }
    input++;
    offset++;
  }
  return buses_count;
}

static int get_wait_time(int64_t arrival, int bus_id){
  int remainder = arrival % bus_id;
  if(remainder == 0)  return 0;
  return bus_id - remainder;
}

int find_earliest_bus(int arrival, Bus * buses, int total_buses){
  int earliest_bus = -1;
  int min = INT_MAX;
  for(int i=0; i < total_buses; i++){
    int wait_time = get_wait_time(arrival, buses[i].id);
    if(wait_time < min){
      min = wait_time;
      earliest_bus = i;
    }
  }
  return earliest_bus;
}

Bus get_max_id_bus(Bus * buses, int total_buses){
  Bus max_bus = {-1, -1};
  for(int i=0; i < total_buses; i++){
    Bus b = buses[i];
    if(b.id > max_bus.id)  max_bus = b;
  }
  return max_bus;
}

bool are_rules_valid(int64_t timestamp, Bus * buses, int total_buses, int64_t * step){
  bool matched_all = true;
  int64_t new_step = 1;
  for(int i=0; i < total_buses; i++){
    Bus b = buses[i];
    /* Offset of some buses is bigger than their id, so we need to count the future buses */
    int future_bus = b.id;
    while(future_bus < b.offset){
      future_bus += b.id;
    }

    /*
      We can get get the next timestamp were the same buses match by finding
      their lower common multiplier, so we use that as the next step.
    */
    int wait_time = get_wait_time(timestamp, future_bus);
    if(wait_time != b.offset)  matched_all = false;
    else                       new_step *= b.id;
  }

  *step = new_step;
  return matched_all;
}

int64_t find_matching_timestamp(Bus * buses, int total_buses){
  int64_t step = 1;
  int64_t timestamp = 0;

  /* Find timestamp that matches the rules */
  while(!are_rules_valid(timestamp, buses, total_buses, &step)){
    timestamp += step;
  }

  /* By using the step find the lowest possible timestamp which matches the rules*/
  while(timestamp - step > 0){
    timestamp -= step;
  }

  return timestamp;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);
  int arrival;
  Bus * buses = malloc(size * sizeof(Bus));
  int total_buses = parse_notes(input, &arrival, buses);
  free(input);

  int earliest_bus = find_earliest_bus(arrival, buses, total_buses);
  int wait_time = get_wait_time(arrival, buses[earliest_bus].id);
  int result = buses[earliest_bus].id * wait_time;
  printf("Part1 Bus %d | Wait Time %d | Result %d\n", buses[earliest_bus].id, wait_time, result);

  int64_t timestamp = find_matching_timestamp(buses, total_buses);
  printf("Part2 timestamp %ld\n", timestamp);
  free(buses);
  return 0;
}

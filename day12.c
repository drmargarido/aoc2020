#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "utils.c"
#include "vec2.c"

#define INPUT_FILE "day12_input.txt"

typedef struct Instruction {
  int arg;
  char direction;
} Instruction;

void parse_instructions(char * input, Instruction * instructions){
  int i=0;
  while(*input != '\0'){
    instructions[i].direction = *input;
    input++;
    input = consume_int(&instructions[i].arg, input);
    input++;
    i++;
  }
}

Vec2 calculate_new_pos(Vec2 pos, Vec2 direction, int mul){
  direction = vec2_mul(direction, mul);
  return vec2_vsum(pos, direction);
}

static Vec2 none  = {0, 0};
static Vec2 north = {0, 1};
static Vec2 south = {0, -1};
static Vec2 east  = {1, 0};
static Vec2 west  = {-1, 0};
Vec2 get_direction_by_degrees(int degrees){
  if(degrees == 0)    return east;
  if(degrees == 90)   return north;
  if(degrees == 180)  return west;
  if(degrees == 270)  return south;
  return none;
}

Vec2 process_instructions(Instruction * instructions, int len){
  Vec2 pos = {0, 0};
  Vec2 direction = east;
  int rotation = 0;
  for(int i=0; i < len; i++){
    Instruction in = instructions[i];
    switch(in.direction){
      case 'N':
        pos = calculate_new_pos(pos, north, in.arg);
        break;
      case 'S':
        pos = calculate_new_pos(pos, south, in.arg);
        break;
      case 'E':
        pos = calculate_new_pos(pos, east, in.arg);
        break;
      case 'W':
        pos = calculate_new_pos(pos, west, in.arg);
        break;
      case 'L':
        rotation = (rotation + in.arg) % 360;
        direction = get_direction_by_degrees(rotation);
        break;
      case 'R':
        rotation -= in.arg;
        if(rotation < 0)  rotation = 360 + rotation;
        direction = get_direction_by_degrees(rotation);
        break;
      case 'F':
        pos = calculate_new_pos(pos, direction, in.arg);
        break;
    }
  }
  return pos;
}

Vec2 rotate_waypoint(Vec2 waypoint, int rot){
  int dot = vec2_dot(waypoint, east);
  float norm = vec2_len(waypoint) * vec2_len(east);
  float current_angle = radians_to_degrees(acos(dot / norm));

  /* acos returns the minimum possible angle to the vector */
  if(waypoint.y < 0)  current_angle = 360 - current_angle;

  float next_angle = current_angle + rot;
  float radius = vec2_len(waypoint);

  float new_x = radius * cos(degrees_to_radians(next_angle));
  float new_y = radius * sin(degrees_to_radians(next_angle));
  /* prevent failure of float to int conversion in some cases */
  if(new_x < 0)  new_x -= 0.5;
  if(new_x > 0)  new_x += 0.5;
  if(new_y < 0)  new_y -= 0.5;
  if(new_y > 0)  new_y += 0.5;

  waypoint.x = new_x;
  waypoint.y = new_y;
  return waypoint;
}

Vec2 process_instructions2(Instruction * instructions, int len){
  Vec2 ship_pos = {0, 0};
  Vec2 waypoint_pos = {10, 1};
  for(int i=0; i < len; i++){
    Instruction in = instructions[i];
    switch(in.direction) {
      case 'N':
        waypoint_pos = calculate_new_pos(waypoint_pos, north, in.arg);
        break;
      case 'S':
        waypoint_pos = calculate_new_pos(waypoint_pos, south, in.arg);
        break;
      case 'E':
        waypoint_pos = calculate_new_pos(waypoint_pos, east, in.arg);
        break;
      case 'W':
        waypoint_pos = calculate_new_pos(waypoint_pos, west, in.arg);
        break;
      case 'L':
        waypoint_pos = rotate_waypoint(waypoint_pos, in.arg);
        break;
      case 'R':
        waypoint_pos = rotate_waypoint(waypoint_pos, -1*in.arg);
        break;
      case 'F':
        ship_pos = calculate_new_pos(ship_pos, waypoint_pos, in.arg);
        break;
    }
  }

  return ship_pos;
}

int main(int argc, char ** argv){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);

  int lines = get_file_lines(input);
  Instruction * instructions = malloc(sizeof(Instruction) * lines);
  parse_instructions(input, instructions);
  free(input);

  Vec2 r = process_instructions(instructions, lines);
  printf("Part1 (x: %d, y: %d) -> %d\n", r.x, r.y, abs(r.x) + abs(r.y));

  r = process_instructions2(instructions, lines);
  printf("Part2 (x: %d, y: %d) -> %d\n", r.x, r.y, abs(r.x) + abs(r.y));

  free(instructions);
  return 0;
}

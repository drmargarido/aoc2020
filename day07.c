#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "utils.c"

#define INPUT_FILE "day7_input.txt"
#define MAX_COLOR_LEN 30
#define MAX_RULES_COUNT 4

typedef struct Child {
  char color[MAX_COLOR_LEN];
  int count;
} Child;

typedef struct Rule {
  char color[MAX_COLOR_LEN];
  Child bags[MAX_RULES_COUNT];
  int bags_count;
} Rule;

char * read_color(char * input, char * color){
  input = consume_until(' ', color, input);
  input++;
  int len = strlen(color);
  color[len] = ' ';
  input = consume_until(' ', &color[len+1], input);
  input++;
  input = ignore_until_whitespace(input);
  return input;
}

void parse_rules(char * input, Rule * rules){
  int line = 0;
  while(*input != '\0'){
    Rule * r = &rules[line];
    r->bags_count = 0;
    input = read_color(input, r->color);
    input++;
    input = ignore_until_whitespace(input);

    int idx = 0;
    char count[3] = "";
    while(*input != '\n' && *input != '\0'){
      input++;
      input = consume_until(' ', count, input);
      if(strcmp("no", count) == 0){
        input = ignore_until('\n', input);
        input++;
        break;
      }

      r->bags[idx].count = atoi(count);
      r->bags_count++;

      input++;
      input = read_color(input, r->bags[idx].color);
      idx++;
    }
    if(*input == '\n'){
      input++;
    }
    line++;
  }
}

bool already_exists_in_list(Rule * bag, Rule * checked, int checked_length){
  for(int i=0; i < checked_length; i++){
    if(strcmp(bag->color, checked[i].color) == 0){
      return true;
    }
  }
  return false;
}

int bag_in(Rule * bag, Rule * rules, int total_rules){
  int count = 0;
  Rule * remaining = malloc(sizeof(Rule) * 100);
  Rule * checked = malloc(sizeof(Rule) * 300);
  int checked_count = 0;
  remaining[0] = *bag;
  int remaining_count = 1;
  while(remaining_count > 0){
    Rule current_rule = remaining[remaining_count - 1];
    remaining_count--;

    for(int i=0; i < total_rules; i++){
      if(already_exists_in_list(&rules[i], checked, checked_count)){
        continue;
      }
      for(int j=0; j < rules[i].bags_count; j++){
        Child * child_bag = &rules[i].bags[j];
        if(strcmp(current_rule.color, child_bag->color) == 0){
          remaining[remaining_count] = rules[i];
          remaining_count++;
          count++;
          checked[checked_count] = rules[i];
          checked_count++;
        }
      }
    }
  }
  free(checked);
  free(remaining);
  return count;
}

int search_bag_index(char * color, Rule * rules, int len){
  for(int i=0; i < len; i++){
    if(strcmp(color, rules[i].color) == 0){
      return i;
    }
  }
  return -1;
}

typedef struct RuleNode {
  Rule rule;
  int parent_count;
} RuleNode;

int count_total_needed_bags(Rule bag, Rule * rules, int len){
  int total = 0;
  RuleNode * remaining = malloc(sizeof(RuleNode) * 100);
  RuleNode start = {bag, 1};
  remaining[0] = start;
  int remaining_count = 1;
  while(remaining_count > 0){
    RuleNode n = remaining[remaining_count-1];
    remaining_count--;

    for(int i=0; i < n.rule.bags_count; i++){
      Child b = n.rule.bags[i];
      total += b.count * n.parent_count;
      int idx = search_bag_index(b.color, rules, len);
      if(idx == -1){
        continue;
      }
      RuleNode child_node = {rules[idx], b.count * n.parent_count};
      remaining[remaining_count] = child_node;
      remaining_count++;
    }
  }
  free(remaining);
  return total;
}

int main(){
  size_t size = get_file_size(INPUT_FILE);
  char * input = malloc(size);
  read_file(INPUT_FILE, input, size);
  int lines = get_file_lines(input);

  printf("Lines %d\n", lines);
  Rule * rules = malloc(sizeof(Rule) * lines);

  parse_rules(input, rules);
  free(input);

  int idx = search_bag_index("shiny gold", rules, lines);
  Rule r = rules[idx];
  int count = bag_in(&r, rules, lines);
  printf("Bag in %d other bags\n", count);

  int total = count_total_needed_bags(r, rules, lines);
  printf("Total needed bags %d\n", total);

  free(rules);
  return 0;
}

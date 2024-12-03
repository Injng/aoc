#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int FILE_LINES = 1000;

bool check_safe(int* report, int size, int skip) {
  // initialize states
  int state = 0;    // increasing: 1, decrasing: -1, start: 0
  bool is_safe = true;
  
  // iterate through each number
  int prev = *report;
  int curr;
  for (int i = 1; i < size; i++) {
    // handle the skip
    if (skip == 0 && i == 1) {
      prev = *(++report);
      continue;
    } else if (skip == i) {
      report++;
      continue;
    }

    // set the new current value
    curr = *(++report);

    // make sure they differ by at least one and at most 3
    int difference = abs(curr - prev);
    if (difference < 1 || difference > 3) {
      is_safe = false;
      break;
    }

    // check which state it should be in and see if it is consistent
    if (state == 0) {
      if (curr > prev) state = 1;
      else if (curr < prev) state = -1;
      else {
        is_safe = false;
        break;
      }
    } else if (state == 1) {
      if (curr <= prev) {
        is_safe = false;
        break;
      }
    } else {
      if (curr >= prev) {
        is_safe = false;
        break;
      }
    }

    // update the previous number
    prev = curr;
  }

  return is_safe;
}

int main(void) {
  FILE* input_file = fopen("day2/day2.txt", "r");    // open input file
  int count = 0;    // count number of safe reports

  // ensure file exists
  if (input_file == NULL) {
    printf("Error: input file does not exist\n");
    return 1;
  }

  // iterate through each report in the file
  for (int i = 0; i < FILE_LINES; i++) {
    // read in input line
    char line[25];
    fgets(&line[0], 25, input_file);

    // put all tokens into an array
    int report[9];
    int size = 1;
    int* report_ptr = &report[0];    // pointer to start of report

    // parse the tokens
    char* token = strtok(&line[0], " ");
    sscanf(token, "%d", report_ptr++);
    token = strtok(NULL, " ");
    while (token != NULL) {
      sscanf(token, "%d", report_ptr++);
      token = strtok(NULL, " ");
      size++;
    }

    // brute force over possible mistakes and check if it is safe
    bool is_safe = false;
    for (int j = 0; j < size; j++) {
      if (check_safe(&report[0], size, j)) {
        is_safe = true;
        break;
      }
    }

    // check if the report is safe or not
    if (is_safe) count++;
  }

  // output the result
  printf("%d\n", count);
  return 0;
}


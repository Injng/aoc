#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

const int FILE_LINES = 1000;

int compare(const void* a, const void* b) {
  int32_t arg1 = *(const int32_t*) a;
  int32_t arg2 = *(const int32_t*) b;

  if (arg1 < arg2) return -1;
  else if (arg1 == arg2) return 0;
  else return 1;
}

int main(void) {
  // open file
  FILE* input_file = fopen("day1/day1.txt", "r");
  if (input_file == NULL) {
    printf("Error: cannot open input file\n");
    return 1;
  }

  // read in each line
  char line[15];
  int32_t left_ids[FILE_LINES];
  int32_t right_ids[FILE_LINES];
  int count = 0;
  while (count < FILE_LINES) {
    fgets(&line[0], sizeof(line), input_file);
    int32_t left_id, right_id;
    sscanf(&line[0], "%d %d\n", &left_id, &right_id);
    left_ids[count] = left_id;
    right_ids[count] = right_id;
    count++;
  }

  // sort the arrays of ids
  qsort(&left_ids[0], FILE_LINES, sizeof(int32_t), &compare);
  qsort(&right_ids[0], FILE_LINES, sizeof(int32_t), &compare);

  // take the distances
  int32_t distance = 0;
  for (int i = 0; i < FILE_LINES; i++) {
    distance += abs(left_ids[i] - right_ids[i]);
  }

  // print answer
  printf("%d\n", distance);
}

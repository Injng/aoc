#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

const int FILE_LINES = 6;
const int LINE_LENGTH = 3600;

int main() {
  // open the file stream
  FILE* input_file = fopen("day3/day3.txt", "r");
  if (input_file == NULL) {
    printf("Error: failed to open input file\n");
    return 1;
  }

  // compile regex expression
  regex_t regex_mul, regex_do, regex_dont;
  regcomp(&regex_mul, "mul\\(([0-9]|[1-9][0-9]*),([0-9]|[1-9][0-9]*)\\)", REG_EXTENDED);
  regcomp(&regex_do, "do\\(\\)", REG_EXTENDED);
  regcomp(&regex_dont, "don't\\(\\)", REG_EXTENDED);

  // iterate over each line in the input file
  char line[LINE_LENGTH];
  long long ans = 0;
  bool disabled_start = false;
  for (int i = 0; i < FILE_LINES; i++) {
    // get a line from the input file
    fgets(&line[0], LINE_LENGTH, input_file);
    int line_size = strlen(&line[0]);

    // get first disabled moment
    regmatch_t matches[1];
    int next_disabled;
    if (regexec(&regex_dont, &line[0], 1, &matches[0], 0) != 0) {
      printf("Warning: failed to match regex_dont\n");
      next_disabled = line_size;
    } else {
      next_disabled = matches[0].rm_so;
    }

    // if disabled start, jump to first enabled moment
    int offset = 0;
    if (disabled_start && regexec(&regex_do, &line[0], 1, &matches[0], 0) != 0) {
      printf("Warning: failed to match regex_do on disabled start\n");
      continue;
    } else if (disabled_start) {
      offset = matches[0].rm_so;
    }

    // match regex
    while (regexec(&regex_mul, &line[offset], 1, &matches[0], 0) == 0
           && offset < line_size) {
      // handle disabled moment
      if (matches[0].rm_so + offset > next_disabled) {
        // set offset to continue matching
        offset = next_disabled + 6;
      
        // find next enabled moment
        if (regexec(&regex_do, &line[ offset], 1, &matches[0], 0) != 0) {
          // this means there is no next enabled moment, indicating disabled start
          disabled_start = true;
          break;
        }

        // jump to next enabled moment
        offset += matches[0].rm_so;

        // try to find next disabled moment
        if (regexec(&regex_dont, &line[offset], 1, &matches[0], 0) != 0) {
          // this means there is no next disabled moment
          disabled_start = false;
          next_disabled = line_size;
        } else {
          // otherwise, set next disabled
          next_disabled = offset + matches[0].rm_so;
        }
        continue;
      }

      
      // parse match for numbers
      int mul_a, mul_b;
      sscanf(&line[matches[0].rm_so + offset], "mul(%d,%d)", &mul_a, &mul_b);
      ans += mul_a * mul_b;

      // set offset to continue matching
      offset += matches[0].rm_eo;
    }
  }

  // free regex
  regfree(&regex_mul);
  regfree(&regex_do);
  regfree(&regex_dont);

  // return the answer
  printf("%lld\n", ans);
  return 0;
}


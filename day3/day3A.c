#include <regex.h>
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
  regex_t regex;
  regcomp(&regex, "mul\\(([0-9]|[1-9][0-9]*),([0-9]|[1-9][0-9]*)\\)", REG_EXTENDED);

  // iterate over each line in the input file
  char line[LINE_LENGTH];
  long long ans = 0;
  for (int i = 0; i < FILE_LINES; i++) {
    // get a line from the input file
    fgets(&line[0], LINE_LENGTH, input_file);
    int line_size = strlen(&line[0]);
    
    // match regex
    regmatch_t matches[1];
    int offset = 0;
    while (regexec(&regex, &line[offset], 1, &matches[0], 0) == 0 && offset < line_size) {
      // parse match for numbers
      int mul_a, mul_b;
      sscanf(&line[matches[0].rm_so + offset], "mul(%d,%d)", &mul_a, &mul_b);
      ans += mul_a * mul_b;

      // set offset to continue matching
      offset += matches[0].rm_eo;
    }
  }

  // free regex
  regfree(&regex);

  // return the answer
  printf("%lld\n", ans);
  return 0;
}


#include "include/util.h"
#include <stdio.h>

long get_file_length(const char *filePath) {
  FILE *file = fopen(filePath, "rb");

  if (file == NULL) {
    // Handle file opening error
    printf("Error opening the file.\n");
    return -1; // Or any other error code
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    // Handle fseek error
    printf("Error moving the file pointer.\n");
    fclose(file);
    return -1; // Or any other error code
  }

  long length = ftell(file);
  fclose(file);

  return length;
}

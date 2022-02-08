#include <stdio.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("The program expects one argument\n");
    return 1;
  }

  if (argv[1][0] == 'A')
    printf("A\n");
  else if (argv[1][0] == 'B')
    printf("B\n");
  else
    printf("C\n");

  return 0;
}
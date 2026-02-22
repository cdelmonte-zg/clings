// intro1.c - Welcome to clings!
//
// This is your first exercise. The program compiles but doesn't
// work correctly — it exits with an error code.
//
// In C, returning 0 from main() means "success".
// Any other value means "failure".
//
// TODO: Fix the return value so the program exits successfully.

#include <stdio.h>

int main(void) {
    printf("Welcome to clings! Let's learn some advanced C.\n");

    return 1;  // BUG: wrong exit code
}

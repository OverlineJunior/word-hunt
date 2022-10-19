#include <stdio.h>
#include <string.h>

#define MATRIX_SIZE 3
#define MAX_WORDS 1

int main() {
    char matrix[MATRIX_SIZE][MATRIX_SIZE] = {
		{'F', 'O', 'O'},
		{'O', 'H', 'A'},
		{'O', 'J', 'S'},
	};

    char words[MAX_WORDS][MATRIX_SIZE] = {"FOO"};

    return 0;
}

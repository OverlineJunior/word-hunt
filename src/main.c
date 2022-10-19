#include <stdio.h>
#include <string.h>

#define MATRIX_SIZE 3
#define MAX_WORDS 1

typedef enum {
    UP,
    DOWN,
} VerticalDirection;

typedef enum {
    LEFT,
    RIGHT,
} HorizontalDirection;

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    int found;
    Position first_pos;
} ScanResult;

ScanResult scan_vertical(char matrix[MATRIX_SIZE][MATRIX_SIZE], char scan_word[MATRIX_SIZE], VerticalDirection dir) {
	
}

int main() {
    char matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {'F', 'O', 'O'},
        {'O', 'H', 'A'},
        {'O', 'J', 'S'},
    };

    char words[MAX_WORDS][MATRIX_SIZE] = {"FOO"};

    return 0;
}

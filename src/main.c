#include <stdio.h>
#include<stdbool.h>
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

// TODO: What if there are 2 occurrences of scan_word in a single vertical line?
// TODO: Make use of dir.
ScanResult scan_vertical(char matrix[MATRIX_SIZE][MATRIX_SIZE], char scan_word[MATRIX_SIZE], int col, VerticalDirection dir) {
    int count = 0;

    for (int i = 0; scan_word[i] != '\0'; i++) {
        char ch = matrix[i][col];
        char scan_ch = scan_word[i];

        if (ch != scan_ch) break;

        count++;
    }

    ScanResult res;
    return res;
}

int main() {
    char matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {'F', 'O', 'O'},
        {'O', 'H', 'A'},
        {'O', 'J', 'S'},
    };

    char words[MAX_WORDS][MATRIX_SIZE + 1] = {"FOO"};

    VerticalDirection dir = DOWN;
    scan_vertical(matrix, words[0], 0, dir);

    return 0;
}

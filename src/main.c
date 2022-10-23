#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MATRIX_SCALE 7
#define MAX_WORDS 1

typedef enum {
    RIGHT,
    LEFT,
    UP,
    DOWN,
} Direction;

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    int found;
    Position first_pos;
    Position last_pos;
} FindFirstResult;

void display_find_first_result(FindFirstResult res) {
    char found[6];
    strcpy(found, res.found == true ? "true" : "false");

    printf(
        "Found: %s\n"
        "FirstPos: {row: %i, col: %i}\n"
        "LastPos: {row: %i, col: %i}\n",
        found,
        res.first_pos.row,
        res.first_pos.col,
        res.last_pos.row,
        res.last_pos.col);
}

FindFirstResult find_first(char matrix[MATRIX_SCALE][MATRIX_SCALE], char word[], Position start_pos, Direction dir) {
    int match_combo = 0;
    int word_len = strlen(word);
    Position first_pos;
    FindFirstResult res = {.found = false};

    // Init: if direction is vertical, start from start_pos.row since we will be moving by rows; else, move by col.
    // Condition: if direction is down or right, use a condition that accepts increasing row or col; else, decreasing row or col.
    // Increment: going down or right means increasing row or col while up or left means decreasing it.
    for (int i = dir == DOWN || dir == UP ? start_pos.row : start_pos.col;
         dir == DOWN || dir == RIGHT ? i < MATRIX_SCALE : i >= 0;
         dir == DOWN || dir == RIGHT ? i++ : i--)
    {
        // If direction is vertical, move by row, while the col is constant; else, move by col, while the row is constant.
        const Position pos = {.row = dir == DOWN || dir == UP ? i : start_pos.row,
                              .col = dir == DOWN || dir == UP ? start_pos.col : i};
        const char ch = matrix[pos.row][pos.col];

        if (ch == word[match_combo]) {
            match_combo++;

            if (match_combo == 1) {
                first_pos = pos;
            }

            if (match_combo == word_len) {
                res.found = true;
                res.first_pos = first_pos;
                res.last_pos = pos;

                break;
            }
        } else {
            match_combo = 0;
        }
    }

    return res;
}

int main() {
    char matrix[MATRIX_SCALE][MATRIX_SCALE] = {
        {'F', 'O', 'O', 'X', 'L', 'C', 'Z'},
        {'O', 'H', 'A', 'O', 'O', 'F', 'A'},
        {'O', 'J', 'S', 'I', 'O', 'D', 'F'},
        {'X', 'F', 'O', 'O', 'H', 'E', 'L'},
        {'F', 'O', 'L', 'C', 'S', 'B', 'B'},
        {'O', 'O', 'F', 'J', 'C', 'Q', 'P'},
        {'O', 'K', 'P', 'F', 'O', 'A', 'Z'},
    };

    char words[MAX_WORDS][MATRIX_SCALE + 1] = {"FOO"};

    Position start_pos = {.row = 1, .col = 6};
    Direction dir = LEFT;
    FindFirstResult res = find_first(matrix, words[0], start_pos, dir);
    display_find_first_result(res);

    return 0;
}

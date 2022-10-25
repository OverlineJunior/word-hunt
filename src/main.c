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

typedef struct {
    char word[MATRIX_SCALE];
    int count;
} WordAndCount;

typedef struct {
    WordAndCount word_and_counts[MAX_WORDS];
    int word_count;
} MatrixData;

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

void display_matrix_data(MatrixData data) {
    for (int i = 0; i < data.word_count; i++) {
        const WordAndCount w_and_c = data.word_and_counts[i];
        printf("%s: %i\n", w_and_c.word, w_and_c.count);
    }
}

FindFirstResult find_first(char word[], char matrix[MATRIX_SCALE][MATRIX_SCALE], Position start_pos, Direction dir) {
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

MatrixData get_matrix_data(char matrix[MATRIX_SCALE][MATRIX_SCALE], char words[MAX_WORDS][MATRIX_SCALE + 1]) {
    // TODO:
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

    const MatrixData data = get_matrix_data(matrix, words);
    display_matrix_data(data);

    return 0;
}

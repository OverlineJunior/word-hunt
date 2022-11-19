// NOTE: For now, it is being assumed that the amount of unique words is equal to MAX_WORDS.

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
	Position first_pos;
	Direction dir;
} Encounter;

typedef struct {
    char word[MATRIX_SCALE + 1];
    int count;
    Encounter encounters[MATRIX_SCALE * MATRIX_SCALE];
} WordData;

typedef struct {
    WordData word_datas[MAX_WORDS];
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
        res.last_pos.col
    );
}

void display_matrix_data(MatrixData matrix_data) {
    for (int i = 0; i < MAX_WORDS; i++) {
        const WordData word_data = matrix_data.word_datas[i];
        printf("'%s' foi encontrado %i vezes:\n", word_data.word, word_data.count);

        for (int j = 0; j < word_data.count; j++) {
            const Encounter enc = word_data.encounters[j];

            char dir_str[11];
            strcpy(dir_str, enc.dir == DOWN || enc.dir == UP ? "Vertical" : "Horizontal");

            printf("%u. 1a posicao: (%u, %u)   Direcao: %s\n", j + 1, enc.first_pos.row, enc.first_pos.col, dir_str);
        }

        printf("\n");
    }
}

FindFirstResult find_first(
    char word[],
    Position start_pos,
    Direction dir,
    char matrix[MATRIX_SCALE][MATRIX_SCALE]
) {
    int match_combo = 0;
    int word_len = strlen(word);
    Position first_pos;
    FindFirstResult res = {.found = false};

    // Init: if direction is vertical, start from start_pos.row since we will be moving by rows; else, move by col.
    // Condition: if direction is down or right, use a condition that accepts increasing row or col; else, decreasing row or col.
    // Increment: going down or right means increasing row or col while up or left means decreasing it.
    for (
        int i = dir == DOWN || dir == UP ? start_pos.row : start_pos.col;
        dir == DOWN || dir == RIGHT ? i < MATRIX_SCALE : i >= 0;
        dir == DOWN || dir == RIGHT ? i++ : i--
    ) {
        // If direction is vertical, move by row, while the col is constant; else, move by col, while the row is constant.
        const Position pos = {
            .row = dir == DOWN || dir == UP ? i : start_pos.row,
            .col = dir == DOWN || dir == UP ? start_pos.col : i
        };
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

MatrixData get_vertical_matrix_data(
    char matrix[MATRIX_SCALE][MATRIX_SCALE],
    char words[MAX_WORDS][MATRIX_SCALE + 1]
) {
    const int last_index = MATRIX_SCALE - 1;
    MatrixData matrix_data;
    Position pos = {0, 0};
    Direction dir = DOWN;

    for (int i = 0; i < MAX_WORDS; i++) {
        char word[MATRIX_SCALE + 1];
        strcpy(word, words[i]);

        WordData word_data = {.count = 0};
        strcpy(word_data.word, word);

        while (true) {
            FindFirstResult res = find_first(word, pos, dir, matrix);
            const int finishing_row = dir == DOWN ? last_index : 0;
            const int is_dir_finished = res.found ? res.last_pos.row == finishing_row : true;
            const int is_scan_finished = is_dir_finished && dir == UP && pos.col == last_index;

            if (res.found) {
                Encounter enc = {res.first_pos, dir};
                word_data.encounters[word_data.count] = enc;

                word_data.count++;
            }

            if (is_scan_finished) break;

            if (is_dir_finished) {
                if (dir == UP) {
                    pos.row = 0;
                    pos.col++;
                    dir = DOWN;
                } else {
                    pos.row = last_index;
                    dir = UP;
                }
            } else {
                pos.row = res.last_pos.row + 1;
            }
        }

        matrix_data.word_datas[i] = word_data;
    }

    return matrix_data;
}

int main() {
    char matrix[MATRIX_SCALE][MATRIX_SCALE] = {
        {'F', 'O', 'O', 'X', 'L', 'C', 'O'},
        {'O', 'H', 'A', 'O', 'O', 'F', 'O'},
        {'O', 'J', 'S', 'I', 'O', 'D', 'F'},
        {'X', 'F', 'O', 'O', 'H', 'E', 'L'},
        {'F', 'O', 'L', 'C', 'S', 'B', 'F'},
        {'O', 'O', 'F', 'J', 'C', 'Q', 'O'},
        {'O', 'K', 'P', 'F', 'O', 'A', 'O'},
    };

    char words[MAX_WORDS][MATRIX_SCALE + 1] = {"FOO"};

    const MatrixData data = get_vertical_matrix_data(matrix, words);
    display_matrix_data(data);

    return 0;
}

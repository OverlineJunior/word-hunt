#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SCALE 10
#define MAX_WORDS 10

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

void assert_msg(int condition, char msg[]) {
	if (!condition) {
		puts(msg);
		exit(1);
	}
}

void load_matrix_data_in_file(MatrixData matrix_data, int word_count) {
    FILE *file_ptr = fopen("../result.md", "a");

    for (int i = 0; i < word_count; i++) {
        const WordData word_data = matrix_data.word_datas[i];

        char times[6];
        strcpy(times, word_data.count == 1 ? "vez" : "vezes");

        fprintf(file_ptr, "# '%s' foi encontrado %i %s:\n", word_data.word, word_data.count, times);

        for (int j = 0; j < word_data.count; j++) {
            const Encounter enc = word_data.encounters[j];

            char dir_str[11];
            strcpy(dir_str, enc.dir == DOWN || enc.dir == UP ? "Vertical" : "Horizontal");

            fprintf(file_ptr, "%u. 1a posicao: (%u, %u)   Direcao: %s\n", j + 1, enc.first_pos.row, enc.first_pos.col, dir_str);
        }

        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);
}

void clear_result_file() {
    FILE *file_ptr = fopen("../result.md", "w");
    assert_msg(file_ptr != NULL, "Nao foi possivel abrir o caminho ../result.md");

    fclose(file_ptr);
}

void fill_matrix_from_file(char matrix[][MATRIX_SCALE], FILE *file_ptr) {
	int row = 0, col = 0;
	char ch;

	do {
		ch = fgetc(file_ptr);

		if ((ch == ' ') || (int) ch == -1) continue;
		
		if (ch == '\n') {
			assert_msg(col == MATRIX_SCALE, "Nao pode haver menos do que 10 letras em uma linha");

			row++;
			col = 0;
		} else {
			assert_msg(col < MATRIX_SCALE, "Nao pode haver mais do que 10 letras em uma linha");

			matrix[row][col] = toupper(ch);
			col++;
		}
	} while (ch != EOF);

	assert_msg(row < MATRIX_SCALE, "Nao pode haver mais do que 10 linhas");
}

int read_words(char words[][MATRIX_SCALE + 1]) {
    int count = 0;

    while (true) {
        printf(
            "Selecione uma opcao:\n"
            "0. Dar uma palavra a ser buscada\n"
            "1. Sair\n"
        );

        int n = 2;
        scanf("%u", &n);
        fflush(stdin);

        if (n == 0) {
            printf("Entre com a palavra a ser buscada: ");

            char word[MATRIX_SCALE + 1];
            fgets(word, MATRIX_SCALE, stdin);
            strupr(word);
            word[strcspn(word, "\n")] = 0;

            strcpy(words[count], word);

            count++;
        } else if (n == 1) {
            if (count == 0) {
                printf("E necessario entrar com pelo menos 1 palavra\n");
                continue;
            }

            break;
        } else {
            printf("Numero invalido\n");
            continue;
        }

        if (count >= MAX_WORDS) break;
    }

    return count;
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

MatrixData sort_matrix_data(MatrixData matrix_data, int word_count) {
    while (true) {
        int did_swap = false;

        for (int i = 0; i < word_count; i++) {
            WordData wdata = matrix_data.word_datas[i];
            WordData next_wdata = matrix_data.word_datas[i + 1];

            if (next_wdata.count > wdata.count) {
                matrix_data.word_datas[i] = next_wdata;
                matrix_data.word_datas[i + 1] = wdata;
                did_swap = true;
            }
        }

        if (!did_swap) break;
    }

    return matrix_data;
}

MatrixData get_vertical_matrix_data(
    char matrix[MATRIX_SCALE][MATRIX_SCALE],
    char words[MAX_WORDS][MATRIX_SCALE + 1],
    int word_count
) {
    const int last_index = MATRIX_SCALE - 1;
    MatrixData matrix_data;
    
    for (int i = 0; i < word_count; i++) {
        Position pos = {0, 0};
        Direction dir = DOWN;

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

MatrixData get_horizontal_matrix_data(
    char matrix[MATRIX_SCALE][MATRIX_SCALE],
    char words[MAX_WORDS][MATRIX_SCALE + 1],
    int word_count
) {
    const int last_index = MATRIX_SCALE - 1;
    MatrixData matrix_data;

    for (int i = 0; i < word_count; i++) {
        Position pos = {0, 0};
        Direction dir = RIGHT;

        char word[MATRIX_SCALE + 1];
        strcpy(word, words[i]);

        WordData word_data = {.count = 0};
        strcpy(word_data.word, word);

        while (true) {
            FindFirstResult res = find_first(word, pos, dir, matrix);
            const int finishing_col = dir == RIGHT ? last_index : 0;
            const int is_dir_finished = res.found ? res.last_pos.col == finishing_col : true;
            const int is_scan_finished = is_dir_finished && dir == LEFT && pos.row == last_index;

            if (res.found) {
                Encounter enc = {res.first_pos, dir};
                word_data.encounters[word_data.count] = enc;

                word_data.count++;
            }

            if (is_scan_finished) break;

            if (is_dir_finished) {
                if (dir == LEFT) {
                    pos.row++;
                    pos.col = 0;
                    dir = RIGHT;
                } else {
                    pos.col = last_index;
                    dir = LEFT;
                }
            } else {
                pos.col = res.last_pos.col + 1;
            }
        }

        matrix_data.word_datas[i] = word_data;
    }

    return matrix_data;
}

// Returns the merge of the vertical and horizontal data.
MatrixData get_matrix_data(
    char matrix[MATRIX_SCALE][MATRIX_SCALE],
    char words[MAX_WORDS][MATRIX_SCALE + 1],
    int word_count
) {
    const MatrixData vertical_data = get_vertical_matrix_data(matrix, words, word_count);
    const MatrixData horizontal_data = get_horizontal_matrix_data(matrix, words, word_count);
    MatrixData matrix_data;

    for (int i = 0; i < word_count; i++) {
        WordData vert_wdata = vertical_data.word_datas[i];
        WordData hori_wdata = horizontal_data.word_datas[i];

        WordData merged_wdata = {
            .count = vert_wdata.count + hori_wdata.count,
        };
        strcpy(merged_wdata.word, vert_wdata.word);

        for (int j = 0; j < vert_wdata.count; j++) {
            merged_wdata.encounters[j] = vert_wdata.encounters[j];
        }

        for (int j = 0; j < hori_wdata.count; j++) {
            merged_wdata.encounters[j + vert_wdata.count] = hori_wdata.encounters[j];
        }

        matrix_data.word_datas[i] = merged_wdata;
    }

    return matrix_data;
}

int main() {
    clear_result_file();

    FILE *file_ptr = fopen("../playground.txt", "r");
	assert_msg(file_ptr != NULL, "Nao foi possivel abrir o caminho '../playground.txt'");

    char matrix[MATRIX_SCALE][MATRIX_SCALE];
    fill_matrix_from_file(matrix, file_ptr);

    char words[MAX_WORDS][MATRIX_SCALE + 1];
    const int word_count = read_words(words);

    const MatrixData data = get_matrix_data(matrix, words, word_count);
    const MatrixData sorted = sort_matrix_data(data, word_count);
    load_matrix_data_in_file(sorted, word_count);

    fclose(file_ptr);

    return 0;
}

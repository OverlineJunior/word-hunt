```c
struct Position {
	int row;
	int col;
};

struct FindFirstResult {
	int found;
	int first_pos; // Exercise wants this displayed.
	int last_pos; // Used in order to continue scanning from where the last scan ended.
};

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

FindFirstResult find_first(char matrix[?][?], char scan_word[?], Position pos, Direction dir)



/*
	NOTE: Only vertical scanning is being considered, meaning this is incomplete prototyping.


	TODO: Implement horizontal scanning. Theory:

	Scan  |  Anchor

	↑ . .
	, . .
	↓ . .	O . .
		 	. . .
	← , →	. . .
	. . .
	. . .

	. ↑ .
	. , .
	. ↓ .	. . .
		 	. O .
	. . .	. . .
	← , →
	. . .

	. . ↑
	. . ,
	. . ↓	. . .
		 	. . .
	. . .	. . O
	. . .
	← , →

	Note that ← , → means 2 scans: , , → and ← , ,
*/



pos = (0, 0)
dir = DOWN

loop:
	res = find_first(matrix, "FOO", pos, dir)

	if dir == UP && pos.col == MATRIX_SIZE - 1:
		if (res.found && res.last_pos.row == 0) || !res.found:
			break

	if res.found && res.last_pos.row != MATRIX_SIZE - 1:
		// Continue from where it ended.
		pos = (res.last_pos.row + 1, res.last_pos.col)
	else:
		if dir == UP:
			// Since we start going down, dir == UP here means we *fully* ended scanning this col.
			// Start scanning from the start of the next col going down.
			pos = (pos.row, pos.col + 1)
			dir = DOWN
		else:
			// Here, dir != UP means we ended scanning down in this col.
			// Start scanning UP in the same col.
			dir = UP

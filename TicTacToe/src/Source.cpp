#include <iostream>
#include <string>
#include <limits>

#define TIC_TAC_BOXS 9

const int X_VALUE = 1;
const int O_VALUE = 2;

int tic_tac_board[TIC_TAC_BOXS];
int current_user = 1;

int ai = 1;
int human = 2;

void InitBoard() {
	for (int i = 0; i < TIC_TAC_BOXS; i++)
		tic_tac_board[i] = 0;
}

int GetUserMove() {
	int move = 0;
	while (!(std::cin >> move)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return -1;
	}
	return ((move > 0 && move < 10) ? move : -1);
}

void PrintOutInputInfo(int input) {
	if (input != -1)
		std::cout << "User puts piece at: " << input << std::endl;
	else
		std::cout << "Error: Input has to be 1 through 9\n";
}

void PrintBoard() {
	std::cout << "Tic Tac Toe!\n";
	std::cout << "Current Player: " << current_user << std::endl;

	for (int i = 0; i < TIC_TAC_BOXS; i++) {
		if (tic_tac_board[i] == 0) {
			std::cout << " " << i + 1 << " ";
			if ((i + 1) % 3 == 0) 
				std::cout << std::endl;
		}
		else {
			switch (tic_tac_board[i]) {
			case X_VALUE: 
				std::cout << " X ";
				break;
			case O_VALUE:
				std::cout << " O ";
				break;
			}
			if ((i + 1) % 3 == 0)
				std::cout << std::endl;
		}
	}

	std::cout << std::endl;
}

int CheckIfDraw() {
	for (int i = 0; i < TIC_TAC_BOXS; i++) 
		if (tic_tac_board[i] == 0)
			return 0;

	return 3;
}

void Update(int input) {
	if (input > 0 && input < 10) {
		if (tic_tac_board[input - 1] == 0) {
			tic_tac_board[input - 1] = current_user;
			if (current_user == human)
				current_user = ai;
		}
	}    
}

int CheckRow(int base, int stride) {
	return tic_tac_board[base] & tic_tac_board[base + stride] & tic_tac_board[base + 2 * stride];
}

int GetWinner() {
	int winner = 0;
	winner |= CheckRow(0, 4);
	for (int i = 0; i < 3; i++) 
		winner |= CheckRow(i, 3);	
	winner |= CheckRow(2, 2);
	for (int i = 0; i < 9; i += 3) 
		winner |= CheckRow(i, 1);

	return winner;
}

void PrintGameOver(int winner) {
	if (winner == 0)
		std::cout << "DRAW!\n";
	else 
		std::cout << "WINNER IS PLAYER " << winner << std::endl;

	std::cout << std::endl;
}

int Minimax(int depth, bool maximizing) {
	int winner = GetWinner();
	if (winner == 1)
		return -10;
	if (winner == 2)
		return 10;

	if (CheckIfDraw() == 3)
		return 0;

	if (maximizing) {
		int best_score = -1000;
		for (int i = 0; i < TIC_TAC_BOXS; i++) {
			if (tic_tac_board[i] == 0) {
				tic_tac_board[i] = ai;
				best_score = std::max(best_score, Minimax(depth + 1, !maximizing));
				tic_tac_board[i] = 0;
			}
		}
		return best_score;
	}
	else {
		int best_score = 1000;
		for (int i = 0; i < TIC_TAC_BOXS; i++) {
			if (tic_tac_board[i] == 0) {
				tic_tac_board[i] = human;
				best_score = std::min(best_score, Minimax(depth + 1, !maximizing));
				tic_tac_board[i] = 0;
			}
		}

		return best_score;
	}
}

void AI() {
	int best_score = -1000;
	int m = 0;
	for (int i = 0; i < TIC_TAC_BOXS; i++) {
		if (tic_tac_board[i] == 0) {
			tic_tac_board[i] = ai;
			int s = Minimax(0, false);
			tic_tac_board[i] = 0;
			if (s > best_score) {
				best_score = s;
				m = i;
			}
		}
	}

	tic_tac_board[m] = ai;
	current_user = human;
}

int main() {
	bool game_running = true;
	int game_winner = 0;

	InitBoard();
	PrintBoard();

	do {
		int input = 0;
		if(current_user == human)
			input = GetUserMove();
		if(current_user == ai)
			AI();
		if(current_user == human)
			Update(input);

		PrintBoard();
		
		int winner = GetWinner();
		if (winner != 0 || CheckIfDraw() == 3) {
			game_winner = winner;
			PrintGameOver(game_winner);
			InitBoard();
			game_winner = 0;	
			current_user = 1;
			PrintBoard();
		}
	} while (game_running);
}
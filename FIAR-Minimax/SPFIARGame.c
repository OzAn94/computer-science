#include "SPFIARGame.h"
#include "SPFIARParserAdditionalHeaders.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
* Force add element to the end of the array list, which means
* that if the list is full, the first element will be deleted.
*
* @param src   - the source array list
* @param elem  - the new element to be inserted
* @return
* SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
* SP_ARRAY_LIST_SUCCESS - otherwise
*/
static SP_ARRAY_LIST_MESSAGE forceSpArrayListAddLast(SPArrayList* src, int elem) {
	if (spArrayListIsFull(src)) {
		spArrayListRemoveFirst(src);
	}

	return spArrayListAddLast(src, elem);
}

SPFiarGame* spFiarGameCreate(int historySize) {
	int i, j;
	SPFiarGame *game;

	if (historySize <= 0) {
		return NULL;
	}

	game = (SPFiarGame*)(malloc(sizeof(SPFiarGame)));
	if ((void*)game == NULL) {
		return NULL;
	}

	game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;

	// set game board
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			(game->gameBoard)[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
		}
	}

	// set tops
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		(game->tops)[i] = 0;
	}
	
	game->history = spArrayListCreate(historySize);

	if ((void*)(game->history) == NULL) {
		free(game);
		return NULL;
	}
	
	return game;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src) {
	int i, j;
	SPFiarGame *game;

	if ((void*)src == NULL) {
		return NULL;
	}

	game = (SPFiarGame*)(malloc(sizeof(SPFiarGame)));

	if ((void*)game == NULL) {
		return NULL;
	}

	game->currentPlayer = src->currentPlayer;

	// set game board
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			(game->gameBoard)[i][j] = (src->gameBoard)[i][j];
		}
	}

	// set tops
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		(game->tops)[i] = (src->tops)[i];
	}

	game->history = spArrayListCopy(src->history);

	if ((void*)(game->history) == NULL) {
		free(game);
		return NULL;
	}

	return game;
}

void spFiarGameDestroy(SPFiarGame* src) {
	if ((void*)src == NULL) {
		return;
	}

	spArrayListDestroy(src->history);
	free(src);
}

/*
* Change the current player of the game.
* @param src the game
*/
static void changePlayer(SPFiarGame* src) {
	if ((void*)src == NULL) {
		return;
	}

	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}

	else {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
}

/*
* Set the board cell row x col.
* Assumes row, col are within range.
* @param src the game
* @param row the row
* @param col the column
* @param symbol the symbol to set
*/
static void setBoardCell(SPFiarGame* src, int row, int col, char symbol) {
	if ((void*)src == NULL) {
		return;
	}

	(src->gameBoard)[row][col] = symbol;
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col) {
	if ((void*)src == NULL || !(col >= 0 && col < SP_FIAR_GAME_N_COLUMNS)) {
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}

	if (!((src->tops)[col] < SP_FIAR_GAME_N_ROWS)) {
		return SP_FIAR_GAME_INVALID_MOVE;
	}
	
	setBoardCell(src, (src->tops)[col], col, spFiarGameGetCurrentPlayer(src));
	(src->tops)[col]++;
	forceSpArrayListAddLast(src->history, col);
	changePlayer(src);

	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
	if ((void*)src == NULL || !(col >= 0 && col < SP_FIAR_GAME_N_COLUMNS)) {
		return false;
	}

	return  (src->tops)[col] < SP_FIAR_GAME_N_ROWS;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	int col, row;

	if ((void*)src == NULL) {
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}

	if (spArrayListIsEmpty(src->history)) {
		return SP_FIAR_GAME_NO_HISTORY;
	}

	col = spArrayListGetLast(src->history);
	spArrayListRemoveLast(src->history);

	((src->tops)[col])--;
	row = (src->tops)[col];
	setBoardCell(src, row, col, SP_FIAR_GAME_EMPTY_ENTRY);

	changePlayer(src);

	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src) {
	int i, j;
	if ((void*)src == NULL) {
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}

	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--) {
		printf("| ");
		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			printf("%c ", (src->gameBoard)[i][j]);
		}
		printf("|\n");
	}

	printf("-----------------\n");
	printf("  1 2 3 4 5 6 7  \n");

	return SP_FIAR_GAME_SUCCESS;
}

char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
	if ((void*)src == NULL || 
		(src->currentPlayer != SP_FIAR_GAME_PLAYER_1_SYMBOL && src->currentPlayer != SP_FIAR_GAME_PLAYER_2_SYMBOL)) {
		return SP_FIAR_GAME_EMPTY_ENTRY;
	}

	return src->currentPlayer;
}

/*
* Checks if there is a FIAR of the input symbol in the game board as a row.
* Assumes src is not null.
*
* @param src the game
* @param symbol the symbol to check (i.e the symbol of the player)
@return true iff the user with the specified symbol is a winner
*/
static bool checkWinAsRow(SPFiarGame* src, char symbol) {
	int count, i, j;

	// check for a win as a row
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		count = 0;

		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if ((src->gameBoard)[i][j] == symbol) {
				count++;

				if (count == SP_FIAR_GAME_SPAN) {
					return true;
				}
			}
			else {
				count = 0;
			}
		}
	}

	return false;
}

/*
* Checks if there is a FIAR of the input symbol in the game board as a col.
* Assumes src is not null.
*
* @param src the game
* @param symbol the symbol to check (i.e the symbol of the player)
@return true iff the user with the specified symbol is a winner
*/
static bool checkWinAsCol(SPFiarGame* src, char symbol) {
	int count, i, j;

	// check for a win as a col
	for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
		count = 0;

		for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
			if ((src->gameBoard)[i][j] == symbol) {
				count++;

				if (count == SP_FIAR_GAME_SPAN) {
					return true;
				}
			}
			else {
				count = 0;
			}
		}
	}

	return false;
}

/*
* Checks if there is a FIAR of the input symbol in the game board as a diag.
* Assumes src is not null.
*
* @param src the game
* @param symbol the symbol to check (i.e the symbol of the player)
@return true iff the user with the specified symbol is a winner
*/
static bool checkWinAsDiag(SPFiarGame* src, char symbol) {
	int count, i, j, k;

	// check for a win as a row as a diagonal of type / 
	//checking the diagonals start with the first column
	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--) {
		count = 0;

		// go diagonaly right up
		for (j = 0, k = i; j < SP_FIAR_GAME_N_COLUMNS && k < SP_FIAR_GAME_N_ROWS; j++, k++) {
			if ((src->gameBoard)[k][j] == symbol) {
				count++;

				if (count == SP_FIAR_GAME_SPAN) 
					return true;
			}
			else 
				count = 0;
		}
	}

	// checking the diagonals start with the bottom row
	for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
		count = 0;

		// go diagonaly right up
		for (i = 0, k = j; i < SP_FIAR_GAME_N_ROWS && k < SP_FIAR_GAME_N_COLUMNS; k++, i++) {
			if ((src->gameBoard)[i][k] == symbol) {
				count++;

				if (count == SP_FIAR_GAME_SPAN)
					return true;
			}
			else
				count = 0;
		}
	}

	// check for a win as a row as a diagonal of type '\', checking the diagonals start with the last column
	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--) {
		count = 0;

		// go diagonaly left up
		for (j = SP_FIAR_GAME_N_COLUMNS - 1, k = i; j >= 0 && k < SP_FIAR_GAME_N_ROWS; j--, k++) {
			if ((src->gameBoard)[k][j] == symbol) {
				count++;

				if (count == SP_FIAR_GAME_SPAN) 
					return true;
			}
			else 
				count = 0;
		}
	}

	// checking the diagonals start with the bottom row
	for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
		count = 0;

		// go diagonaly left up
		for (i = 0, k = j; i < SP_FIAR_GAME_N_ROWS && k >= 0; k--, i++) {
			if ((src->gameBoard)[i][k] == symbol) {
				count++;

				if (count == SP_FIAR_GAME_SPAN) 
					return true;
			}
			else 
				count = 0;
		}
	}

	return false;
}

/*
* Checks if there is a FIAR of the input symbol in the game board.
* Assumes src is not null.
*
* @param src the game 
* @param symbol the symbol to check (i.e the symbol of the player)
@return true iff the user with the specified symbol is a winner
*/
static bool isWinnerWithSymbol(SPFiarGame* src, char symbol) {
	
	if (checkWinAsRow(src, symbol))
		return true;

	if (checkWinAsCol(src, symbol))
		return true;

	if (checkWinAsDiag(src, symbol))
		return true;

	return false;
}

char spFiarCheckWinner(SPFiarGame* src) {
	int i;

	if ((void*)src == NULL) {
		return '\0';
	}

	if (isWinnerWithSymbol(src, SP_FIAR_GAME_PLAYER_1_SYMBOL)) {
		return SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}

	if (isWinnerWithSymbol(src, SP_FIAR_GAME_PLAYER_2_SYMBOL)) {
		return SP_FIAR_GAME_PLAYER_2_SYMBOL;
	}

	// check for tie - no more moves
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS && !spFiarGameIsValidMove(src, i); i++);

	if (i == SP_FIAR_GAME_N_COLUMNS) {
		return SP_FIAR_GAME_TIE_SYMBOL;
	}

	return '\0';
}
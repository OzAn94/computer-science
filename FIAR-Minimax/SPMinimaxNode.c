#include "SPMinimaxNode.h"
#include <string.h>

SPMinimaxNode* spMinimaxNodeCreate(int move, SP_MINIMAX_NODE_TYPE type, SP_PlayerA player_A_identity) {
	int i;
	SPMinimaxNode* node;
	
	if (!(move >= ROOT_NO_MOVE && move < SP_FIAR_GAME_N_COLUMNS)) {
		return NULL;
	}

	node = (SPMinimaxNode*)(malloc(sizeof(SPMinimaxNode)));

	if ((void*)node == NULL) {
		return NULL;
	}
	
	if ((void*)(node->children) == NULL) {
		free(node);
		return NULL;
	}

	node->move = move;
	node->type = type;
	
	// set 0 children
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		*(node->children + i) = NULL;
	}

	node->score_defined = false;
	node->player_A_identity = player_A_identity;

	return node;
}

void spMinimaxNodeDestroy(SPMinimaxNode* node) {
	if ((void*)node == NULL) {
		return;
	}

	free(node);
}

/**
*  For node of type max returns min, and the opposite.
*  Suppose node != null
*  @param node - the node
*  @return
*  SP_MINIMAX_NODE_TYPE - the opposite type
*/
static SP_MINIMAX_NODE_TYPE getOppositeType(SPMinimaxNode* node) {
	if (node->type == MAX_NODE) {
		return MIN_NODE;
	}

	return MAX_NODE;
}

SP_MINIMAX_NODE_MESSAGE spBuildNodeSubtree(SPMinimaxNode* node, SPFiarGame* game, unsigned int depth) {
	int i;
	SPMinimaxNode* child_node;
	SP_MINIMAX_NODE_MESSAGE msg;

	if ((void*)node == NULL || (void*)game == NULL || depth == 0) {
		return SP_MINIMAX_NODE_INVALID_ARGUMENT;
	}

	// set the node move
	if (node->move != ROOT_NO_MOVE) {
		spFiarGameSetMove(game, node->move);
	}

	// check if the game has ended
	if (spFiarCheckWinner(game) != NO_WINNER) {
		spFiarGameUndoPrevMove(game);
		return SP_MINIMAX_NODE_SUCCESS;
	}

	// valid node and depth > 0
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (spFiarGameIsValidMove(game, i)) {
			
			child_node = spMinimaxNodeCreate(i, getOppositeType(node), node->player_A_identity);

			if ((void*)child_node == NULL) 
				return SP_MINIMAX_NODE_MEM_ERR;

			// give birth to the child
			*(node->children + i) = child_node;

			// recurse
			msg = spBuildNodeSubtree(child_node, game, depth - 1);

			if (msg == SP_MINIMAX_NODE_MEM_ERR) {
				spMinimaxSubtreeDestroy(child_node);
				return SP_MINIMAX_NODE_MEM_ERR;
			}
		}
	}
	
	// undo move
	if (node->move != ROOT_NO_MOVE) {
		spFiarGameUndoPrevMove(game);
	}

	return SP_MINIMAX_NODE_SUCCESS;
}

void spMinimaxSubtreeDestroy(SPMinimaxNode* node) {
	int i;

	if ((void*)node == NULL) {
		return;
	}

	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if ((void*)(*(node->children + i)) != NULL) {
			spMinimaxSubtreeDestroy(*(node->children + i));
		}
	}

	spMinimaxNodeDestroy(node);
}

int spCalculateNodeScore(SPMinimaxNode* node, SPFiarGame* game) {
	int val, i, nextval; // index
	
	if ((void*)node == NULL || (void*)game == NULL) {
		return 0;
	}

	// set the node move
	if (node->move != ROOT_NO_MOVE) {
		spFiarGameSetMove(game, node->move);
	}

	if (isLeaf(node)) {
		val = spCalculateLeafScore(node, game);

		// undo move
		if (node->move != ROOT_NO_MOVE) {
			spFiarGameUndoPrevMove(game);
		}

		return val;
	}

	// get to the first non-null child
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS && (void*)(*(node->children + i)) == NULL; i++);

	val = spCalculateNodeScore(*(node->children + i), game);
	//index = i;

	if (node->type == MAX_NODE) {
		for (i = i + 1; i < SP_FIAR_GAME_N_COLUMNS; i++) {
			if ((void*)(*(node->children + i)) != NULL) {
				nextval = spCalculateNodeScore(*(node->children + i), game);
				if (nextval > val) {
					val = nextval;
					//index = i;
				}
			}
		}
	}

	else { // min node
		for (i = i + 1; i < SP_FIAR_GAME_N_COLUMNS; i++) {
			if ((void*)(*(node->children + i)) != NULL) {
				nextval = spCalculateNodeScore(*(node->children + i), game);
				if (nextval < val) {
					val = nextval;
					//index = i;
				}
			}
		}
	}

	node->score = val;
	node->score_defined = true;

	// undo move
	if (node->move != ROOT_NO_MOVE) {
		spFiarGameUndoPrevMove(game);
	}

	return val;
}

/*
*  Gets the value of the given characer.
*  @param symbol the character
*  @return 
*  1 for Player1 character, -1 for Player2 character, 0 o/w
*/
static int spGetSymbolValue(char symbol) {
	if (symbol == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		return 1;
	}

	if (symbol == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
		return -1;
	}

	return 0;
}

/*
*  Calculates the minimax algorithm value of the given histogram.
*  Assume histogram is of length SIZE_OF_HISTOGRAM.
*  @param histogram - the histogram
*  @param playerA_identity - the identity of player A
*  @return
*  The minimax algorithm value of the given histogram.
*/
static int spCalculateValFromHistogram(int histogram[SIZE_OF_HISTOGRAM], SP_PlayerA playerA_identity) {
	int weights[] = WEIGHTS, result_arr[(SP_FIAR_GAME_SPAN - 1) * 2];
	int i, result = 0;

	/*
	// -4 score was found
	if (histogram[0] > 0) {
		if (playerA_identity == Player1) {
			return INT_MIN;
		}

		return INT_MAX;
	}

	
	// 4 score was found
	if (histogram[SIZE_OF_HISTOGRAM - 1] > 0) {
		if (playerA_identity == Player1) {
			return INT_MAX;
		}

		return INT_MIN;
	}
	*/

	// get the relevant results from the histogram
	for (i = 0; i < (SP_FIAR_GAME_SPAN - 1) * 2; i++) {
		if (i < SP_FIAR_GAME_SPAN - 1) {
			result_arr[i] = histogram[i + 1];
		}
		else {
			result_arr[i] = histogram[i + 2];
		}
	}

	for (i = 0; i < (SP_FIAR_GAME_SPAN - 1) * 2; i++) {
		result += weights[i] * result_arr[i];
	}

	if (playerA_identity == Player2) {
		result = -1 * result;
	}

	return result;
}

/*
Populates the histogram according to all of the column spans.
@param histogram - the histogram (assumes of SIZE_OF_HISTOGRAM length)
@param game - the game
*/
static void populateHistogramWithColSpans(int histogram[], SPFiarGame* game) {
	int span_val, i, j, m;

	// check col spans
	for (j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {

		for (i = 0; i < SP_FIAR_GAME_N_ROWS - SP_FIAR_GAME_SPAN + 1; i++) {
			span_val = 0;

			for (m = 0; m < SP_FIAR_GAME_SPAN; m++) {
				span_val += spGetSymbolValue(game->gameBoard[i + m][j]);
			}

			histogram[span_val + SP_FIAR_GAME_SPAN]++;
		}
	}
}

/*
Populates the histogram according to all of the rows spans.
@param histogram - the histogram (assumes of SIZE_OF_HISTOGRAM length)
@param game - the game
*/
static void populateHistogramWithRowSpans(int histogram[], SPFiarGame* game) {
	int span_val, i, j, m;

	// check rows spans
	for (i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {

		for (j = 0; j < SP_FIAR_GAME_N_COLUMNS - SP_FIAR_GAME_SPAN + 1; j++) {
			span_val = 0;

			for (m = 0; m < SP_FIAR_GAME_SPAN; m++) {
				span_val += spGetSymbolValue(game->gameBoard[i][j + m]);
			}

			histogram[span_val + SP_FIAR_GAME_SPAN]++;
		}
	}
}

/*
Populates the histogram according to all of the left diagonal spans.
@param histogram - the histogram (assumes of SIZE_OF_HISTOGRAM length)
@param game - the game
*/
static void populateHistogramWithLeftDiagSpans(int histogram[], SPFiarGame* game) {
	int span_val, i, j, k, m;

	// check diagonals of type '\'
		// checking the diagonals start with the last column
	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--) {

		// go diagonaly left up
		for (j = SP_FIAR_GAME_N_COLUMNS - 1, k = i; j >= 0 && k < SP_FIAR_GAME_N_ROWS; j--, k++) {
			// check if the span is within range
			if (j - (SP_FIAR_GAME_SPAN - 1) >= 0 && k + SP_FIAR_GAME_SPAN - 1 < SP_FIAR_GAME_N_ROWS) {
				span_val = 0;

				for (m = 0; m < SP_FIAR_GAME_SPAN; m++) {
					span_val += spGetSymbolValue(game->gameBoard[k + m][j - m]);
				}

				histogram[span_val + SP_FIAR_GAME_SPAN]++;
			}
		}
	}

	// checking the diagonals start with the bottom row
	for (j = 0; j < SP_FIAR_GAME_N_COLUMNS - 1; j++) {

		// go diagonaly left up
		for (i = 0, k = j; i < SP_FIAR_GAME_N_ROWS && k >= 0; k--, i++) {
			// check if the span is within range
			if (k - (SP_FIAR_GAME_SPAN - 1) >= 0 && i + SP_FIAR_GAME_SPAN - 1 < SP_FIAR_GAME_N_ROWS) {
				span_val = 0;

				for (m = 0; m < SP_FIAR_GAME_SPAN; m++) {
					span_val += spGetSymbolValue(game->gameBoard[i + m][k - m]);
				}

				histogram[span_val + SP_FIAR_GAME_SPAN]++;
			}
		}
	}
}

/*
Populates the histogram according to all of the right diagonal spans.
@param histogram - the histogram (assumes of SIZE_OF_HISTOGRAM length)
@param game - the game
*/
static void populateHistogramWithRightDiagSpans(int histogram[], SPFiarGame* game) {
	int span_val, i, j, k, m;

	// check diagonals of type /
	// checking the diagonals start with the first column
	for (i = SP_FIAR_GAME_N_ROWS - 1; i >= 0; i--) {

		// go diagonaly right up
		for (j = 0, k = i; j < SP_FIAR_GAME_N_COLUMNS && k < SP_FIAR_GAME_N_ROWS; j++, k++) {
			// check if there is k + 3, j + 3 is within range
			if (j + SP_FIAR_GAME_SPAN - 1 < SP_FIAR_GAME_N_COLUMNS && k + SP_FIAR_GAME_SPAN - 1 < SP_FIAR_GAME_N_ROWS) {
				span_val = 0;

				for (m = 0; m < SP_FIAR_GAME_SPAN; m++) {
					span_val += spGetSymbolValue(game->gameBoard[k + m][j + m]);
				}

				histogram[span_val + SP_FIAR_GAME_SPAN]++;
			}
		}
	}

	// checking the diagonals start with the bottom row (except first column - already passed)
	for (j = 1; j < SP_FIAR_GAME_N_COLUMNS; j++) {

		// go diagonaly right up
		for (i = 0, k = j; i < SP_FIAR_GAME_N_ROWS && k < SP_FIAR_GAME_N_COLUMNS; k++, i++) {
			// check if there is k + 3, j + 3 is within range
			if (i + SP_FIAR_GAME_SPAN - 1 < SP_FIAR_GAME_N_ROWS && k + SP_FIAR_GAME_SPAN - 1 < SP_FIAR_GAME_N_COLUMNS) {
				span_val = 0;

				for (m = 0; m < SP_FIAR_GAME_SPAN; m++) {
					span_val += spGetSymbolValue(game->gameBoard[i + m][k + m]);
				}

				histogram[span_val + SP_FIAR_GAME_SPAN]++;
			}
		}
	}
}

int spCalculateLeafScore(SPMinimaxNode* leaf, SPFiarGame* game) {
	int i, val; 
	int histogram[SIZE_OF_HISTOGRAM]; // -4, -3, -2, -1, 0, 1, 2, 3, 4
	char winner;
	
	if ((void*)leaf == NULL || (void*)game == NULL || !isLeaf(leaf)) 
		return 0;

	// check if the game has ended
	switch (winner = spFiarCheckWinner(game)) {

	case SP_FIAR_GAME_TIE_SYMBOL:
		val = 0;
		break;

	case SP_FIAR_GAME_PLAYER_1_SYMBOL:
		if (leaf->player_A_identity == Player1) 
			val = INT_MAX;
		else 
			val = INT_MIN;
		break;

	case SP_FIAR_GAME_PLAYER_2_SYMBOL:
		if (leaf->player_A_identity == Player1) 
			val = INT_MIN;
		else 
			val = INT_MAX;
		break;

	default:
		break;
	}

	// return if the game has ended
	if (winner != NO_WINNER) {
		leaf->score = val;
		leaf->score_defined = true;

		return val;
	}

	// set array to zero
	for (i = 0; i < SIZE_OF_HISTOGRAM; i++) {
		histogram[i] = 0;
	}

	populateHistogramWithColSpans(histogram, game);
	populateHistogramWithRowSpans(histogram, game);
	populateHistogramWithRightDiagSpans(histogram, game);
	populateHistogramWithLeftDiagSpans(histogram, game);
		
	val = spCalculateValFromHistogram(histogram, leaf->player_A_identity);

	leaf->score = val;
	leaf->score_defined = true;

	return val;
}

bool isLeaf(SPMinimaxNode* node) {
	int i;

	if ((void*)node == NULL) {
		return false;
	}

	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if ((void*)(*(node->children + i)) != NULL) {
			return false;
		}
	}
	
	return true;
}

int spGetMinimaxBestMove(SPMinimaxNode* node, SPFiarGame* game) {
	int i;

	if ((void*)node == NULL || (void*)game == NULL || !(node->score_defined)) {
		return -1;
	}

	// check if game already ended
	if (spFiarCheckWinner(game) != NO_WINNER) {
		return -1;
	}

	// get index to the desired position
	for (i = 0; i < SP_FIAR_GAME_N_COLUMNS &&
		((void*)(*(node->children + i)) == NULL || (*(node->children + i))->score != node->score); i++);

	return i;
}
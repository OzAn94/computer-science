#include "SPMainAux.h"

/**
*  Gets a command from the user and removes trailing \n.
*  @param cmd the command
*  @return the command without trailing \n
*/
static char* getCommandFromUserStripped(char * cmd) {
	int i;

	if (fgets(cmd, SP_MAX_LINE_LENGTH + 1, stdin) == NULL) {
		return NULL;
	}

	for (i = 0; i < SP_MAX_LINE_LENGTH + 1 && cmd[i] != NULL_CHARACTER; i++) {
		if (cmd[i] == '\n') {
			cmd[i] = NULL_CHARACTER;
		}
	}

	return cmd;
}


/*
Called when user wants to undo his move.
Undoes 2 moves - the user move and the previous computer move.
@param game - the game
@return
false iff there's no history 
*/
static bool undoUserMove(SPFiarGame* game) {
	SP_FIAR_GAME_MESSAGE msg;

	int computer_last_move = spArrayListGetLast(game->history),
		user_last_move = spArrayListGetAt(game->history, spArrayListSize(game->history) - 2);

	msg = spFiarGameUndoPrevMove(game);

	if (msg == SP_FIAR_GAME_NO_HISTORY) {
		return false;
	}

	spFiarGameUndoPrevMove(game);

	printf("Remove disc: remove computer's disc at column %d\n", computer_last_move + 1);
	printf("Remove disc: remove user's disc at column %d\n", user_last_move + 1);

	spFiarGamePrintBoard(game);

	return true;
}

/*
Called when user wants to undo his move after he won.
@param game - the game
@return
false iff there's no history
*/
static bool undoOnlyUserMove(SPFiarGame* game) {
	SP_FIAR_GAME_MESSAGE msg;

	int user_last_move = spArrayListGetLast(game->history);

	msg = spFiarGameUndoPrevMove(game);

	if (msg == SP_FIAR_GAME_NO_HISTORY) {
		return false;
	}

	printf("Remove disc: remove user's disc at column %d\n", user_last_move + 1);

	spFiarGamePrintBoard(game);

	return true;
}

/*
Called when the user adds a disc. Prints the relevant error message if an error occures.
@param game - the game
@param col - the column to put disc in
@return 
true iff disc successfully added to col
*/
static bool userAddsDisc(SPFiarGame* game, int col) {

	if (!(col >= 0 && col < SP_FIAR_GAME_N_COLUMNS)) {
		error(ADD_DISC_NUM_ERR, NULL, 0);
		return false;
	}

	// column full
	if (spFiarGameSetMove(game, col) == SP_FIAR_GAME_INVALID_MOVE) {
		error(ADD_DISC_FULL_COLUMN_ERR, NULL, col + 1);
		return false;
	}

	return true;
}

/*
Called when the Computer adds a disc. Prints the relevant error message if an error occures.
@param game - the game
@param col - the column to put disc in
@return
true iff disc successfully added to col
*/
static bool addComputerDisc(SPFiarGame* game, unsigned int level) {
	int move;

	if ((move = spMinimaxSuggestMove(game, level)) == -1) {
		error(MEM_ERR, MALLOC, 0);
		return false;
	};

	spFiarGameSetMove(game, move);

	printf("Computer move: add disc to column %d\n", move + 1);

	return true;
}


/*
Handles quit event.
@param game - the game
*/
static void quit(SPFiarGame* game) {
	spFiarGameDestroy(game);
	printf("Exiting...\n");
}

/*
Handles restart event.
@param game - the game
*/
static void restart_game(SPFiarGame* game) {
	spFiarGameDestroy(game);
	printf("Game restarted!\n");
}

/*
Called when user want a move suggestion.
@param game - the game
@param level - the level of the game
@return
the suggested move or -1 if error occured
*/
static int suggestMoveToUser(SPFiarGame* game, unsigned int level) {
	int move;

	if ((move = spMinimaxSuggestMove(game, level)) == -1) {
		error(MEM_ERR, MALLOC, 0);
		return move;
	};

	printf("Suggested move: drop a disc to column %d\n", move + 1);

	return move;
}


unsigned int init() {
	char cmd[SP_MAX_LINE_LENGTH + 1], *token;
	unsigned int level;

	while (true) {
		printf("Please enter the difficulty level between [1-7]:\n");

		if (getCommandFromUserStripped(cmd) == NULL) {
			error(CMD_INVALID_ERR, NULL, 0);
			return EXIT;
		}

		if (spParserIsInt(cmd)) {
			token = strtok(cmd, SEP);
			level = atoi(token);

			if (!(level >= MIN_LEVEL && level <= MAX_LEVEL)) {
				error(INIT_ERR, NULL, 0);
			}

			else {
				return level;
			}
		}

		else {
			if (spParserPraseLine(cmd).cmd == SP_QUIT) {
				quit(NULL);
				return EXIT;
			}

			error(CMD_INVALID_ERR, NULL, 0);
		}
	}
}

/*
Called when the game has ended and handles it.
@param game - the game
@param winner - the winner
@return
QUIT_GAME if the user wants to finish the game, 
RESTART if the user wants to restart, 
CONTINUE_GAME_MOVE_UNDONE if the user undone it's previous move
*/
static GAME_HAS_ENDED_MESSAGE gameHasEndedEvent(SPFiarGame* game, char winner) {
	char cmd[SP_MAX_LINE_LENGTH + 1];
	SPCommand cmd_parsed;

	printf("Please enter 'quit' to exit or 'restart' to start a new game!\n");

	do {

		if (getCommandFromUserStripped(cmd) == NULL) {
			error(CMD_INVALID_ERR, NULL, 0);
			return QUIT_GAME;
		}

		cmd_parsed = spParserPraseLine(cmd);

		switch (cmd_parsed.cmd)
		{
		case SP_QUIT:
			return QUIT_GAME;
		case SP_RESTART:
			return RESTART;
		case SP_UNDO_MOVE:
			if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL) { // the user won
				if (!undoOnlyUserMove(game)) {
					error(UNDO_MOVE_ERR, NULL, 0);
					return QUIT_GAME;
				}
			}

			else { // computer won or tie
				if (!undoUserMove(game)) {
					error(UNDO_MOVE_ERR, NULL, 0);
					return QUIT_GAME;
				}
			}

			return CONTINUE_GAME_MOVE_UNDONE;
		case SP_ADD_DISC:
		case SP_SUGGEST_MOVE:
			error(GAME_OVER_ERR, NULL, 0);
			break;
		default:
			error(CMD_INVALID_ERR, NULL, 0);
			break;
		}

	} while (true);
}

/*
Prints the relevant message when the game has ended.
@param winner - the winner symbol
*/
static void printGameHasEndedMessage(char winner) {
	if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		printf("Game over: you win\n");
	}

	else if (winner == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
		printf("Game over: computer wins\n");
	}

	// tie
	else {
		printf("Game over: it's a tie\n");
	}
}

/*
Handles a parsed user commands.
@param cmd_parsed - the parsed command
@param game - the game
@param level - the level
@param winner - the winner status
@return
A struct contains the winner status and a message.
*/
SP_WINNER_AND_MSG handleUserCommand(SPCommand cmd_parsed, SPFiarGame* game, unsigned int level, char winner) {
	SP_WINNER_AND_MSG ret_struct;

	ret_struct.winner = winner;
	ret_struct.msg = CONTINUE;

	switch (cmd_parsed.cmd)
	{
	case SP_UNDO_MOVE:
		if (!undoUserMove(game))
			error(UNDO_MOVE_ERR, NULL, 0);
		else
			printf(MAKE_NEXT_MOVE_STRING);
		break;
	case SP_QUIT:
		quit(game);
		ret_struct.msg = QUIT_GAME;
		break;
	case SP_RESTART:
		restart_game(game);
		ret_struct.msg = RESTART;
		break;
	case SP_ADD_DISC:
		// error occured while adding disc 
		if (!userAddsDisc(game, cmd_parsed.arg - 1))
			break;

		if ((ret_struct.winner = spFiarCheckWinner(game)) == NULL_CHARACTER) {
			if (!addComputerDisc(game, level)) {
				spFiarGameDestroy(game);
				ret_struct.msg = QUIT_GAME;
				break;
			}
		}

		// user didn't win, maybe computer won
		if (ret_struct.winner == NO_WINNER)
			ret_struct.winner = spFiarCheckWinner(game);

		spFiarGamePrintBoard(game);

		// still no winner
		if (ret_struct.winner == NO_WINNER)
			printf(MAKE_NEXT_MOVE_STRING);
		break;
	case SP_SUGGEST_MOVE:
		if (suggestMoveToUser(game, level) == -1) {
			spFiarGameDestroy(game);
			ret_struct.msg = QUIT_GAME;
			break;
		}
		break;
	default:
		error(CMD_INVALID_ERR, NULL, 0);
		break;
	}

	return ret_struct;
}

GAME_HAS_ENDED_MESSAGE run_game(unsigned int level) {
	SPFiarGame* game = spFiarGameCreate(HISTORY_SIZE);
	char winner, cmd[SP_MAX_LINE_LENGTH + 1];
	SPCommand cmd_parsed;
	GAME_HAS_ENDED_MESSAGE msg;
	SP_WINNER_AND_MSG win_msg_struct;

	if ((void*)game == NULL) {
		error(MEM_ERR, MALLOC, 0);
		return QUIT_GAME;
	}

	spFiarGamePrintBoard(game);

	do {
		winner = NO_WINNER;
		printf(MAKE_NEXT_MOVE_STRING);

		do {
			if (getCommandFromUserStripped(cmd) == NULL) {
				spFiarGameDestroy(game);
				error(CMD_INVALID_ERR, NULL, 0);
				return QUIT_GAME;
			}

			cmd_parsed = spParserPraseLine(cmd);

			win_msg_struct = handleUserCommand(cmd_parsed, game, level, winner);

			winner = win_msg_struct.winner;

			if (win_msg_struct.msg != CONTINUE)
				return win_msg_struct.msg;

		} while (winner == NULL_CHARACTER);

		printGameHasEndedMessage(winner);
	
	} while ((msg = gameHasEndedEvent(game, winner)) == CONTINUE_GAME_MOVE_UNDONE);

	if (msg == RESTART) 
		restart_game(game);

	else if (msg == QUIT_GAME) 
		quit(game);

	return msg;
}

void error(SP_ERROR_TYPE err, const char* bad_func_name, int col) {
	switch (err)
	{
	case MEM_ERR:
		printf("Error: %s has failed", bad_func_name);
		break;
	case INIT_ERR:
		printf("Error: invalid level (should be between 1 to 7)\n");
		break;
	case CMD_INVALID_ERR:
		printf("Error: invalid command\n");
		break;
	case ADD_DISC_NUM_ERR:
		printf("Error: column number must be in range 1-7\n");
		break;
	case ADD_DISC_FULL_COLUMN_ERR:
		printf("Error: column %d is full\n", col);
		break;
	case UNDO_MOVE_ERR:
		printf("Error: cannot undo previous move!\n");
		break;
	case GAME_OVER_ERR:
		printf("Error: the game is over\n");
		break;
	default:
		break;
	}
}
#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPFIARParser.h"
#include "SPMinimax.h"
#include "SPFIARParserAdditionalHeaders.h"

#define MIN_LEVEL 1
#define MAX_LEVEL 7
#define EXIT 0
#define NULL_CHARACTER '\0'
#define NO_WINNER '\0'
#define MALLOC "malloc"
#define HISTORY_SIZE 20
#define MAKE_NEXT_MOVE_STRING "Please make the next move:\n"

/*
SPMainAux summary:
	This module contains all the relevant function for handling the game run.

	init - Handles the initialization of the game.
	error - Handles errors which occure during the game. Prints the relevant message.
	run_game - Runs the game with the desired level.
	
*/

/* enum used for errors */
typedef enum t_error_type {
	MEM_ERR,
	INIT_ERR,
	CMD_INVALID_ERR,
	ADD_DISC_NUM_ERR,
	ADD_DISC_FULL_COLUMN_ERR,
	UNDO_MOVE_ERR,
	GAME_OVER_ERR
} SP_ERROR_TYPE ;

/* enum used to indicate different return types */
typedef enum t_game_has_ended_message {
	CONTINUE_GAME_MOVE_UNDONE,
	QUIT_GAME,
	RESTART,
	CONTINUE
} GAME_HAS_ENDED_MESSAGE;

/*
Struct contains the winner status and relevant message.
*/
typedef struct t_winner_and_message {
	char winner;
	GAME_HAS_ENDED_MESSAGE msg;
} SP_WINNER_AND_MSG;

//put auxiliary functions and constants used by the main function here.

/* 
Handles the initialization of the game.
@return
the game level.
*/
unsigned int init();

/*
Handles errors which occure during the game. Prints the relevant message.
@param err - the error type
@param bad_fund_name - optional, the name of the system function that caused error
@param col - optional, the index of the column that is full while trying to add disc to
*/
void error(SP_ERROR_TYPE err, const char* bad_func_name, int col);

/*
Runs the game with the desired level.
@param level - the game level
@return
QUIT_GAME - if the user wants to quit
RESTART - if the user wants to restart the game
*/
GAME_HAS_ENDED_MESSAGE run_game(unsigned int level);

#endif

#include "SPFIARParser.h"
#include "SPFIARParserAdditionalHeaders.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

/*
* Checks if the string is an integer number.
* Assumes str is of length 1024 at most, and str is not NULL.
*
* @return true iff the string represents an integer number.
*/
static bool isIntegerNumber(const char* str) {
	int i = 0;
	
	// check first digit
	if (!isdigit(str[i])) {
		if (str[i] != '-' && str[i] != '+')
			return false;

		// negative number or starts with +
		if (!isdigit(str[++i]))
			return false;
	}
		
	for (; i < SP_MAX_LINE_LENGTH; i++) {
		if (str[i] == '\0')
			return true;
		if (!isdigit(str[i]))
			return false;
	}

	// no null terminator
	return false;
}

bool spParserIsInt(const char* str) {
	char str_copied[SP_MAX_LINE_LENGTH + 1], *token;
	long long result;

	if (str == NULL) {
		return false;
	}

	// copy the line to prevent a case where str is not null terminated
	strncpy(str_copied, str, SP_MAX_LINE_LENGTH);
	str_copied[SP_MAX_LINE_LENGTH] = '\0';

	// get the number token
	token = strtok(str_copied, SEP);

	if (token == NULL) {
		return false;
	}

	if (!isIntegerNumber(token))
		return false;

	// see that there are no more token
	token = strtok(NULL, SEP);

	if (token != NULL) {
		return false;
	}

	// check if the number can be converted to long
	result = strtoll(str, NULL, 10);
	
	// if the number is 0, the check isn't ok
	//if (!result)
	//	return false;

	// the number is not in the integer range
	if (!(INT_MIN <= result && result <= INT_MAX))
		return false; 

	return true;
}

SPCommand spParserPraseLine(const char* str) {
	SPCommand sp_cmd;
	char line[SP_MAX_LINE_LENGTH + 1], *token;
	int param;

	// set to invalid line by default
	sp_cmd.cmd = SP_INVALID_LINE;
	sp_cmd.validArg = false;

	// check if the string is null
	if (str == NULL) {
		return sp_cmd;
	}

	// copy the line to prevent a case where str is not null terminated
	strncpy(line, str, SP_MAX_LINE_LENGTH);
	line[SP_MAX_LINE_LENGTH] = '\0';

	// get the first token - command string
	token = strtok(line, SEP);

	if (token == NULL) {
		return sp_cmd;
	}

	// switch-case
	if (strcmp(token, ADD_DISC) == 0) {
		token = strtok(NULL, SEP);

		// check if second token is valid param
		if ((token != NULL) && (param = spParserIsInt(token))) {
			sp_cmd.validArg = true;
			sp_cmd.arg = atoi(token);
			sp_cmd.cmd = SP_ADD_DISC;
		}

		else {
			return sp_cmd;
		}
	}
	else if (strcmp(token, SUGGEST_MOVE) == 0) {
		sp_cmd.cmd = SP_SUGGEST_MOVE;
	}
	else if (strcmp(token, UNDO_MOVE) == 0) {
		sp_cmd.cmd = SP_UNDO_MOVE;
	}
	else if (strcmp(token, QUIT) == 0) {
		sp_cmd.cmd = SP_QUIT;
	}
	else if (strcmp(token, RESTART_GAME) == 0) {
		sp_cmd.cmd = SP_RESTART;
	}

	// check if the string has ended
	token = strtok(NULL, SEP);

	if (token != NULL) {
		sp_cmd.cmd = SP_INVALID_LINE;
		sp_cmd.validArg = false;
	}

	return sp_cmd;
}
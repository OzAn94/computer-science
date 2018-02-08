#include "SPMinimax.h"
#include <string.h>
#include "SPMinimaxNode.h"

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	SP_PlayerA current_player;
	SPMinimaxNode *root;
	SPFiarGame* copied_game;
	int move;

	if ((void*)currentGame == NULL || maxDepth <= 0) 
		return -1;

	if (spFiarGameGetCurrentPlayer(currentGame) == SP_FIAR_GAME_PLAYER_1_SYMBOL) 
		current_player = Player1;

	else 
		current_player = Player2;

	// copy the current game and clean the history for space. Assumes history >= max depth
	copied_game = spFiarGameCopy(currentGame);

	if (copied_game == NULL) 
		return -1;

	while (spArrayListRemoveLast(copied_game->history) != SP_ARRAY_LIST_EMPTY);

	root = spMinimaxNodeCreate(ROOT_NO_MOVE, MAX_NODE, current_player);

	if ((void*)root == NULL) {
		spFiarGameDestroy(copied_game);
		return -1;
	}

	if (spBuildNodeSubtree(root, copied_game, maxDepth) != SP_MINIMAX_NODE_SUCCESS) {
		spFiarGameDestroy(copied_game);
		spMinimaxNodeDestroy(root);
		return -1;
	}

	spCalculateNodeScore(root, copied_game);
	move = spGetMinimaxBestMove(root, copied_game);

	spMinimaxSubtreeDestroy(root);
	spFiarGameDestroy(copied_game);

	return move;
}
#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#define NO_WINNER '\0'
#define SIZE_OF_HISTOGRAM 9
#define WEIGHTS { -5, -2, -1, 1, 2, 5 }
#define ROOT_NO_MOVE -1

#include "SPFIARGame.h"
#include <limits.h>
#include <stdlib.h>

//Put all decleartions and constants here

/** 
* SPMinimaxNode summary:
*
* A container represents a Minimax tree node. The node has score, type, move that it represents, 
* the identity of player A and pointers to the children.
*
* spMinimaxNodeCreate      - Creates a minimax node of the required type with associated game instance.
* spMinimaxNodeDestroy     - Destroys the minimax node and frees all memory associated with it.
* spBuildNodeSubtree       - Builds the minimax node subtree to the specified depth.
* spMinimaxSubtreeDestroy  - Destroys the subtree rooted in node.
* spCalculateNodeScore     - Calculates the score of the specified node.
* spCalculateLeafScore     - Calculates the score the specified leaf.
* isLeaf				   - Returns true iff the node is a leaf.
* spGetMinimaxBestMove      - Returns the best move for the game which the input node represents.
*/

/*
* The type of the node.
*/
typedef enum sp_minimax_node_type_t {
	MAX_NODE,
	MIN_NODE
} SP_MINIMAX_NODE_TYPE;

/*
*  Enum for player A symbol identity.
*/
typedef enum sp_player_A {
	Player1,
	Player2
} SP_PlayerA;

typedef struct sp_minimax_node_t {
	int score;
	bool score_defined;
	SP_MINIMAX_NODE_TYPE type;
	int move;
	struct sp_minimax_node_t *children[SP_FIAR_GAME_N_COLUMNS];
	SP_PlayerA player_A_identity;
	
} SPMinimaxNode;

/*
*  Enum for errors.
*/
typedef enum sp_minimax_node_message_t {
	SP_MINIMAX_NODE_SUCCESS,
	SP_MINIMAX_NODE_INVALID_ARGUMENT,
	SP_MINIMAX_NODE_MEM_ERR
} SP_MINIMAX_NODE_MESSAGE;

/**
*  Creates a minimax node of the required type with associated game instance.
*  @param game - the game to associate with the node
*  @param type - the type of the node
*  @param player_A_identity - the identity of player A
*  @return
*  NULL, if an allocation error occurred or game  == NULL.
*  The minimax node otherwise.
*/
SPMinimaxNode* spMinimaxNodeCreate(int move, SP_MINIMAX_NODE_TYPE type, SP_PlayerA player_A_identity);

/**
*  Destroys the minimax node and frees all memory associated with it.
*  If the node is NULL, nothing happens.
*  @param node - the node to destroy
*/
void spMinimaxNodeDestroy(SPMinimaxNode* node);

/**
*  Builds the minimax node subtree to the specified depth.
*  @param node - the node to build subtree to
*  @param depth - depth to recurse (0 means no more)
*  @param game - the game to make the node's move in 
*  @return
*  SP_MINIMAX_NODE_INVALID_ARGUMENT, if node == NULL or depth < 0
*  SP_MINIMAX_NODE_MEM_ERR if allocation fails
*  SP_MINIMAX_NODE_SUCCESS otherwise.
*/
SP_MINIMAX_NODE_MESSAGE spBuildNodeSubtree(SPMinimaxNode* node, SPFiarGame* game, unsigned int depth);

/**
*  Destroys the subtree rooted in node.
*  If the node is NULL, nothing happens.
*  @param node - the root of the subtree
*/
void spMinimaxSubtreeDestroy(SPMinimaxNode* node);

/**
*  Calculates the score of the specified node.
*  @param node - the node to calculate score for.
*  @param game - the game to make the node's move in
*  @return
*  undefined, if node == NULL
*  the score of the node otherwise.
*/
int spCalculateNodeScore(SPMinimaxNode* node, SPFiarGame* game);

/**
*  Calculates the score the specified leaf.
*  @param node - the node to calculate score for.
*  @param game - the game associated with the leaf
*  @return
*  undefined, if leaf == NULL or is not a real leaf
*  the score of the node otherwise.
*/
int spCalculateLeafScore(SPMinimaxNode* leaf, SPFiarGame* game);

/**
*  Returns true iff the node is a leaf.
*  @param node - the node to check
*  @return
*  true iff the node is a leaf 
*/
bool isLeaf(SPMinimaxNode* node);

/**
*  Returns the best move for the game which the input node represents.
*  This function has to be called after the tree nodes have scores calculated.
*  @param node - the root node
*  @param game - the root game
*  @return
*  The column number (0-based) of the best move if node->val_defined == True and node != NULL
*  and the game hasn't ended.
*  -1 otherwise
*/
int spGetMinimaxBestMove(SPMinimaxNode* root, SPFiarGame* game);

#endif
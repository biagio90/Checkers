#include "player.hpp"
#include <cstdlib>
#include <climits>
#define MAX_EVAL INT_MAX
#define MAX_TIME 400000
#include <stdio.h>
#include <math.h>

namespace checkers
{

int eval(GameState state, int depth);
int maxFunction(GameState state, int level, int alpha, int beta);
int minFunction(GameState state, int level, int alpha, int beta);
//float distanceAvg(GameState state);

/**
 *    col 0  1  2  3  4  5  6  7
 * row  -------------------------
 *  0  |     4     4     4     4 |  0
 *  1  |  4     3     3     3    |  1
 *  2  |     3     2     2     4 |  2
 *  3  |  4     2     1     3    |  3
 *  4  |     3     1     2     4 |  4
 *  5  |  4     2     2     3    |  5
 *  6  |     3     3     3     4 |  6
 *  7  |  4     4     4     4    |  7
 *      -------------------------
 *        0  1  2  3  4  5  6  7
 **/
 /*
int boardValue[32] = {    4, 4, 4, 4,
                        4, 3, 3, 3,
                            3, 2, 2, 4,
                        4, 2, 1, 3,
                            3, 1, 2, 4,
                        4, 2, 2, 3,
                            3, 3, 3, 4,
                        4, 4, 4, 4};

int boardValueR[32] = {    0, 0, 0, 0,
                        0, 0, 0, 0,
                            3, 2, 2, 4,
                        4, 2, 1, 3,
                            3, 1, 2, 4,
                        4, 2, 2, 3,
                            3, 3, 3, 4,
                        4, 4, 4, 4};

int boardValueW[32] = {    4, 4, 4, 4,
                        4, 3, 3, 3,
                            3, 2, 2, 4,
                        4, 2, 1, 3,
                            3, 1, 2, 4,
                        4, 2, 2, 3,
                            0, 0, 0, 0,
                        0, 0, 0, 0};

int boardValueK[32] = {    0, 0, 0, 0,
						0, 2, 2, 2,
							2, 2, 2, 0,
						0, 4, 4, 4,
							4, 4, 4, 0,
						0, 2, 2, 2,
							2, 2, 2, 0,
						0, 0, 0, 0};
*/
int boardValueAll[6][32]={
                       {    5, 5, 5, 5,
                        5, 5, 5, 5,
                            8, 7, 7, 9,
                        9, 7, 6, 8,
                            8, 6, 7, 9,
                        9, 7, 7, 8,
                            8, 8, 8, 9,
                        9, 9, 9, 9},

                        {    9, 9, 9, 9,
                        9, 8, 8, 8,
                            8, 7, 7, 9,
                        9, 7, 6, 8,
                            8, 6, 7, 9,
                        9, 7, 7, 8,
                            5, 5, 5, 5,
                        5, 5, 5, 5},

                        {    10, 10, 10, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 14, 14, 14,
							14, 14, 14, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 10, 10, 10},

						{    10, 10, 10, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 14, 14, 14,
							14, 14, 14, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 10, 10, 10},

						{    10, 10, 10, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 14, 14, 14,
							14, 14, 14, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 10, 10, 10},

						{    10, 10, 10, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 14, 14, 14,
							14, 14, 14, 10,
						10, 12, 12, 12,
							12, 12, 12, 10,
						10, 10, 10, 10}

};

int player;
GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0) return GameState(pState, Move());

    if(pState.getNextPlayer() == CELL_RED) player = CELL_RED;
    else                                   player = CELL_WHITE;

    int best=-MAX_EVAL, i_best=0;

    for(uint16_t i = 0; i < lNextStates.size(); i++)
    {
        int val = minFunction(lNextStates[i], 10, -MAX_EVAL, MAX_EVAL);
        if(val > best)
        {
            best = val;
            i_best = i;
        }
    }

    return lNextStates[i_best];
}

int eval(GameState state, int depth)
{
        if((player == CELL_WHITE && state.isWhiteWin()) || (player == CELL_RED && state.isRedWin()))
    return 10000*(depth);
    else if((player == CELL_RED && state.isWhiteWin()) || (player == CELL_WHITE && state.isRedWin()))
    return -10000*(depth);

    int pieces[7]={0,0,0,0,0,0,0}, k;

    for (int i = 1; i <= 32; ++i)
	{
        k = state.at(i);
        pieces[k] += boardValueAll[k-1][i-1];
    }


    if(player == CELL_WHITE)
    {
        return ((pieces[2] + pieces[6]) - (pieces[1] + pieces[5]));

    } else {
        return ((pieces[1] + pieces[5]) - (pieces[2] + pieces[6]));
    }

}
/*
float distanceAvg(GameState state)
{
    float avg = 0;
    for (int i = 1; i <= 32; ++i)
	{
        if (state.at(i) & CELL_RED)
        {
            for (int j = 1; j <= 32; ++j)
            {
                if (state.at(j) & CELL_WHITE)
                {

                    int rowR = (i-1)/4;
                    int rowW = (j-1)/4;
                    int colR = (i-1)%4;
                    int colW = (j-1)%4;
                    avg += sqrt((rowR-rowW)*(rowR-rowW)+(colR-colW)*(colR-colW));
                }
            }
        }
    }

    return avg;
}
*/

int maxFunction(GameState state, int level, int alpha, int beta)
{
    //if(level == 0) return eval(state);

    std::vector<GameState> lNextStates;
    state.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0) return eval(state, level);

    for(uint16_t i = 0; i < lNextStates.size(); i++)
    {
        int val;
        if(level-1 == 0) val = eval(lNextStates[i], level);
        else        val = minFunction(lNextStates[i], level-1, alpha, beta);
        if(val > alpha) alpha = val;
        if(beta <= alpha) return alpha;
    }

    return alpha;
}

int minFunction(GameState state, int level, int alpha, int beta)
{
    //if(level == 0) return eval(state);

    std::vector<GameState> lNextStates;
    state.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0) return eval(state, level);

    for(uint16_t i = 0; i < lNextStates.size(); i++)
    {
        int val;
        if(level-1 == 0) val = eval(lNextStates[i], level);
        else        val = maxFunction(lNextStates[i], level-1, alpha, beta);
        if(val < beta) beta = val;
        if(beta <= alpha) return beta;
    }

    return beta;
}

}

#ifndef ENGINE_H
#define ENGINE_H

#include <chrono>
#include <thread>

#include "constants.h"
#include "moves.h" 

inline uint8_t moveEquals(const Move &move1, const Move &move2) {
    return move1.piece == move2.piece && move1.startTile == move2.startTile
    && move1.endTile == move2.endTile && move1.special == move2.special;
}

int64_t evaluateGame(const Game &game) {
    int64_t eval = 0;
    eval += std::popcount(game.whitePawn) * 1;
    eval += std::popcount(game.whiteKnight) * 3;
    eval += std::popcount(game.whiteBishop) * 3;
    eval += std::popcount(game.whiteRook) * 5;
    eval += std::popcount(game.whiteQueen) * 9;
    eval -= std::popcount(game.blackPawn) * 1;
    eval -= std::popcount(game.blackKnight) * 3;
    eval -= std::popcount(game.blackBishop) * 3;
    eval -= std::popcount(game.blackRook) * 5;
    eval -= std::popcount(game.blackQueen) * 9;
    if(game.isWhiteTurn) {
        return eval;
    }
    else {
        return -1*eval;
    }
}

// alpha-beta negamax with capped quiesence search
int64_t negamax(std::stop_token sToken, auto startTime, int64_t timeMS, uint32_t &lastStopCheck, const Game &game, int64_t alpha, int64_t beta, int8_t depth) {
    Move moveTable[200] = {0};
    uint8_t offset = getLegalMoves(game, moveTable);
    Game nextGame;
    ++lastStopCheck;
    if(lastStopCheck > 1000) {
        if(sToken.stop_requested()) {
            return 0;
        }
        
        auto endTime = std::chrono::steady_clock::now();
        int64_t timeElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        if(timeElapsedMS > timeMS) {
            return 0;
        }
        
        lastStopCheck = 0;
    }
    // performs a quiesence search when the maximum depth is reached
    // limited to a depth of 3
    if(depth <= -3) {
        return evaluateGame(game);
    }
    
    if(depth <= 0) {
        int64_t gameEval = evaluateGame(game);
        uint8_t moveFlag = 1;
        for(uint8_t index = 0; index < offset; index++) {
            if(moveTable[index].piece) {
                moveFlag = 0;
                doMove(game, nextGame, moveTable[index]);
                int64_t nextGameEval = -1*evaluateGame(nextGame);
                if(gameEval != nextGameEval) {
                    nextGameEval = -1*negamax(sToken, startTime, timeMS, lastStopCheck, nextGame, -1*beta, -1*alpha, depth-1);
                }
                if(nextGameEval >= beta) {
                    return beta;
                }
                if(nextGameEval > alpha) {
                    alpha = nextGameEval;
                }
            }
        }
        if(moveFlag) {
            uint8_t result = checkGameEnding(game);
            if(result == LOSS) {
                return -1000-depth;
            }
            else {
                return 0;
            }
        }
        return alpha;
    }
    
    uint8_t moveFlag = 1;
    for(uint8_t index = 0; index < offset; index++) {
        if(moveTable[index].piece) {
            moveFlag = 0;
            doMove(game, nextGame, moveTable[index]);
            int64_t currentEval = -1*negamax(sToken, startTime, timeMS, lastStopCheck, nextGame, -1*beta, -1*alpha, depth-1);
            if(currentEval >= beta) {
                return beta;
            }
            if(currentEval > alpha) {
                alpha = currentEval;
            }
        }
    }
    if(moveFlag) {
        uint8_t result = checkGameEnding(game);
        if(result == LOSS) {
            return -1000-depth;
        }
        else {
            return 0;
        }
    }
    return alpha;
}

Move getBestMove(std::stop_token sToken, auto startTime, int64_t timeMS, const Game &game, int8_t depth, int64_t &eval, Move lBestMove) {
    Move moveTable[200] = {0};
    uint8_t offset = getLegalMoves(game, moveTable);
    int64_t alpha = -99999999;
    int64_t beta = 99999999;
    Move bestMove = lBestMove;
    Game nextGame;
    uint32_t lastStopCheck = 0;
    
    if(lBestMove.piece) {
        doMove(game, nextGame, lBestMove);
        alpha = -1*negamax(sToken, startTime, timeMS, lastStopCheck, nextGame, -1*beta, -1*alpha, depth-1);
    }
    
    for(uint8_t index = 0; index < offset; index++) {
        if(moveTable[index].piece && !(moveEquals(moveTable[index], lBestMove))) {
            doMove(game, nextGame, moveTable[index]);
            int64_t currentEval = -1*negamax(sToken, startTime, timeMS, lastStopCheck, nextGame, -1*beta, -1*alpha, depth-1);
            if(currentEval > alpha) {
                alpha = currentEval;
                bestMove = moveTable[index];
            }
        }
        if(sToken.stop_requested()) {
            return bestMove;
        }
        
        auto endTime = std::chrono::steady_clock::now();
        int64_t timeElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        if(timeElapsedMS > timeMS) {
            return bestMove;
        }
    }
    eval = alpha;
    return bestMove;
}

void timedSearch(std::stop_token sToken, int64_t timeMS, const Game &game, Move &bestMove) {
    auto startTime = std::chrono::steady_clock::now();
    int64_t eval;
    int8_t currentDepth = 1;
    while(currentDepth < 100) {
        Move currentMove = getBestMove(sToken, startTime, timeMS, game, currentDepth, eval, bestMove);
        if(sToken.stop_requested()) {
            std::string returnStr = "bestmove ";
            returnStr += getTileString(bestMove.startTile);
            returnStr += getTileString(bestMove.endTile);
            std::cout << returnStr << std::endl;
            return;
        }
        
        auto endTime = std::chrono::steady_clock::now();
        int64_t timeElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        if(timeElapsedMS > timeMS) {
            std::string returnStr = "bestmove ";
            returnStr += getTileString(bestMove.startTile);
            returnStr += getTileString(bestMove.endTile);
            std::cout << returnStr << std::endl;
            return;
        }
        
        if(currentMove.piece) {
            bestMove = currentMove;
        }
        else {
            currentDepth--;
        }
        currentDepth++;
    }
}

#endif

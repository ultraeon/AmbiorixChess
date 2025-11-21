#ifndef ENGINE_H
#define ENGINE_H

#include <chrono>

#include "constants.h"
#include "moves.h" 

Move iterativeDeepening(const Game &game, uint32_t timeMS);
Move getBestMove(const Game &game, uint8_t depth, uint32_t timeMS, int64_t &eval);
int64_t negamax(const Game &game, uint8_t depth);
int64_t evaluateGame(const Game &game);

Move iterativeDeepening(const Game &game, uint32_t timeMS) {
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = std::chrono::steady_clock::now();
    uint32_t timeElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
    
    Move bestMove;
    int64_t eval;
    uint8_t currentDepth = 1;
    while((timeElapsedMS < timeMS) && (currentDepth < 200)) {
        Move currentMove = getBestMove(game, currentDepth, timeMS-timeElapsedMS, eval);
        if(currentMove.piece) {
            bestMove = currentMove;
            std::cout << "Depth: " << std::to_string(currentDepth) << std::endl;
            std::cout << "Evaluation: " << std::to_string(eval) << std::endl;
            std::cout << "Best Move: " << getMoveString(bestMove) << "\n" << std::endl;
        }
        else {
            currentDepth--;
        }
        endTime = std::chrono::steady_clock::now();
        timeElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        currentDepth++;
    }
    return bestMove;
}

Move getBestMove(const Game &game, uint8_t depth, uint32_t timeMS, int64_t &eval) {
    auto startTime = std::chrono::steady_clock::now();
    Move moveTable[200] = {0};
    getLegalMoves(game, moveTable);
    int64_t bestEval = -99999999;
    Move bestMove;
    Game nextGame;
    for(Move move : moveTable) {
        auto endTime = std::chrono::steady_clock::now();
        uint32_t timeElapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        if(timeElapsedMS > timeMS) {
            Move dummyMove = {0};
            return dummyMove;
        }
        if(move.piece) {
            doMove(game, nextGame, move);
            int64_t currentEval = -1*negamax(nextGame, depth-1);
            if(currentEval > bestEval) {
                bestEval = currentEval;
                bestMove = move;
            }
        }
    }
    eval = bestEval;
    return bestMove;
}

// vanilla negamax
int64_t negamax(const Game &game, uint8_t depth) {
    if(depth == 0) {
        return evaluateGame(game);
    }
    Move moveTable[200] = {0};
    getPLMoves(game, moveTable);
    int64_t bestEval = -99999999;
    Game nextGame;
    
    for(Move move : moveTable) {
        if(move.piece) {
            doMove(game, nextGame, move);
            int64_t currentEval = -1*negamax(nextGame, depth-1);
            if(currentEval > bestEval) {
                bestEval = currentEval;
            }
        }
    }
    return bestEval;
}

inline uint8_t popCount(uint64_t bitboard) {
    uint8_t pop = 0;
    while(bitboard) {
        bitboard ^= uint64_t(1) << std::countr_zero(bitboard);
        pop++;
    }
    return pop;
}

int64_t evaluateGame(const Game &game) {
    int64_t eval = 0;
    eval += popCount(game.whitePawn) * 1;
    eval += popCount(game.whiteKnight) * 3;
    eval += popCount(game.whiteBishop) * 3;
    eval += popCount(game.whiteRook) * 5;
    eval += popCount(game.whiteQueen) * 9;
    eval += popCount(game.whiteKing) * 1000;
    eval -= popCount(game.blackPawn) * 1;
    eval -= popCount(game.blackKnight) * 3;
    eval -= popCount(game.blackBishop) * 3;
    eval -= popCount(game.blackRook) * 5;
    eval -= popCount(game.blackQueen) * 9;
    eval -= popCount(game.blackKing) * 1000;
    if(game.isWhiteTurn) {
        return eval;
    }
    else {
        return -1*eval;
    }
}

#endif

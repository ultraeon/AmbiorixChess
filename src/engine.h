#ifndef ENGINE_H
#define ENGINE_H

#include "constants.h"
#include "moves.h" 

Move getBestMove(const Game &game, uint8_t depth);
int64_t negamax(const Game &game, uint8_t depth);
int64_t evaluateGame(const Game &game);

Move getBestMove(const Game &game, uint8_t depth) {
    Move moveTable[200] = {0};
    getLegalMoves(game, moveTable);
    int64_t bestEval = -99999999;
    Move bestMove;
    Game nextGame;
    for(Move move : moveTable) {
        if(depth == 0) {
            return move;
        }
        if(move.piece) {
            doMove(game, nextGame, move);
            int64_t currentEval = negamax(nextGame, depth-1);
            if(currentEval > bestEval) {
                bestEval = currentEval;
                bestMove = move;
            }
        }
    }
    std::cout << bestEval << std::endl;
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
            int64_t currentEval = -negamax(nextGame, depth-1);
            if(currentEval > bestEval) {
                bestEval = currentEval;
            }
        }
        else {
            break;
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
        return -1*eval;
    }
    return eval;
}

#endif


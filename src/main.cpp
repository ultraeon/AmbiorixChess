#include <iostream>
#include <cstdint>
#include <string>
#include <print>
#include <bit>
#include <chrono>

#include "constants.h"
#include "precompute.h"
#include "display.h"
#include "moves.h"
#include "engine.h"
#include "uci.h"

uint64_t nodeCount(const Game &game, uint8_t depth) {
    if(depth == 0) {
        return 1;
    }
    uint64_t totalNodes = 0;
    Game nextGame;
    Move moves[200] = {0};
    getLegalMoves(game, moves);
    for(Move move : moves) {
        if(move.piece) {
            doMove(game, nextGame, move);
            totalNodes += nodeCount(nextGame, depth-1);
        }
    }
    return totalNodes;
}


void perft(std::string fenString, uint8_t depth) {
    auto start_t = std::chrono::steady_clock::now();
    Game game = gameFromFEN(fenString);
    Game nextGame;
    Move moves[200] = {0};
    getLegalMoves(game, moves);
    uint64_t totalNodes = 0;
    for(Move move : moves) {
        if(move.piece) {
            doMove(game, nextGame, move);
            uint64_t currentNodes = nodeCount(nextGame, depth-1);
            totalNodes += currentNodes;
            std::cout << getTileString(move.startTile) << getTileString(move.endTile) << " " << std::to_string(currentNodes) << std::endl;
        }
    }
    auto end_t = std::chrono::steady_clock::now();
    std::cout << "\n" << std::to_string(totalNodes) << std::endl;
    
    auto duration = end_t - start_t;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << "Time Taken: " << milliseconds << "ms" << std::endl;
    auto nps = totalNodes / (milliseconds);
    std::cout << "Speed: " << nps << " kN/s" << std::endl;
}

int main(int argc, char *argv[]) {
	run();
	return 0;
}

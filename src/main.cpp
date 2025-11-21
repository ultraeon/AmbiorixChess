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

// puts the board to the start of a normal chess game
Game initGame() {
	Game game;

	game.blackPawn = BLACK_PAWN_START;
	game.blackRook = BLACK_ROOK_START;
	game.blackKnight = BLACK_KNIGHT_START;
	game.blackBishop = BLACK_BISHOP_START;
	game.blackQueen = BLACK_QUEEN_START;
	game.blackKing = BLACK_KING_START;

	game.whitePawn = WHITE_PAWN_START;
	game.whiteRook = WHITE_ROOK_START;
	game.whiteKnight = WHITE_KNIGHT_START;
	game.whiteBishop = WHITE_BISHOP_START;
	game.whiteQueen = WHITE_QUEEN_START;
	game.whiteKing = WHITE_KING_START;

	game.enPassantBoard = 0;
	game.isWhiteTurn = 1;
	game.whiteCanCastleKingside = 1;
	game.blackCanCastleKingside = 1;
	game.whiteCanCastleQueenside = 1;
	game.blackCanCastleQueenside = 1;

	return game;
}

// half moves and full moves unused
Game gameFromFEN(std::string fenString) {
    Game game;
    
    game.blackPawn = 0;
	game.blackRook = 0;
	game.blackKnight = 0;
	game.blackBishop = 0;
	game.blackQueen = 0;
	game.blackKing = 0;

	game.whitePawn = 0;
	game.whiteRook = 0;
	game.whiteKnight = 0;
	game.whiteBishop = 0;
	game.whiteQueen = 0;
	game.whiteKing = 0;
	
	game.enPassantBoard = 0;
	game.whiteCanCastleKingside = 0;
	game.blackCanCastleKingside = 0;
	game.whiteCanCastleQueenside = 0;
	game.blackCanCastleQueenside = 0;
	
    uint8_t i = 0;
    uint8_t index = 0;

    while(i < 64) {
        switch(fenString[index]) {
            case 'p': game.blackPawn |= uint64_t(1) << i; break;
            case 'n': game.blackKnight |= uint64_t(1) << i; break;
            case 'b': game.blackBishop |= uint64_t(1) << i; break;
            case 'r': game.blackRook |= uint64_t(1) << i; break;
            case 'q': game.blackQueen |= uint64_t(1) << i; break;
            case 'k': game.blackKing |= uint64_t(1) << i; break;
            case 'P': game.whitePawn |= uint64_t(1) << i; break;
            case 'N': game.whiteKnight |= uint64_t(1) << i; break;
            case 'B': game.whiteBishop |= uint64_t(1) << i; break;
            case 'R': game.whiteRook |= uint64_t(1) << i; break;
            case 'Q': game.whiteQueen |= uint64_t(1) << i; break;
            case 'K': game.whiteKing |= uint64_t(1) << i; break;
            case '2': i += 1; break;
            case '3': i += 2; break;
            case '4': i += 3; break;
            case '5': i += 4; break;
            case '6': i += 5; break;
            case '7': i += 6; break;
            case '8': i += 7; break;
            case '/': i--; break;
        }
        index++;
        i++;
    }
    index++;
    if(fenString[index] == 'w') {
        game.isWhiteTurn = 1;
    }
    else {
        game.isWhiteTurn = 0;
    }
    
    index += 2;
    while(fenString[index] != ' ') {
        switch(fenString[index]) {
            case 'K': game.whiteCanCastleKingside = 1; break;
            case 'Q': game.whiteCanCastleQueenside = 1; break;
            case 'k': game.blackCanCastleKingside = 1; break;
            case 'q': game.blackCanCastleQueenside = 1; break;
        }
        index++;
    }
    
    index++;
    if(fenString[index] != '-') {
        uint8_t row = 7-(fenString[index+1]-49);
        uint8_t col = fenString[index]-97;
        game.enPassantBoard = uint64_t(1) << (row*8 + col);
    }
    return game;
}

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
	initMasterLUT();
    perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5);
    
	return 0;
}

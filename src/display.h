#ifndef DISPLAY_H
#define DISPLAY_H

#include "constants.h"

struct Game;
struct Move;

std::string getTileString(uint8_t index) {
    std::string returnStr = "";
    switch(index % 8) {
        case(0): returnStr += "a"; break;
        case(1): returnStr += "b"; break;
        case(2): returnStr += "c"; break;
        case(3): returnStr += "d"; break;
        case(4): returnStr += "e"; break;
        case(5): returnStr += "f"; break;
        case(6): returnStr += "g"; break;
        case(7): returnStr += "h"; break;
    }
    
    switch(index / 8) {
        case(0): returnStr += "8"; break;
        case(1): returnStr += "7"; break;
        case(2): returnStr += "6"; break;
        case(3): returnStr += "5"; break;
        case(4): returnStr += "4"; break;
        case(5): returnStr += "3"; break;
        case(6): returnStr += "2"; break;
        case(7): returnStr += "1"; break;
    }
    
    return returnStr;
}

std::string getPieceString(uint8_t piece) {
    std::string returnStr = "";
    switch(piece) {
        case(WHITE_KING): returnStr += "White King"; break;
        case(WHITE_PAWN): returnStr += "White Pawn"; break;
        case(WHITE_KNIGHT): returnStr += "White Knight"; break;
        case(WHITE_BISHOP): returnStr += "White Bishop"; break;
        case(WHITE_ROOK): returnStr += "White Rook"; break;
        case(WHITE_QUEEN): returnStr += "White Queen"; break;
        case(BLACK_KING): returnStr += "Black King"; break;
        case(BLACK_PAWN): returnStr += "Black Pawn"; break;
        case(BLACK_KNIGHT): returnStr += "Black Knight"; break;
        case(BLACK_BISHOP): returnStr += "Black Bishop"; break;
        case(BLACK_ROOK): returnStr += "Black Rook"; break;
        case(BLACK_QUEEN): returnStr += "Black Queen"; break;
    }
    return returnStr;
}

// gets a string representing the board state
std::string getGameString(const Game &game) {
    std::string returnStr = (game.isWhiteTurn) ? "White to Move\n" : "Black to Move\n";
    for(uint8_t row = 0; row < 8; row++) {
        for(uint8_t col = 0; col < 8; col++) {
            
            if(readTile(game.blackPawn, row, col)) {
                returnStr += "p";
            }
            else if (readTile(game.blackRook, row, col)) {
                returnStr += "r";
            }
            else if (readTile(game.blackKnight, row, col)) {
                returnStr += "n";
            }
            else if (readTile(game.blackQueen, row, col)) {
                returnStr += "q";
            }
            else if (readTile(game.blackBishop, row, col)) {
                returnStr += "b";
            }
            else if (readTile(game.blackKing, row, col)) {
                returnStr += "k";
            }
            else if (readTile(game.whitePawn, row, col)) {
                returnStr += "P";
            }
            else if (readTile(game.whiteRook, row, col)) {
                returnStr += "R";
            }
            else if (readTile(game.whiteKnight, row, col)) {
                returnStr += "N";
            }
            else if (readTile(game.whiteBishop, row, col)) {
                returnStr += "B";
            }
            else if (readTile(game.whiteQueen, row, col)) {
                returnStr += "Q";
            }
            else if (readTile(game.whiteKing, row, col)) {
                returnStr += "K";
            }
            else if ((row + col) % 2 == 0) {
                returnStr += "-";
            }
            else {
                returnStr += "\\";
            }
            returnStr += " ";
        }
        returnStr += "\n";
    }
    
    return returnStr;
}

// gets a string representing a single bitboard
std::string getBitboardString(uint64_t bitboard) {
    std::string returnStr = "";
    for(uint8_t row = 0; row < 8; row++) {
        for(uint8_t col = 0; col < 8; col++) {
            if(readTile(bitboard, row, col)) {
                returnStr += "1";
            }
            else {
                returnStr += "0";
            }
        }
        returnStr += "\n";
    }
    return returnStr;
}

std::string getMoveString(const Move &move) {
    std::string returnStr = "";
    returnStr += getPieceString(move.piece);
    returnStr += " | ";
    returnStr += getTileString(move.startTile);
    returnStr += " to ";
    returnStr += getTileString(move.endTile);
    
    if(move.special == PROMOTE_QUEEN) {
        returnStr += " | Promotion to Queen";
    }
    else if(move.special == PROMOTE_KNIGHT) {
        returnStr += " | Promotion to Knight";
    }
    else if(move.special == PROMOTE_BISHOP) {
        returnStr += " | Promotion to Bishop";
    }
    else if(move.special == PROMOTE_ROOK) {
        returnStr += " | Promotion to Rook";
    }
    return returnStr;
}

// gets a list of all moves stored in an array of moves
std::string getMoveTableString(const Move (&moveTable)[200]) {
    std::string returnStr = "";
    for(Move move : moveTable) {
        if(move.piece) {
            returnStr += getMoveString(move);
            returnStr += "\n";
        }
    }
    return returnStr;
}

#endif

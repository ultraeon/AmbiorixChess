#ifndef PRECOMPUTE_H
#define PRECOMPUTE_H

#include "constants.h"

void getKingLookupTable(uint64_t (&kingLookup)[64]) {
    uint64_t kingBoard;
    uint64_t kingMoves;
    
    // center tiles
    for(uint8_t row = 0; row < 8; row++) {
        for(uint8_t col = 0; col < 8; col++) {
            kingBoard = uint64_t(1) << row*8+col;
            // handling all directions
            uint8_t leftEdge = (col == 0) ? 1 : 0;
            uint8_t rightEdge = (col == 7) ? 1 : 0;
            uint8_t downEdge = (row == 7) ? 1 : 0;
            uint8_t upEdge = (row == 0) ? 1 : 0;
            
            kingMoves = 0;
            if(!(upEdge || leftEdge)) {
                kingMoves |= kingBoard >> 9; // up left
                kingMoves |= kingBoard >> 8; // up
                kingMoves |= kingBoard >> 1; // left
            }
            
            if(!(upEdge || rightEdge)) {
                kingMoves |= kingBoard >> 7; // up right
                kingMoves |= kingBoard >> 8; // up
                kingMoves |= kingBoard << 1; // right
            }
            
            if(!(downEdge || leftEdge)) {
                kingMoves |= kingBoard << 7; // down left
                kingMoves |= kingBoard << 8; // down
                kingMoves |= kingBoard >> 1; // left
            }
            
            if(!(downEdge || rightEdge)) {
                kingMoves |= kingBoard << 9; // down right
                kingMoves |= kingBoard << 8; // down
                kingMoves |= kingBoard << 1; // right
            }

            kingLookup[row*8 + col] = kingMoves;
        }
    }
}

void getKnightLookupTable(uint64_t (&knightLookup)[64]) {
    uint64_t knightBoard;
    uint64_t knightMoves;
    
    for(uint8_t row = 0; row < 8; row++) {
        for(uint8_t col = 0; col < 8; col++) {
            knightBoard = uint64_t(1) << row*8 + col;
            
            uint8_t leftEdge = (col == 0) ? 1 : 0;
            uint8_t rightEdge = (col == 7) ? 1 : 0;
            uint8_t downEdge = (row == 7) ? 1 : 0;
            uint8_t upEdge = (row == 0) ? 1 : 0;
            
            uint8_t leftClose = (leftEdge || (col == 1)) ? 1 : 0;
            uint8_t rightClose = (rightEdge || (col == 6)) ? 1 : 0;
            uint8_t downClose = (downEdge || (row == 6)) ? 1 : 0;
            uint8_t upClose = (upEdge || (row == 1)) ? 1 : 0;
            
            knightMoves = 0;
            if(!(upClose || leftEdge)) {
                knightMoves |= knightBoard >> 17; // up left
            }
            if(!(upEdge || leftClose)) {
                knightMoves |= knightBoard >> 10; // left up
            }
            if(!(upClose || rightEdge)) {
                knightMoves |= knightBoard >> 15; // up right
            }
            if(!(upEdge || rightClose)) {
                knightMoves |= knightBoard >> 6; // right up
            }
            
            if(!(downClose || leftEdge)) {
                knightMoves |= knightBoard << 15; // down left
            }
            if(!(downEdge || leftClose)) { 
                knightMoves |= knightBoard << 6; // left down
            }
            if(!(downClose || rightEdge)) {
                knightMoves |= knightBoard << 17; // down right
            }
            if(!(downEdge || rightClose)) {
                knightMoves |= knightBoard << 10; // right down
            }

            knightLookup[row*8 + col] = knightMoves;
        }
    }
}

// 0-63 Diagonal
// 64-127 Antidiagonal
// 128-191 File
// 192-255 Rank
// pieceIndex is index % 64
void getHQLookupTable(uint64_t (&hQLUT)[256]) {
    for(uint8_t row = 0; row < 8; row++) {
        for(uint8_t col = 0; col < 8; col++) {
            uint8_t tempRow = row;
            uint8_t tempCol = col;
            uint64_t diagBoard = uint64_t(1) << (row*8 + col);
            uint64_t antiDiagBoard = diagBoard;
        
            while((tempRow < 7) && (tempCol < 7)) {
                diagBoard |= diagBoard << 9;
                tempRow++;
                tempCol++;
            }
            tempRow = row;
            tempCol = col;
            while((tempRow > 0) && (tempCol > 0)) {
                diagBoard |= diagBoard >> 9;
                tempRow--;
                tempCol--;
            }
        
            tempRow = row;
            tempCol = col;
            while((tempRow < 7) && (tempCol > 0)) {
                antiDiagBoard |= antiDiagBoard << 7;
                tempRow++;
                tempCol--;
            }
            tempRow = row;
            tempCol = col;
            while((tempRow > 0) && (tempCol < 7)) {
                antiDiagBoard |= antiDiagBoard >> 7;
                tempRow--;
                tempCol++;
            }
            
            hQLUT[row*8+col] = diagBoard;
            hQLUT[row*8+col+64] = antiDiagBoard;
            hQLUT[row*8+col+128] = uint64_t(0x0101010101010101) << col;
            hQLUT[row*8+col+192] = uint64_t(0xFF) << (row*8);
        }
    }
}

#endif

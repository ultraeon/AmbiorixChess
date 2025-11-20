#ifndef CONSTANTS_H
#define CONSTANTS_H

// structs

/* bitboard structure

      A  B  C  D  E  F  G  H
    -------------------------
  8 | 0  1  2  3  4  5  6  7
  7 | 8  9  10 11 12 13 14 15
  6 | 16 17 18 19 20 21 22 23
  5 | 24 25 26 27 28 29 30 31
  4 | 32 33 34 35 36 37 38 39
  3 | 40 41 42 43 44 45 46 47
  2 | 48 49 50 51 52 53 54 55
  1 | 56 57 58 59 60 61 62 63
  
*/

struct Game {
	uint64_t blackPawn;
	uint64_t blackRook;
	uint64_t blackKnight;
	uint64_t blackBishop;
	uint64_t blackQueen;
	uint64_t blackKing;

	uint64_t whitePawn;
	uint64_t whiteRook;
	uint64_t whiteKnight;
	uint64_t whiteBishop;
	uint64_t whiteQueen;
	uint64_t whiteKing;

	uint64_t enPassantBoard;
	uint8_t isWhiteTurn;
	uint8_t whiteCanCastleKingside;
	uint8_t whiteCanCastleQueenside;
	uint8_t blackCanCastleKingside;
	uint8_t blackCanCastleQueenside;
};

// might add extra info for special moves(castling, en passant, etc.)
struct Move {
	uint8_t startTile;
	uint8_t endTile;
	uint8_t piece;
	uint8_t special;
};

// bitwise methods

// basic bitwise get method
// intakes row and column for printing help
uint8_t readTile(uint64_t bitmap, uint8_t row, uint8_t col) {
	uint8_t index = row*8 + col;
	return 1 & (bitmap >> index);
}

// basic bitwise set method
uint64_t setTile(uint64_t bitmap, uint8_t row, uint8_t col) {
	uint8_t index = row*8 + col;
	uint64_t flip = uint64_t(1) << index;
	return bitmap | flip;
}

// basic bitwise clear method
uint64_t clearTile(uint64_t bitmap, uint8_t row, uint8_t col) {
	uint8_t index = row*8 + col;
	uint64_t flip = ~(uint64_t(1) << index);
	return bitmap ^ flip;
}


// basic bitwise get method
// intakes an index(constexpr converts A1...H8 to indices)
uint8_t readTile(uint64_t bitmap, uint8_t index) {
	return 1 & (bitmap >> index);
}

// basic bitwise set method
uint64_t setTile(uint64_t bitmap, uint8_t index) {
	uint64_t flip = uint64_t(1) << index;
	return bitmap | flip;
}

// basic bitwise clear method
uint64_t clearTile(uint64_t bitmap, uint8_t index) {
	uint64_t flip = ~(uint64_t(1) << index);
	return bitmap ^ flip;
}

// start positions
constexpr inline uint64_t BLACK_PAWN_START = 0x000000000000FF00;
constexpr inline uint64_t BLACK_ROOK_START = 0x0000000000000081;
constexpr inline uint64_t BLACK_KNIGHT_START = 0x0000000000000042;
constexpr inline uint64_t BLACK_BISHOP_START = 0x0000000000000024;
constexpr inline uint64_t BLACK_QUEEN_START = 0x0000000000000008;
constexpr inline uint64_t BLACK_KING_START = 0x0000000000000010;

constexpr inline uint64_t WHITE_PAWN_START = 0x00FF000000000000;
constexpr inline uint64_t WHITE_ROOK_START = 0x8100000000000000;
constexpr inline uint64_t WHITE_KNIGHT_START = 0x4200000000000000;
constexpr inline uint64_t WHITE_BISHOP_START = 0x2400000000000000;
constexpr inline uint64_t WHITE_QUEEN_START = 0x0800000000000000;
constexpr inline uint64_t WHITE_KING_START = 0x1000000000000000;

// tiling
constexpr inline uint8_t A1 = 56;
constexpr inline uint8_t B1 = 57;
constexpr inline uint8_t C1 = 58;
constexpr inline uint8_t D1 = 59;
constexpr inline uint8_t E1 = 60;
constexpr inline uint8_t F1 = 61;
constexpr inline uint8_t G1 = 62;
constexpr inline uint8_t H1 = 63;

constexpr inline uint8_t A2 = 48;
constexpr inline uint8_t B2 = 49;
constexpr inline uint8_t C2 = 50;
constexpr inline uint8_t D2 = 51;
constexpr inline uint8_t E2 = 52;
constexpr inline uint8_t F2 = 53;
constexpr inline uint8_t G2 = 54;
constexpr inline uint8_t H2 = 55;

constexpr inline uint8_t A3 = 40;
constexpr inline uint8_t B3 = 41;
constexpr inline uint8_t C3 = 42;
constexpr inline uint8_t D3 = 43;
constexpr inline uint8_t E3 = 44;
constexpr inline uint8_t F3 = 45;
constexpr inline uint8_t G3 = 46;
constexpr inline uint8_t H3 = 47;

constexpr inline uint8_t A4 = 32;
constexpr inline uint8_t B4 = 33;
constexpr inline uint8_t C4 = 34;
constexpr inline uint8_t D4 = 35;
constexpr inline uint8_t E4 = 36;
constexpr inline uint8_t F4 = 37;
constexpr inline uint8_t G4 = 38;
constexpr inline uint8_t H4 = 39;

constexpr inline uint8_t A5 = 24;
constexpr inline uint8_t B5 = 25;
constexpr inline uint8_t C5 = 26;
constexpr inline uint8_t D5 = 27;
constexpr inline uint8_t E5 = 28;
constexpr inline uint8_t F5 = 29;
constexpr inline uint8_t G5 = 30;
constexpr inline uint8_t H5 = 31;

constexpr inline uint8_t A6 = 16;
constexpr inline uint8_t B6 = 17;
constexpr inline uint8_t C6 = 18;
constexpr inline uint8_t D6 = 19;
constexpr inline uint8_t E6 = 20;
constexpr inline uint8_t F6 = 21;
constexpr inline uint8_t G6 = 22;
constexpr inline uint8_t H6 = 23;

constexpr inline uint8_t A7 = 8;
constexpr inline uint8_t B7 = 9;
constexpr inline uint8_t C7 = 10;
constexpr inline uint8_t D7 = 11;
constexpr inline uint8_t E7 = 12;
constexpr inline uint8_t F7 = 13;
constexpr inline uint8_t G7 = 14;
constexpr inline uint8_t H7 = 15;

constexpr inline uint8_t A8 = 0;
constexpr inline uint8_t B8 = 1;
constexpr inline uint8_t C8 = 2;
constexpr inline uint8_t D8 = 3;
constexpr inline uint8_t E8 = 4;
constexpr inline uint8_t F8 = 5;
constexpr inline uint8_t G8 = 6;
constexpr inline uint8_t H8 = 7;

// pieces
constexpr inline uint8_t WHITE_KING = 1;
constexpr inline uint8_t WHITE_PAWN = 2;
constexpr inline uint8_t WHITE_KNIGHT = 3;
constexpr inline uint8_t WHITE_BISHOP = 4;
constexpr inline uint8_t WHITE_ROOK = 5;
constexpr inline uint8_t WHITE_QUEEN = 6;
constexpr inline uint8_t BLACK_KING = 7;
constexpr inline uint8_t BLACK_PAWN = 8;
constexpr inline uint8_t BLACK_KNIGHT = 9;
constexpr inline uint8_t BLACK_BISHOP = 10;
constexpr inline uint8_t BLACK_ROOK = 11;
constexpr inline uint8_t BLACK_QUEEN = 12;

// special values for move struct
constexpr inline uint8_t NORMAL = 0;
constexpr inline uint8_t EN_PASSANT = 1;
constexpr inline uint8_t CASTLE_KINGSIDE = 2;
constexpr inline uint8_t CASTLE_QUEENSIDE = 3;
constexpr inline uint8_t PROMOTE_QUEEN = 4;
constexpr inline uint8_t PROMOTE_KNIGHT = 5;
constexpr inline uint8_t PROMOTE_BISHOP = 6;
constexpr inline uint8_t PROMOTE_ROOK = 7;

// file masks
constexpr inline uint64_t FILE_A_MASK = 0x0101010101010101;
constexpr inline uint64_t FILE_B_MASK = 0x0202020202020202;
constexpr inline uint64_t FILE_C_MASK = 0x0404040404040404;
constexpr inline uint64_t FILE_D_MASK = 0x0808080808080808;
constexpr inline uint64_t FILE_E_MASK = 0x1010101010101010;
constexpr inline uint64_t FILE_F_MASK = 0x2020202020202020;
constexpr inline uint64_t FILE_G_MASK = 0x4040404040404040;
constexpr inline uint64_t FILE_H_MASK = 0x8080808080808080;

// rank masks
constexpr inline uint64_t RANK_1_MASK = 0xFF00000000000000;
constexpr inline uint64_t RANK_2_MASK = 0x00FF000000000000;
constexpr inline uint64_t RANK_3_MASK = 0x0000FF0000000000;
constexpr inline uint64_t RANK_4_MASK = 0x000000FF00000000;
constexpr inline uint64_t RANK_5_MASK = 0x00000000FF000000;
constexpr inline uint64_t RANK_6_MASK = 0x0000000000FF0000;
constexpr inline uint64_t RANK_7_MASK = 0x000000000000FF00;
constexpr inline uint64_t RANK_8_MASK = 0x00000000000000FF;

// castling masks
constexpr inline uint64_t WHITE_KINGSIDE_CASTLE_MASK = 0x6000000000000000;
constexpr inline uint64_t WHITE_QUEENSIDE_CASTLE_MASK = 0x0E00000000000000;
constexpr inline uint64_t BLACK_KINGSIDE_CASTLE_MASK = 0x0000000000000060;
constexpr inline uint64_t BLACK_QUEENSIDE_CASTLE_MASK = 0x000000000000000E;
  
// castling start and end positions combined
constexpr inline uint64_t WHITE_KINGSIDE_KING_CASTLE = 0x5000000000000000;
constexpr inline uint64_t WHITE_QUEENSIDE_KING_CASTLE = 0x1400000000000000;
constexpr inline uint64_t BLACK_KINGSIDE_KING_CASTLE = 0x0000000000000050;
constexpr inline uint64_t BLACK_QUEENSIDE_KING_CASTLE = 0x0000000000000014;

constexpr inline uint64_t WHITE_KINGSIDE_ROOK_CASTLE = 0xA000000000000000;
constexpr inline uint64_t WHITE_QUEENSIDE_ROOK_CASTLE = 0x0900000000000000;
constexpr inline uint64_t BLACK_KINGSIDE_ROOK_CASTLE = 0x00000000000000A0;
constexpr inline uint64_t BLACK_QUEENSIDE_ROOK_CASTLE = 0x0000000000000009;

// master lookup table offsets
constexpr inline uint16_t HQ_DIAGONAL_OFFSET = 0;
constexpr inline uint16_t HQ_ANTIDIAGONAL_OFFSET = 64;
constexpr inline uint16_t HQ_FILE_OFFSET = 128;
constexpr inline uint16_t HQ_RANK_OFFSET = 192;
constexpr inline uint16_t KING_LUT_OFFSET = 256;
constexpr inline uint16_t KNIGHT_LUT_OFFSET = 320;

#endif

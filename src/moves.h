#ifndef MOVES_H
#define MOVES_H

#include "constants.h"

uint64_t masterLUT[384];

void initMasterLUT() {
    uint64_t kingLUT[64];
	uint64_t knightLUT[64];
	uint64_t hQLUT[256];
	getKingLookupTable(kingLUT);
	getKnightLookupTable(knightLUT);
	getHQLookupTable(hQLUT);
	
	uint16_t i = 0;
	for(i = 0; i < 256; i++) {
	    masterLUT[i] = hQLUT[i];
	}
	for(i = 256; i < 320; i++) {
	    masterLUT[i] = kingLUT[i-256];
	}
	for(i = 320; i < 384; i++) {
	    masterLUT[i] = knightLUT[i-320];
	}
}

uint8_t getPLKingMoves(const Game &game, Move (&moves)[200], uint8_t offset) {
	uint8_t pieceIndex = (game.isWhiteTurn) ? std::countr_zero(game.whiteKing) : std::countr_zero(game.blackKing);
	uint64_t moveBoard = masterLUT[pieceIndex+KING_LUT_OFFSET];
	uint64_t friendlyMask = 0;
	uint64_t enemyMask = 0;

	if(game.isWhiteTurn) {
		friendlyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		friendlyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
		enemyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		enemyMask |= game.blackBishop | game.blackRook | game.blackKnight;
	}
	else {
		friendlyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		friendlyMask |= game.blackBishop | game.blackRook | game.blackKnight;
		enemyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		enemyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
	}
	moveBoard &= ~friendlyMask;

	if(game.isWhiteTurn) {
		if(game.whiteCanCastleQueenside && !((friendlyMask | enemyMask) & WHITE_QUEENSIDE_CASTLE_MASK)) {
			moves[offset].startTile = E1;
			moves[offset].endTile = C1;
			moves[offset].piece = WHITE_KING;
			moves[offset].special = CASTLE_QUEENSIDE;
			offset++;
		}
		if(game.whiteCanCastleKingside && !((friendlyMask | enemyMask) & WHITE_KINGSIDE_CASTLE_MASK)) {
			moves[offset].startTile = E1;
			moves[offset].endTile = G1;
			moves[offset].piece = WHITE_KING;
			moves[offset].special = CASTLE_KINGSIDE;
			offset++;
		}
	}
	else {
		if(game.blackCanCastleQueenside && !((friendlyMask | enemyMask) & BLACK_QUEENSIDE_CASTLE_MASK)) {
			moves[offset].startTile = E8;
			moves[offset].endTile = C8;
			moves[offset].piece = BLACK_KING;
			moves[offset].special = CASTLE_QUEENSIDE;
			offset++;
		}
		if(game.blackCanCastleKingside && !((friendlyMask | enemyMask) & BLACK_KINGSIDE_CASTLE_MASK)) {
			moves[offset].startTile = E8;
			moves[offset].endTile = G8;
			moves[offset].piece = BLACK_KING;
			moves[offset].special = CASTLE_KINGSIDE;
			offset++;
		}
	}

	uint8_t rsb = std::countr_zero(moveBoard);
	uint8_t moveIndex = rsb;
	while(moveBoard) {
		moveBoard ^= uint64_t(1) << rsb;
		moves[offset].startTile = pieceIndex;
		moves[offset].endTile = moveIndex;
		moves[offset].special = NORMAL;
		moves[offset].piece = (game.isWhiteTurn) ? WHITE_KING : BLACK_KING;
		offset++;
		rsb = std::countr_zero(moveBoard);
		moveIndex = rsb;
	}
	return offset;
}

uint8_t getPLKnightMoves(const Game &game, Move (&moves)[200], uint8_t offset) {
	uint64_t knightBoard = (game.isWhiteTurn) ? game.whiteKnight : game.blackKnight;
	uint64_t friendlyMask = 0;
	if(game.isWhiteTurn) {
		friendlyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		friendlyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
	}
	else {
		friendlyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		friendlyMask |= game.blackBishop | game.blackRook | game.blackKnight;
	}

	uint8_t rsb = std::countr_zero(knightBoard);
	uint8_t rsb2 = 0;
	uint8_t pieceIndex = rsb;
	uint8_t moveIndex = 0;
	uint64_t currentKnight = 0;
	uint64_t moveBoard = 0;

	while(knightBoard) {
		knightBoard ^= uint64_t(1) << rsb;

		currentKnight = uint64_t(1) << pieceIndex;
		moveBoard = masterLUT[pieceIndex+KNIGHT_LUT_OFFSET];
		moveBoard &= ~(friendlyMask);

		uint8_t rsb2 = std::countr_zero(moveBoard);
		moveIndex = rsb2;

		while(moveBoard) {
			moveBoard ^= uint64_t(1) << rsb2;
			moves[offset].startTile = pieceIndex;
			moves[offset].endTile = moveIndex;
			moves[offset].special = NORMAL;
			moves[offset].piece = (game.isWhiteTurn) ? WHITE_KNIGHT : BLACK_KNIGHT;

			offset++;
			rsb2 = std::countr_zero(moveBoard);
			moveIndex = rsb2;
		}

		rsb = std::countr_zero(knightBoard);
		pieceIndex = rsb;
	}
	return offset;
}

uint8_t getPLPawnMoves(const Game &game, Move (&moves)[200], uint8_t offset) {
	uint64_t pawnBoard = (game.isWhiteTurn) ? game.whitePawn : game.blackPawn;

	uint64_t friendlyMask = 0;
	uint64_t enemyMask = 0;
	if(game.isWhiteTurn) {
		friendlyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		friendlyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
		enemyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		enemyMask |= game.blackBishop | game.blackRook | game.blackKnight;
	}
	else {
		enemyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		enemyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
		friendlyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		friendlyMask |= game.blackBishop | game.blackRook | game.blackKnight;
	}

	uint8_t rsb = std::countr_zero(pawnBoard);
	uint8_t rsb2 = 0;
	uint8_t pieceIndex = rsb;
	uint8_t moveIndex = 0;
	uint64_t currentPawn = 0;
	uint64_t moveBoard = 0;
	uint64_t attackBoard = 0;

	while(pawnBoard) {
		pawnBoard ^= uint64_t(1) << rsb;

		currentPawn = uint64_t(1) << pieceIndex;

		if(game.isWhiteTurn) {
			moveBoard = (currentPawn >> 8) & (~enemyMask); // normal advances
			moveBoard |= ((currentPawn & WHITE_PAWN_START) >> 16) & (~(enemyMask | (enemyMask >> 8) | (friendlyMask >> 8))); // double advances
			if(!(currentPawn & FILE_H_MASK)) {
				attackBoard = currentPawn >> 7; // right attack
			}
			if(!(currentPawn & FILE_A_MASK)) {
				attackBoard |= currentPawn >> 9; // left attack
			}
		}
		else {
			moveBoard = currentPawn << 8 & (~enemyMask); // normal advances
			moveBoard |= ((currentPawn & BLACK_PAWN_START) << 16) & (~(enemyMask | (enemyMask << 8) | (friendlyMask << 8))); // double advances
			if(!(currentPawn & FILE_A_MASK)) {
				attackBoard = currentPawn << 7; // left attack
			}
			if(!(currentPawn & FILE_A_MASK)) {
				attackBoard |= currentPawn << 9; // right attack
			}
		}

		if(attackBoard & game.enPassantBoard) {
			moves[offset].startTile = pieceIndex;
			moves[offset].endTile = std::countr_zero(game.enPassantBoard);
			moves[offset].piece = (game.isWhiteTurn) ? WHITE_PAWN : BLACK_PAWN;
			moves[offset].special = EN_PASSANT;
			offset++;
		}

		moveBoard &= ~(friendlyMask);
		attackBoard &= enemyMask;
		moveBoard |= attackBoard;
		attackBoard = 0;

		uint8_t rsb2 = std::countr_zero(moveBoard);
		moveIndex = rsb2;

		while(moveBoard) {
			moveBoard ^= uint64_t(1) << rsb2;

			if(game.isWhiteTurn) {
				if(moveIndex >= A7) {
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = NORMAL;
					moves[offset].piece = WHITE_PAWN;
				}
				else {
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_QUEEN;
					moves[offset].piece = WHITE_PAWN;
					offset++;
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_KNIGHT;
					moves[offset].piece = WHITE_PAWN;
					offset++;
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_BISHOP;
					moves[offset].piece = WHITE_PAWN;
					offset++;
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_ROOK;
					moves[offset].piece = WHITE_PAWN;
				}
			}
			else {
				if(moveIndex <= H2) {
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = NORMAL;
					moves[offset].piece = BLACK_PAWN;
				}
				else {
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_QUEEN;
					moves[offset].piece = BLACK_PAWN;
					offset++;
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_KNIGHT;
					moves[offset].piece = BLACK_PAWN;
					offset++;
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_BISHOP;
					moves[offset].piece = BLACK_PAWN;
					offset++;
					moves[offset].startTile = pieceIndex;
					moves[offset].endTile = moveIndex;
					moves[offset].special = PROMOTE_ROOK;
					moves[offset].piece = BLACK_PAWN;
				}
			}

			offset++;
			rsb2 = std::countr_zero(moveBoard);
			moveIndex = rsb2;
		}

		rsb = std::countr_zero(pawnBoard);
		pieceIndex = rsb;
	}
	return offset;
}

// stolen from the chess stack exchange
// thank you for hard carrying me
inline uint64_t reverseBits(uint64_t bitboard) {
    bitboard = (bitboard & 0x5555555555555555) << 1 | ((bitboard >> 1) & 0x5555555555555555);
    bitboard = (bitboard & 0x3333333333333333) << 2 | ((bitboard >> 2) & 0x3333333333333333);
    bitboard = (bitboard & 0x0f0f0f0f0f0f0f0f) << 4 | ((bitboard >> 4) & 0x0f0f0f0f0f0f0f0f);
    bitboard = (bitboard & 0x00ff00ff00ff00ff) << 8 | ((bitboard >> 8) & 0x00ff00ff00ff00ff);

    return (bitboard << 48) | ((bitboard & 0xffff0000) << 16) | ((bitboard >> 16) & 0xffff0000) | (bitboard >> 48);
}

uint8_t getPLBishopMoves(const Game &game, Move (&moves)[200], uint8_t offset) {
	uint64_t bishopBoard = (game.isWhiteTurn) ? game.whiteBishop : game.blackBishop;
	uint64_t friendlyMask = 0;
	uint64_t enemyMask = 0;
	if(game.isWhiteTurn) {
		friendlyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		friendlyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
        enemyMask |= game.blackKing | game.blackQueen | game.blackPawn;
        enemyMask |= game.blackBishop | game.blackRook | game.blackKnight;
	}
	else {
		friendlyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		friendlyMask |= game.blackBishop | game.blackRook | game.blackKnight;
		enemyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		enemyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
	}

	uint8_t rsb = std::countr_zero(bishopBoard);
	uint8_t rsb2 = 0;
	uint8_t pieceIndex = rsb;
	uint8_t moveIndex = 0;
	uint64_t currentBishop = 0;
	uint64_t moveBoard = 0;
	uint64_t forward = 0;;

	while(bishopBoard) {
		bishopBoard ^= uint64_t(1) << rsb;

		currentBishop = uint64_t(1) << pieceIndex;
		
		// diagonal hyperbolic quintessence
		forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_DIAGONAL_OFFSET];
		forward -= currentBishop * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_DIAGONAL_OFFSET]) - reverseBits(currentBishop) * 2);
		forward &= masterLUT[pieceIndex+HQ_DIAGONAL_OFFSET];
        moveBoard = forward;
        
        // antidiagonal hyperbolic quintessence
        forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex + HQ_ANTIDIAGONAL_OFFSET];
		forward -= currentBishop * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex + HQ_ANTIDIAGONAL_OFFSET]) - reverseBits(currentBishop) * 2);
		forward &= masterLUT[pieceIndex + HQ_ANTIDIAGONAL_OFFSET];
        moveBoard |= forward;

        moveBoard &= ~(friendlyMask);
		uint8_t rsb2 = std::countr_zero(moveBoard);
		moveIndex = rsb2;

		while(moveBoard) {
			moveBoard ^= uint64_t(1) << rsb2;
			moves[offset].startTile = pieceIndex;
			moves[offset].endTile = moveIndex;
			moves[offset].special = NORMAL;
			moves[offset].piece = (game.isWhiteTurn) ? WHITE_BISHOP : BLACK_BISHOP;

			offset++;
			rsb2 = std::countr_zero(moveBoard);
			moveIndex = rsb2;
		}

		rsb = std::countr_zero(bishopBoard);
		pieceIndex = rsb;
	}
	return offset;
}

uint8_t getPLRookMoves(const Game &game, Move (&moves)[200], uint8_t offset) {
	uint64_t rookBoard = (game.isWhiteTurn) ? game.whiteRook : game.blackRook;
	uint64_t friendlyMask = 0;
	uint64_t enemyMask = 0;
	if(game.isWhiteTurn) {
		friendlyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		friendlyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
        enemyMask |= game.blackKing | game.blackQueen | game.blackPawn;
        enemyMask |= game.blackBishop | game.blackRook | game.blackKnight;
	}
	else {
		friendlyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		friendlyMask |= game.blackBishop | game.blackRook | game.blackKnight;
		enemyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		enemyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
	}

	uint8_t rsb = std::countr_zero(rookBoard);
	uint8_t rsb2 = 0;
	uint8_t pieceIndex = rsb;
	uint8_t moveIndex = 0;
	uint64_t currentRook = 0;
	uint64_t moveBoard = 0;
	uint64_t forward = 0;;

	while(rookBoard) {
		rookBoard ^= uint64_t(1) << rsb;

		currentRook = uint64_t(1) << pieceIndex;
		
		// file hyperbolic quintessence
		forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex + HQ_FILE_OFFSET];
		forward -= currentRook * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex + HQ_FILE_OFFSET]) - reverseBits(currentRook) * 2);
		forward &= masterLUT[pieceIndex + HQ_FILE_OFFSET];
        moveBoard = forward;
        
        // rank hyperbolic quintessence
        forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex + HQ_RANK_OFFSET];
		forward -= currentRook * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex + HQ_RANK_OFFSET]) - reverseBits(currentRook) * 2);
		forward &= masterLUT[pieceIndex + HQ_RANK_OFFSET];
        moveBoard |= forward;

        moveBoard &= ~(friendlyMask);
		uint8_t rsb2 = std::countr_zero(moveBoard);
		moveIndex = rsb2;

		while(moveBoard) {
			moveBoard ^= uint64_t(1) << rsb2;
			moves[offset].startTile = pieceIndex;
			moves[offset].endTile = moveIndex;
			moves[offset].special = NORMAL;
			moves[offset].piece = (game.isWhiteTurn) ? WHITE_ROOK : BLACK_ROOK;

			offset++;
			rsb2 = std::countr_zero(moveBoard);
			moveIndex = rsb2;
		}

		rsb = std::countr_zero(rookBoard);
		pieceIndex = rsb;
	}
	return offset;
}

uint8_t getPLQueenMoves(const Game &game, Move (&moves)[200], uint8_t offset) {
	uint64_t queenBoard = (game.isWhiteTurn) ? game.whiteQueen : game.blackQueen;
	uint64_t friendlyMask = 0;
	uint64_t enemyMask = 0;
	if(game.isWhiteTurn) {
		friendlyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		friendlyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
        enemyMask |= game.blackKing | game.blackQueen | game.blackPawn;
        enemyMask |= game.blackBishop | game.blackRook | game.blackKnight;
	}
	else {
		friendlyMask |= game.blackKing | game.blackQueen | game.blackPawn;
		friendlyMask |= game.blackBishop | game.blackRook | game.blackKnight;
		enemyMask |= game.whiteKing | game.whiteQueen | game.whitePawn;
		enemyMask |= game.whiteBishop | game.whiteRook | game.whiteKnight;
	}

	uint8_t rsb = std::countr_zero(queenBoard);
	uint8_t rsb2 = 0;
	uint8_t pieceIndex = rsb;
	uint8_t moveIndex = 0;
	uint64_t currentQueen = 0;
	uint64_t moveBoard = 0;
	uint64_t forward = 0;;

	while(queenBoard) {
		queenBoard ^= uint64_t(1) << rsb;

		currentQueen = uint64_t(1) << pieceIndex;
		
		// diagonal hyperbolic quintessence
		forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_DIAGONAL_OFFSET];
		forward -= currentQueen * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_DIAGONAL_OFFSET]) - reverseBits(currentQueen) * 2);
		forward &= masterLUT[pieceIndex+HQ_DIAGONAL_OFFSET];
        moveBoard = forward;
        
        // antidiagonal hyperbolic quintessence
        forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_ANTIDIAGONAL_OFFSET];
		forward -= currentQueen * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_ANTIDIAGONAL_OFFSET]) - reverseBits(currentQueen) * 2);
		forward &= masterLUT[pieceIndex+HQ_ANTIDIAGONAL_OFFSET];
        moveBoard |= forward;
        
		// file hyperbolic quintessence
		forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_FILE_OFFSET];
		forward -= currentQueen * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_FILE_OFFSET]) - reverseBits(currentQueen) * 2);
		forward &= masterLUT[pieceIndex+HQ_FILE_OFFSET];
        moveBoard |= forward;
        
        // rank hyperbolic quintessence
        forward = (friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_RANK_OFFSET];
		forward -= currentQueen * 2;
		forward ^= reverseBits(reverseBits((friendlyMask | enemyMask) & masterLUT[pieceIndex+HQ_RANK_OFFSET]) - reverseBits(currentQueen) * 2);
		forward &= masterLUT[pieceIndex+HQ_RANK_OFFSET];
        moveBoard |= forward;

        moveBoard &= ~(friendlyMask);
		uint8_t rsb2 = std::countr_zero(moveBoard);
		moveIndex = rsb2;

		while(moveBoard) {
			moveBoard ^= uint64_t(1) << rsb2;
			moves[offset].startTile = pieceIndex;
			moves[offset].endTile = moveIndex;
			moves[offset].special = NORMAL;
			moves[offset].piece = (game.isWhiteTurn) ? WHITE_QUEEN : BLACK_QUEEN;

			offset++;
			rsb2 = std::countr_zero(moveBoard);
			moveIndex = rsb2;
		}

		rsb = std::countr_zero(queenBoard);
		pieceIndex = rsb;
	}
	return offset;
}

uint8_t getPLMoves(const Game &game, Move (&moves)[200]) {
    uint8_t offset = 0;
    offset = getPLKingMoves(game, moves, offset);
    offset = getPLKnightMoves(game, moves, offset);
    offset = getPLPawnMoves(game, moves, offset);
    offset = getPLBishopMoves(game, moves, offset);
    offset = getPLRookMoves(game, moves, offset);
    offset = getPLQueenMoves(game, moves, offset);
    return offset;
}

void doMove(const Game &game, Game &nextGame, const Move &move) {
    nextGame.isWhiteTurn = !game.isWhiteTurn;
    
    nextGame.whiteCanCastleKingside = game.whiteCanCastleKingside;
    nextGame.whiteCanCastleQueenside = game.whiteCanCastleQueenside;
    nextGame.blackCanCastleKingside = game.blackCanCastleKingside;
    nextGame.blackCanCastleQueenside = game.blackCanCastleQueenside;
    
    nextGame.whiteKing = game.whiteKing;
    nextGame.whiteQueen = game.whiteQueen;
    nextGame.whiteBishop = game.whiteBishop;
    nextGame.whiteKnight = game.whiteKnight;
    nextGame.whitePawn = game.whitePawn;
    nextGame.whiteRook = game.whiteRook;
    
    nextGame.blackKing = game.blackKing;
    nextGame.blackQueen = game.blackQueen;
    nextGame.blackBishop = game.blackBishop;
    nextGame.blackKnight = game.blackKnight;
    nextGame.blackPawn = game.blackPawn;
    nextGame.blackRook = game.blackRook;
    
    nextGame.enPassantBoard = 0;
    
    if(move.special == CASTLE_KINGSIDE) {
        if(move.piece == WHITE_KING) {
            nextGame.whiteCanCastleKingside = 0;
            nextGame.whiteCanCastleQueenside = 0;
            nextGame.whiteKing ^= WHITE_KINGSIDE_KING_CASTLE;
            nextGame.whiteRook ^= WHITE_KINGSIDE_ROOK_CASTLE;
        }
        else {
            nextGame.blackCanCastleKingside = 0;
            nextGame.blackCanCastleQueenside = 0;
            nextGame.blackKing ^= BLACK_KINGSIDE_KING_CASTLE;
            nextGame.blackRook ^= BLACK_KINGSIDE_ROOK_CASTLE;
        }
        return;
    }
    else if(move.special == CASTLE_QUEENSIDE) {
        if(move.piece == WHITE_KING) {
            nextGame.whiteCanCastleKingside = 0;
            nextGame.whiteCanCastleQueenside = 0;
            nextGame.whiteKing ^= WHITE_QUEENSIDE_KING_CASTLE;
            nextGame.whiteRook ^= WHITE_QUEENSIDE_ROOK_CASTLE;
        }
        else {
            nextGame.blackCanCastleKingside = 0;
            nextGame.blackCanCastleQueenside = 0;
            nextGame.blackKing ^= BLACK_QUEENSIDE_KING_CASTLE;
            nextGame.blackRook ^= BLACK_QUEENSIDE_ROOK_CASTLE;
        }
        return;
    }
    uint64_t startTileMask = uint64_t(1) << move.startTile;
    uint64_t endTileMask = uint64_t(1) << move.endTile;
    
    if(move.special == PROMOTE_ROOK) {
        if(move.piece == WHITE_PAWN) {
            nextGame.whitePawn ^= startTileMask;
            nextGame.whiteRook ^= endTileMask;
            nextGame.blackKnight &= ~endTileMask;
            nextGame.blackBishop &= ~endTileMask;
            nextGame.blackKing &= ~endTileMask;
            nextGame.blackQueen &= ~endTileMask;
            nextGame.blackRook &= ~endTileMask;
        }
        else {
            nextGame.blackPawn ^= startTileMask;
            nextGame.blackRook ^= endTileMask;
            nextGame.whiteKnight &= ~endTileMask;
            nextGame.whiteBishop &= ~endTileMask;
            nextGame.whiteKing &= ~endTileMask;
            nextGame.whiteQueen &= ~endTileMask;
            nextGame.whiteRook &= ~endTileMask;
        }
        return;
    }
    else if(move.special == PROMOTE_BISHOP) {
        if(move.piece == WHITE_PAWN) {
            nextGame.whitePawn ^= startTileMask;
            nextGame.whiteBishop ^= endTileMask;
            nextGame.blackKnight &= ~endTileMask;
            nextGame.blackBishop &= ~endTileMask;
            nextGame.blackKing &= ~endTileMask;
            nextGame.blackQueen &= ~endTileMask;
            nextGame.blackRook &= ~endTileMask;
        }
        else {
            nextGame.blackPawn ^= startTileMask;
            nextGame.blackBishop ^= endTileMask;
            nextGame.whiteKnight &= ~endTileMask;
            nextGame.whiteBishop &= ~endTileMask;
            nextGame.whiteKing &= ~endTileMask;
            nextGame.whiteQueen &= ~endTileMask;
            nextGame.whiteRook &= ~endTileMask;
        }
        return;
    }
    else if(move.special == PROMOTE_KNIGHT) {
        if(move.piece == WHITE_PAWN) {
            nextGame.whitePawn ^= startTileMask;
            nextGame.whiteKnight ^= endTileMask;
            nextGame.blackKnight &= ~endTileMask;
            nextGame.blackBishop &= ~endTileMask;
            nextGame.blackKing &= ~endTileMask;
            nextGame.blackQueen &= ~endTileMask;
            nextGame.blackRook &= ~endTileMask;
        }
        else {
            nextGame.blackPawn ^= startTileMask;
            nextGame.blackKnight ^= endTileMask;
            nextGame.whiteKnight &= ~endTileMask;
            nextGame.whiteBishop &= ~endTileMask;
            nextGame.whiteKing &= ~endTileMask;
            nextGame.whiteQueen &= ~endTileMask;
            nextGame.whiteRook &= ~endTileMask;
        }
        return;
    }
    else if(move.special == PROMOTE_QUEEN) {
        if(move.piece == WHITE_PAWN) {
            nextGame.whitePawn ^= startTileMask;
            nextGame.whiteQueen ^= endTileMask;
            nextGame.blackKnight &= ~endTileMask;
            nextGame.blackBishop &= ~endTileMask;
            nextGame.blackKing &= ~endTileMask;
            nextGame.blackQueen &= ~endTileMask;
            nextGame.blackRook &= ~endTileMask;
        }
        else {
            nextGame.blackPawn ^= startTileMask;
            nextGame.blackQueen ^= endTileMask;
            nextGame.whiteKnight &= ~endTileMask;
            nextGame.whiteBishop &= ~endTileMask;
            nextGame.whiteKing &= ~endTileMask;
            nextGame.whiteQueen &= ~endTileMask;
            nextGame.whiteRook &= ~endTileMask;
        }
        return;
    }
    
    uint64_t combinedMask = startTileMask | endTileMask;
    if(move.special == EN_PASSANT) {
        if(move.piece == WHITE_PAWN) {
            nextGame.whitePawn ^= combinedMask;
            nextGame.blackPawn &= ~(endTileMask << 8);
        }
        else {
            nextGame.blackPawn ^= combinedMask;
            nextGame.whitePawn &= ~(endTileMask << 8);
        }
    }
    else if(move.special == NORMAL) {
        if(move.piece <= WHITE_QUEEN) { // is white
            switch(move.piece) {
                case(WHITE_KING): 
                    nextGame.whiteCanCastleKingside = 0;
                    nextGame.whiteCanCastleQueenside = 0;
                    nextGame.whiteKing ^= combinedMask;
                    break;
                case(WHITE_PAWN):
                    if(move.startTile-move.endTile == 16) {
                        nextGame.enPassantBoard = (startTileMask >> 8);
                    }
                    nextGame.whitePawn ^= combinedMask;
                    break;
                case(WHITE_KNIGHT): nextGame.whiteKnight ^= combinedMask; break;
                case(WHITE_ROOK):
                    if(move.startTile == A1) {
                        nextGame.whiteCanCastleQueenside = 0;
                    }
                    else if(move.startTile == H1) {
                        nextGame.whiteCanCastleKingside = 0;
                    }
                    nextGame.whiteRook ^= combinedMask;
                    break;
                case(WHITE_BISHOP): nextGame.whiteBishop ^= combinedMask; break;
                case(WHITE_QUEEN): nextGame.whiteQueen ^= combinedMask; break;
            }
            nextGame.blackKing &= ~endTileMask;
            nextGame.blackQueen &= ~endTileMask;
            nextGame.blackRook &= ~endTileMask;
            nextGame.blackPawn &= ~endTileMask;
            nextGame.blackBishop &= ~endTileMask;
            nextGame.blackKnight &= ~endTileMask;
        }
        else {
            switch(move.piece) {
                case(BLACK_KING): 
                    nextGame.blackCanCastleKingside = 0;
                    nextGame.blackCanCastleQueenside = 0;
                    nextGame.blackKing ^= combinedMask; 
                    break;
                case(BLACK_PAWN):
                    if(move.endTile-move.startTile == 16) {
                        nextGame.enPassantBoard = (endTileMask >> 8);
                    }
                    nextGame.blackPawn ^= combinedMask;
                    break;
                case(BLACK_KNIGHT): nextGame.blackKnight ^= combinedMask; break;
                case(BLACK_ROOK):
                    if(move.startTile == A8) {
                        nextGame.blackCanCastleQueenside = 0;
                    }
                    else if (move.startTile == H8) {
                        nextGame.blackCanCastleKingside = 0;
                    }
                    nextGame.blackRook ^= combinedMask;
                    break;
                case(BLACK_BISHOP): nextGame.blackBishop ^= combinedMask; break;
                case(BLACK_QUEEN): nextGame.blackQueen ^= combinedMask; break;
            }
            nextGame.whiteKing &= ~endTileMask;
            nextGame.whiteQueen &= ~endTileMask;
            nextGame.whiteRook &= ~endTileMask;
            nextGame.whitePawn &= ~endTileMask;
            nextGame.whiteBishop &= ~endTileMask;
            nextGame.whiteKnight &= ~endTileMask;
        }
    }
}

void getLegalMoves(const Game &game, Move (&moves)[200]) {
    uint8_t offset = getPLMoves(game, moves);
    
    Game nextGame;
    Game nextNextGame;
    for(uint8_t index = 0; index < offset; index++) {
        doMove(game, nextGame, moves[index]);
        Move nextMoves[200] = {0};
        uint8_t offset2 = getPLMoves(nextGame, nextMoves);
        for(uint8_t index2 = 0; index2 < offset2; index2++) {
            doMove(nextGame, nextNextGame, nextMoves[index2]);
            if(!(nextNextGame.blackKing) || !(nextNextGame.whiteKing)) {
                moves[index].piece = 0;
                break;
            }
        }
    }
}

#endif

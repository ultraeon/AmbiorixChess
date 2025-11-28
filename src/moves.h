#ifndef MOVES_H
#define MOVES_H

#include "constants.h"

uint64_t masterLUT[384];

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
	
	game.whitePieces = game.whitePawn | game.whiteRook | game.whiteKnight
	                   | game.whiteBishop | game.whiteQueen | game.whiteKing;
	game.blackPieces = game.blackPawn | game.blackRook | game.blackKnight 
	                   | game.blackBishop | game.blackQueen | game.blackKing;
	game.allPieces = game.whitePieces | game.blackPieces;

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
    
    std::cout << fenString << std::endl;
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
    
    game.whitePieces = game.whitePawn | game.whiteRook | game.whiteKnight
	                   | game.whiteBishop | game.whiteQueen | game.whiteKing;
	game.blackPieces = game.blackPawn | game.blackRook | game.blackKnight 
	                   | game.blackBishop | game.blackQueen | game.blackKing;
	game.allPieces = game.whitePieces | game.blackPieces;
    
    return game;
}

void initMasterLUT() {
	uint64_t kingLUT[64];
	uint64_t knightLUT[64];
	uint64_t hQLUT[256];
	getKingLookupTable(kingLUT);
	getKnightLookupTable(knightLUT);
	getHQLookupTable(hQLUT);

	uint16_t i = 0;
	for(i = 0; i < 256; ++i) {
		masterLUT[i] = hQLUT[i];
	}
	for(i = 256; i < 320; ++i) {
		masterLUT[i] = kingLUT[i-256];
	}
	for(i = 320; i < 384; ++i) {
		masterLUT[i] = knightLUT[i-320];
	}
}

inline void addMove(uint_fast8_t startTile, uint_fast8_t endTile, uint_fast8_t special, uint_fast8_t piece, uint_fast8_t &offset, Move(&moves)[200]) {
    moves[offset].startTile = startTile;
	moves[offset].endTile = endTile;
	moves[offset].special = special;
	moves[offset].piece = piece;
	++offset;
}

inline void addMovesFromBoard(uint64_t moveBoard, uint_fast8_t pieceIndex, uint_fast8_t piece, uint_fast8_t &offset, Move (&moves)[200]) {
    uint_fast8_t moveIndex = std::countr_zero(moveBoard);
	while(moveBoard) {
		moveBoard &= (moveBoard-1);
		addMove(pieceIndex, moveIndex, NORMAL, piece, offset, moves);
		moveIndex = std::countr_zero(moveBoard);
	}
}

uint_fast8_t getPLKingMoves(const Game &game, Move (&moves)[200], uint_fast8_t offset) {
	uint_fast8_t pieceIndex = (game.isWhiteTurn) ? std::countr_zero(game.whiteKing) : std::countr_zero(game.blackKing);
	uint_fast8_t piece = (game.isWhiteTurn) ? WHITE_KING : BLACK_KING;
	uint64_t friendlyMask = (game.isWhiteTurn) ? game.whitePieces : game.blackPieces;
	uint64_t occMask = game.allPieces;
	
	uint64_t moveBoard = masterLUT[pieceIndex+KING_LUT_OFFSET];
	moveBoard &= ~friendlyMask;

	if(game.isWhiteTurn) {
		if(game.whiteCanCastleQueenside && (!(occMask & WHITE_QUEENSIDE_CASTLE_MASK))) {
			addMove(pieceIndex, C1, CASTLE_QUEENSIDE, piece, offset, moves);
		}
		if(game.whiteCanCastleKingside && (!(occMask & WHITE_KINGSIDE_CASTLE_MASK))) {
			addMove(pieceIndex, G1, CASTLE_KINGSIDE, piece, offset, moves);
		}
	}
	else {
		if(game.blackCanCastleQueenside && (!(occMask & BLACK_QUEENSIDE_CASTLE_MASK))) {
			addMove(pieceIndex, C8, CASTLE_QUEENSIDE, piece, offset, moves);
		}
		if(game.blackCanCastleKingside && (!(occMask & BLACK_KINGSIDE_CASTLE_MASK))) {
			addMove(pieceIndex, G8, CASTLE_KINGSIDE, piece, offset, moves);
		}
	}
    addMovesFromBoard(moveBoard, pieceIndex, piece, offset, moves);
    return offset;
}

uint_fast8_t getPLKnightMoves(const Game &game, Move (&moves)[200], uint_fast8_t offset) {
	uint64_t knightBoard = (game.isWhiteTurn) ? game.whiteKnight : game.blackKnight;
	uint_fast8_t piece = (game.isWhiteTurn) ? WHITE_KNIGHT : BLACK_KNIGHT;
	uint64_t friendlyMask = (game.isWhiteTurn) ? game.whitePieces : game.blackPieces;

	uint_fast8_t pieceIndex = std::countr_zero(knightBoard);
	uint64_t moveBoard = 0;

	while(knightBoard) {
		knightBoard &= (knightBoard-1);
        
		moveBoard = masterLUT[pieceIndex+KNIGHT_LUT_OFFSET];
		moveBoard &= ~(friendlyMask);
		addMovesFromBoard(moveBoard, pieceIndex, piece, offset, moves);

		pieceIndex = std::countr_zero(knightBoard);
	}
	return offset;
}

uint_fast8_t getPLPawnMoves(const Game &game, Move (&moves)[200], uint_fast8_t offset) {
	uint64_t pawnBoard = (game.isWhiteTurn) ? game.whitePawn : game.blackPawn;
	uint_fast8_t piece = (game.isWhiteTurn) ? WHITE_PAWN : BLACK_PAWN;
	uint64_t friendlyMask = (game.isWhiteTurn) ? game.whitePieces : game.blackPieces;
	uint64_t enemyMask = (game.isWhiteTurn) ? game.blackPieces : game.whitePieces;
	uint64_t occMask = game.allPieces;

	uint_fast8_t pieceIndex = std::countr_zero(pawnBoard);
	uint64_t currentPawn = 0;
	uint64_t moveBoard = 0;
	uint64_t attackBoard = 0;

	while(pawnBoard) {
		pawnBoard &= (pawnBoard-1);

		currentPawn = uint64_t(1) << pieceIndex;

		if(game.isWhiteTurn) {
			moveBoard = (currentPawn >> 8) & (~enemyMask); // normal advances
			moveBoard |= ((currentPawn & WHITE_PAWN_START) >> 16) & (~(enemyMask | (occMask >> 8))); // double advances
			attackBoard = currentPawn >> 7 & ~(FILE_A_MASK); // right attack
			attackBoard |= currentPawn >> 9 & ~(FILE_H_MASK); // left attack
		}
		else {
			moveBoard = currentPawn << 8 & (~enemyMask); // normal advances
			moveBoard |= ((currentPawn & BLACK_PAWN_START) << 16) & (~(enemyMask | (occMask << 8))); // double advances
			attackBoard = currentPawn << 7 & ~(FILE_H_MASK); // left attack
			attackBoard |= currentPawn << 9 & ~(FILE_A_MASK); // right attack
		}

		if(attackBoard & game.enPassantBoard) {
		    addMove(pieceIndex, std::countr_zero(game.enPassantBoard), EN_PASSANT, piece, offset, moves);
		}

		moveBoard &= ~(friendlyMask);
		attackBoard &= enemyMask;
		moveBoard |= attackBoard;
		attackBoard = 0;
		
		uint64_t promotionBoard = moveBoard & PAWN_PROMOTION_MASK;
		uint64_t normalBoard = moveBoard & (~PAWN_PROMOTION_MASK);
        
        addMovesFromBoard(normalBoard, pieceIndex, piece, offset, moves);
		uint_fast8_t moveIndex = std::countr_zero(promotionBoard);

		while(promotionBoard) {
			promotionBoard ^= uint64_t(1) << moveIndex;
			addMove(pieceIndex, moveIndex, PROMOTE_QUEEN, piece, offset, moves);
			addMove(pieceIndex, moveIndex, PROMOTE_KNIGHT, piece, offset, moves);
			addMove(pieceIndex, moveIndex, PROMOTE_ROOK, piece, offset, moves);
			addMove(pieceIndex, moveIndex, PROMOTE_BISHOP, piece, offset, moves);
			moveIndex = std::countr_zero(promotionBoard);
		}
		pieceIndex = std::countr_zero(pawnBoard);
	}
	return offset;
}

inline uint64_t reverseBits(uint64_t bitboard) {
	bitboard = (bitboard & 0x5555555555555555) << 1 | ((bitboard >> 1) & 0x5555555555555555);
	bitboard = (bitboard & 0x3333333333333333) << 2 | ((bitboard >> 2) & 0x3333333333333333);
	bitboard = (bitboard & 0x0f0f0f0f0f0f0f0f) << 4 | ((bitboard >> 4) & 0x0f0f0f0f0f0f0f0f);
	bitboard = (bitboard & 0x00ff00ff00ff00ff) << 8 | ((bitboard >> 8) & 0x00ff00ff00ff00ff);

	return (bitboard << 48) | ((bitboard & 0xffff0000) << 16) | ((bitboard >> 16) & 0xffff0000) | (bitboard >> 48);
}

inline uint64_t hyperbolaQuintessence(uint64_t occMask, uint64_t pieceBoard, uint_fast8_t pieceIndex, uint16_t offset) {
    uint64_t forward = (occMask & masterLUT[pieceIndex+offset]) - (pieceBoard*2);
	forward ^= reverseBits(reverseBits(occMask & masterLUT[pieceIndex+offset]) - reverseBits(pieceBoard) * 2);
	forward &= masterLUT[pieceIndex+offset];
	return forward;
}

uint_fast8_t getPLBishopMoves(const Game &game, Move (&moves)[200], uint_fast8_t offset) {
	uint64_t bishopBoard = (game.isWhiteTurn) ? game.whiteBishop : game.blackBishop;
	if(!bishopBoard) {
	    return offset;
	}
	uint_fast8_t piece = (game.isWhiteTurn) ? WHITE_BISHOP : BLACK_BISHOP;
	uint64_t friendlyMask = (game.isWhiteTurn) ? game.whitePieces : game.blackPieces;
	uint64_t occMask = game.allPieces;

	uint_fast8_t pieceIndex = std::countr_zero(bishopBoard);
	uint64_t currentBishop = 0;
	uint64_t moveBoard = 0;
	uint64_t forward = 0;

	while(bishopBoard) {
		bishopBoard ^= uint64_t(1) << pieceIndex;
		currentBishop = uint64_t(1) << pieceIndex;

		moveBoard = hyperbolaQuintessence(occMask, currentBishop, pieceIndex, HQ_DIAGONAL_OFFSET);
        moveBoard |= hyperbolaQuintessence(occMask, currentBishop, pieceIndex, HQ_ANTIDIAGONAL_OFFSET);
		moveBoard &= ~(friendlyMask);
		addMovesFromBoard(moveBoard, pieceIndex, piece, offset, moves);
		
		pieceIndex = std::countr_zero(bishopBoard);
	}
	return offset;
}

uint_fast8_t getPLRookMoves(const Game &game, Move (&moves)[200], uint_fast8_t offset) {
	uint64_t rookBoard = (game.isWhiteTurn) ? game.whiteRook : game.blackRook;
	uint_fast8_t piece = (game.isWhiteTurn) ? WHITE_ROOK : BLACK_ROOK; 
	uint64_t friendlyMask = (game.isWhiteTurn) ? game.whitePieces : game.blackPieces;
	uint64_t occMask = game.allPieces;
    
	uint_fast8_t pieceIndex = std::countr_zero(rookBoard);
	uint64_t currentRook = 0;
	uint64_t moveBoard = 0;
	uint64_t forward = 0;;

	while(rookBoard) {
		rookBoard ^= uint64_t(1) << pieceIndex;
		currentRook = uint64_t(1) << pieceIndex;

		moveBoard = hyperbolaQuintessence(occMask, currentRook, pieceIndex, HQ_FILE_OFFSET);
		moveBoard |= hyperbolaQuintessence(occMask, currentRook, pieceIndex, HQ_RANK_OFFSET);
		moveBoard &= ~(friendlyMask);
        addMovesFromBoard(moveBoard, pieceIndex, piece, offset, moves);

		pieceIndex = std::countr_zero(rookBoard);
	}
	return offset;
}

uint_fast8_t getPLQueenMoves(const Game &game, Move (&moves)[200], uint_fast8_t offset) {
	uint64_t queenBoard = (game.isWhiteTurn) ? game.whiteQueen : game.blackQueen;
	uint_fast8_t piece = (game.isWhiteTurn) ? WHITE_QUEEN : BLACK_QUEEN;
	uint64_t friendlyMask = (game.isWhiteTurn) ? game.whitePieces : game.blackPieces;
	uint64_t occMask = game.allPieces;
    
	uint_fast8_t pieceIndex = std::countr_zero(queenBoard);
	uint64_t currentQueen = 0;
	uint64_t moveBoard = 0;
	uint64_t forward = 0;;

	while(queenBoard) {
		queenBoard ^= uint64_t(1) << pieceIndex;
		currentQueen = uint64_t(1) << pieceIndex;

        moveBoard = hyperbolaQuintessence(occMask, currentQueen, pieceIndex, HQ_DIAGONAL_OFFSET);
        moveBoard |= hyperbolaQuintessence(occMask, currentQueen, pieceIndex, HQ_ANTIDIAGONAL_OFFSET);
        moveBoard |= hyperbolaQuintessence(occMask, currentQueen, pieceIndex, HQ_FILE_OFFSET);
        moveBoard |= hyperbolaQuintessence(occMask, currentQueen, pieceIndex, HQ_RANK_OFFSET);
		moveBoard &= ~(friendlyMask);
		addMovesFromBoard(moveBoard, pieceIndex, piece, offset, moves);
		
		pieceIndex = std::countr_zero(queenBoard);
	}
	return offset;
}

uint_fast8_t getPLMoves(const Game &game, Move (&moves)[200]) {
    if(!(game.blackKing) || !(game.whiteKing)) {
        return 0;
    }
	uint_fast8_t offset = 0;
	offset = getPLPawnMoves(game, moves, offset);
	offset = getPLBishopMoves(game, moves, offset);
	offset = getPLKnightMoves(game, moves, offset);
	offset = getPLRookMoves(game, moves, offset);
	offset = getPLQueenMoves(game, moves, offset);
	offset = getPLKingMoves(game, moves, offset);
	return offset;
}

inline void updateWhiteAttacked(Game &game, uint64_t endTileMask) {
    game.whitePawn &= ~endTileMask;
    game.whiteKnight &= ~endTileMask;
	game.whiteBishop &= ~endTileMask;
	game.whiteKing &= ~endTileMask;
	game.whiteQueen &= ~endTileMask;
	game.whiteRook &= ~endTileMask;
	game.whitePieces &= ~endTileMask;
}

inline void updateBlackAttacked(Game &game, uint64_t endTileMask) {
    game.blackPawn &= ~endTileMask;
    game.blackKnight &= ~endTileMask;
	game.blackBishop &= ~endTileMask;
	game.blackKing &= ~endTileMask;
	game.blackQueen &= ~endTileMask;
	game.blackRook &= ~endTileMask;
	game.blackPieces &= ~endTileMask;
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
	nextGame.whitePieces = game.whitePieces;

	nextGame.blackKing = game.blackKing;
	nextGame.blackQueen = game.blackQueen;
	nextGame.blackBishop = game.blackBishop;
	nextGame.blackKnight = game.blackKnight;
	nextGame.blackPawn = game.blackPawn;
	nextGame.blackRook = game.blackRook;
	nextGame.blackPieces = game.blackPieces;
	
	nextGame.enPassantBoard = 0;
    
    uint64_t startTileMask = uint64_t(1) << move.startTile;
	uint64_t endTileMask = uint64_t(1) << move.endTile;
	uint64_t combinedMask = startTileMask | endTileMask;
    
	if(move.special == CASTLE_KINGSIDE) {
		if(move.piece == WHITE_KING) {
			nextGame.whiteCanCastleKingside = 0;
			nextGame.whiteCanCastleQueenside = 0;
			nextGame.whiteKing ^= WHITE_KINGSIDE_KING_CASTLE;
			nextGame.whiteRook ^= WHITE_KINGSIDE_ROOK_CASTLE;
			nextGame.whitePieces ^= WHITE_KINGSIDE_KING_CASTLE;
			nextGame.whitePieces ^= WHITE_KINGSIDE_ROOK_CASTLE;
		}
		else {
			nextGame.blackCanCastleKingside = 0;
			nextGame.blackCanCastleQueenside = 0;
			nextGame.blackKing ^= BLACK_KINGSIDE_KING_CASTLE;
			nextGame.blackRook ^= BLACK_KINGSIDE_ROOK_CASTLE;
			nextGame.blackPieces ^= BLACK_KINGSIDE_KING_CASTLE;
			nextGame.blackPieces ^= BLACK_KINGSIDE_ROOK_CASTLE;
		}
	}
	else if(move.special == CASTLE_QUEENSIDE) {
		if(move.piece == WHITE_KING) {
			nextGame.whiteCanCastleKingside = 0;
			nextGame.whiteCanCastleQueenside = 0;
			nextGame.whiteKing ^= WHITE_QUEENSIDE_KING_CASTLE;
			nextGame.whiteRook ^= WHITE_QUEENSIDE_ROOK_CASTLE;
			nextGame.whitePieces ^= WHITE_QUEENSIDE_KING_CASTLE;
			nextGame.whitePieces ^= WHITE_QUEENSIDE_ROOK_CASTLE;
		}
		else {
			nextGame.blackCanCastleKingside = 0;
			nextGame.blackCanCastleQueenside = 0;
			nextGame.blackKing ^= BLACK_QUEENSIDE_KING_CASTLE;
			nextGame.blackRook ^= BLACK_QUEENSIDE_ROOK_CASTLE;
			nextGame.blackPieces ^= BLACK_QUEENSIDE_KING_CASTLE;
			nextGame.blackPieces ^= BLACK_QUEENSIDE_ROOK_CASTLE;
		}
	}
    else if(move.special == PROMOTE_ROOK) {
		if(move.piece == WHITE_PAWN) {
			nextGame.whitePawn ^= startTileMask;
			nextGame.whiteRook ^= endTileMask;
			nextGame.whitePieces ^= combinedMask;
			updateBlackAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.blackRook ^ game.blackRook);
			if(rookCheck == A8) {
			    nextGame.blackCanCastleQueenside = 0;
			}
			else if(rookCheck == H8) {
			    nextGame.blackCanCastleKingside = 0;
			}
		}
		else {
			nextGame.blackPawn ^= startTileMask;
			nextGame.blackRook ^= endTileMask;
			nextGame.blackPieces ^= combinedMask;
			updateWhiteAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.whiteRook ^ game.whiteRook);
			if(rookCheck == A1) {
			    nextGame.whiteCanCastleQueenside = 0;
			}
			else if(rookCheck == H1) {
			    nextGame.whiteCanCastleKingside = 0;
			}
		}
	}
	else if(move.special == PROMOTE_BISHOP) {
		if(move.piece == WHITE_PAWN) {
			nextGame.whitePawn ^= startTileMask;
			nextGame.whiteBishop ^= endTileMask;
			nextGame.whitePieces ^= combinedMask;
			updateBlackAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.blackRook ^ game.blackRook);
			if(rookCheck == A8) {
			    nextGame.blackCanCastleQueenside = 0;
			}
			else if(rookCheck == H8) {
			    nextGame.blackCanCastleKingside = 0;
			}
		}
		else {
			nextGame.blackPawn ^= startTileMask;
			nextGame.blackBishop ^= endTileMask;
			nextGame.blackPieces ^= combinedMask;
			updateWhiteAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.whiteRook ^ game.whiteRook);
			if(rookCheck == A1) {
			    nextGame.whiteCanCastleQueenside = 0;
			}
			else if(rookCheck == H1) {
			    nextGame.whiteCanCastleKingside = 0;
			}
		}
	}
	else if(move.special == PROMOTE_KNIGHT) {
		if(move.piece == WHITE_PAWN) {
			nextGame.whitePawn ^= startTileMask;
			nextGame.whiteKnight ^= endTileMask;
			nextGame.whitePieces ^= combinedMask;
			updateBlackAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.blackRook ^ game.blackRook);
			if(rookCheck == A8) {
			    nextGame.blackCanCastleQueenside = 0;
			}
			else if(rookCheck == H8) {
			    nextGame.blackCanCastleKingside = 0;
			}
		}
		else {
			nextGame.blackPawn ^= startTileMask;
			nextGame.blackKnight ^= endTileMask;
			nextGame.blackPieces &= combinedMask;
			updateWhiteAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.whiteRook ^ game.whiteRook);
			if(rookCheck == A1) {
			    nextGame.whiteCanCastleQueenside = 0;
			}
			else if(rookCheck == H1) {
			    nextGame.whiteCanCastleKingside = 0;
			}
		}
	}
	else if(move.special == PROMOTE_QUEEN) {
		if(move.piece == WHITE_PAWN) {
			nextGame.whitePawn ^= startTileMask;
			nextGame.whiteQueen ^= endTileMask;
			nextGame.whitePieces ^= combinedMask;
			updateBlackAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.blackRook ^ game.blackRook);
			if(rookCheck == A8) {
			    nextGame.blackCanCastleQueenside = 0;
			}
			else if(rookCheck == H8) {
			    nextGame.blackCanCastleKingside = 0;
			}
		}
		else {
			nextGame.blackPawn ^= startTileMask;
			nextGame.blackQueen ^= endTileMask;
			nextGame.blackPieces ^= combinedMask;
			updateWhiteAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.whiteRook ^ game.whiteRook);
			if(rookCheck == A1) {
			    nextGame.whiteCanCastleQueenside = 0;
			}
			else if(rookCheck == H1) {
			    nextGame.whiteCanCastleKingside = 0;
			}
		}
	}
    else if(move.special == EN_PASSANT) {
		if(move.piece == WHITE_PAWN) {
			nextGame.whitePawn ^= combinedMask;
			nextGame.whitePieces ^= combinedMask;
			nextGame.blackPawn &= ~(endTileMask << 8);
			nextGame.blackPieces &= ~(endTileMask << 8);
		}
		else {
			nextGame.blackPawn ^= combinedMask;
			nextGame.blackPieces ^= combinedMask;
			nextGame.whitePawn &= ~(endTileMask >> 8);
			nextGame.whitePieces &= ~(endTileMask >> 8);
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
			case(WHITE_KNIGHT):
				nextGame.whiteKnight ^= combinedMask;
				break;
			case(WHITE_ROOK):
				if(move.startTile == A1) {
					nextGame.whiteCanCastleQueenside = 0;
				}
				else if(move.startTile == H1) {
					nextGame.whiteCanCastleKingside = 0;
				}
				nextGame.whiteRook ^= combinedMask;
				break;
			case(WHITE_BISHOP):
				nextGame.whiteBishop ^= combinedMask;
				break;
			case(WHITE_QUEEN):
				nextGame.whiteQueen ^= combinedMask;
				break;
			}
			nextGame.whitePieces ^= combinedMask;
			updateBlackAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.blackRook ^ game.blackRook);
			if(rookCheck == A8) {
			    nextGame.blackCanCastleQueenside = 0;
			}
			else if(rookCheck == H8) {
			    nextGame.blackCanCastleKingside = 0;
			}
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
			case(BLACK_KNIGHT):
				nextGame.blackKnight ^= combinedMask;
				break;
			case(BLACK_ROOK):
				if(move.startTile == A8) {
					nextGame.blackCanCastleQueenside = 0;
				}
				else if (move.startTile == H8) {
					nextGame.blackCanCastleKingside = 0;
				}
				nextGame.blackRook ^= combinedMask;
				break;
			case(BLACK_BISHOP):
				nextGame.blackBishop ^= combinedMask;
				break;
			case(BLACK_QUEEN):
				nextGame.blackQueen ^= combinedMask;
				break;
			}
			nextGame.blackPieces ^= combinedMask;
			updateWhiteAttacked(nextGame, endTileMask);
			uint_fast8_t rookCheck = std::countr_zero(nextGame.whiteRook ^ game.whiteRook);
			if(rookCheck == A1) {
			    nextGame.whiteCanCastleQueenside = 0;
			}
			else if(rookCheck == H1) {
			    nextGame.whiteCanCastleKingside = 0;
			}
		}
	}
	nextGame.allPieces = nextGame.whitePieces | nextGame.blackPieces;
}

inline uint_fast8_t isSquareAttacked(const Game &game, uint_fast8_t index) {
    uint64_t position = uint64_t(1) << index;
    uint64_t occMask = game.allPieces;
    uint64_t kingBoard;
    uint64_t knightBoard;
    uint64_t pawnBoard;
    uint64_t bishopBoard;
    uint64_t rookBoard;
    uint64_t queenBoard;
    if(game.isWhiteTurn) {
        kingBoard = game.blackKing;
        knightBoard = game.blackKnight;
        pawnBoard = game.blackPawn;
        bishopBoard = game.blackBishop;
        rookBoard = game.blackRook;
        queenBoard = game.blackQueen;
    }
    else {
        kingBoard = game.whiteKing;
        knightBoard = game.whiteKnight;
        pawnBoard = game.whitePawn;
        bishopBoard = game.whiteBishop;
        rookBoard = game.whiteRook;
        queenBoard = game.whiteQueen;
    }
    // king
    uint64_t kingPositions = masterLUT[index+KING_LUT_OFFSET];
    if(kingPositions & kingBoard) {
        return 1;
    }
    // knight
    uint64_t knightPositions = masterLUT[index+KNIGHT_LUT_OFFSET];
    if(knightPositions & knightBoard) {
        return 1;
    }
    // pawn
    uint64_t pawnPositions;
    if(game.isWhiteTurn) {
        pawnPositions = (position >> 7) & (~FILE_A_MASK);
        pawnPositions |= (position >> 9) & (~FILE_H_MASK);
    }
    else {
        pawnPositions = (position << 7) & (~FILE_H_MASK);
        pawnPositions |= (position << 9) & (~FILE_A_MASK);
    }
    if(pawnPositions & pawnBoard) {
        return 1;
    }
    // bishop/queen
    uint64_t bishopPositions = hyperbolaQuintessence(occMask, position, index, HQ_DIAGONAL_OFFSET);
    bishopPositions |= hyperbolaQuintessence(occMask, position, index, HQ_ANTIDIAGONAL_OFFSET);
    if(bishopPositions & (bishopBoard | queenBoard)) {
        return 1;
    }
    // rook/queen
    uint64_t rookPositions = hyperbolaQuintessence(occMask, position, index, HQ_FILE_OFFSET);
    rookPositions |= hyperbolaQuintessence(occMask, position, index, HQ_RANK_OFFSET);
    if(rookPositions & (rookBoard | queenBoard)) {
        return 1;
    }
    return 0;
}

uint_fast8_t getLegalMoves(const Game &game, Move (&moves)[200]) {
	uint_fast8_t offset = getPLMoves(game, moves);

	for(uint_fast8_t index = 0; index < offset; ++index) {
		if(moves[index].piece != WHITE_KING && moves[index].piece != BLACK_KING) {
		    Game nextGame;
		    doMove(game, nextGame, moves[index]);
		    nextGame.isWhiteTurn = !nextGame.isWhiteTurn;
		    uint64_t kingIndex = (game.isWhiteTurn) ? std::countr_zero(game.whiteKing) : std::countr_zero(game.blackKing);
		    if(isSquareAttacked(nextGame, kingIndex)) {
		        moves[index].piece = 0;
		    }
		}
		else {
		    Game nextGame;
		    doMove(game, nextGame, moves[index]);
		    nextGame.isWhiteTurn = !nextGame.isWhiteTurn;
		    if(isSquareAttacked(nextGame, moves[index].endTile)) {
		        moves[index].piece = 0;
		    }
		    if(moves[index].special == CASTLE_KINGSIDE) {
		        if(isSquareAttacked(game, moves[index].startTile) || isSquareAttacked(game, moves[index].startTile+1)) {
		            moves[index].piece = 0;
		        }
		    }
		    else if(moves[index].special == CASTLE_QUEENSIDE) {
		        if(isSquareAttacked(game, moves[index].startTile) || isSquareAttacked(game, moves[index].startTile-1)) {
		            moves[index].piece = 0;
		        }
		    }
		}
	}
	return offset;
}

uint8_t checkGameEnding(const Game &game) {
    uint8_t kingIndex = (game.isWhiteTurn) ? std::countr_zero(game.whiteKing) : std::countr_zero(game.blackKing);
    if(isSquareAttacked(game, kingIndex)) {
        return LOSS;
    }
    else {
        return DRAW;
    }
}

#endif

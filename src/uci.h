#ifndef UCI_H
#define UCI_H

#include <string>
#include <list>
#include <sstream>
#include <thread>
#include <functional>
#include <iostream>

#include "moves.h"
#include "engine.h"
#include "display.h"

Game game;
std::jthread searchThread;
uint8_t isActive;

std::list<std::string> inputToTokens(std::string input) {
    std::list<std::string> tokens;
    std::istringstream stringStream(input);
    std::string token;
    
    while(stringStream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string handleUci() {
    initMasterLUT();
    std::string returnStr = "id name AmbiorixV3\n";
    returnStr += "id author Ultraeon\n";
    returnStr += "uciok";
    return returnStr;
}

// unimplemented
void handleDebug(std::list<std::string> tokens) {
    return;
}

std::string handleIsReady() {
    std::string returnStr = "readyok";
    return returnStr;
}

// unimplemented
void handleSetOption(std::list<std::string> tokens) {
    return;
} 

// unimplemented
void handleRegister(std::list<std::string> tokens) {
    return;
}

void handleUciNewGame() {
    game = initGame();
    return;
}

void handlePosition(std::list<std::string> tokens) {
    std::string initType = tokens.front();
    tokens.pop_front();
    
    if(initType == "startpos") {
        game = initGame();
    }
    else if(initType == "fen") {
        std::string fenString;
        
        std::string token = (tokens.empty()) ? "" : tokens.front();
        while(!(token.empty()) && (token != "moves")) {
            tokens.pop_front();
            
            fenString += " " + token;
            
            token = (tokens.empty()) ? "" : tokens.front();;
        }
        game = gameFromFEN(initType);
    }
    
    if(!tokens.empty()) {
        tokens.pop_front();
    }
    
    for(std::string token : tokens) {
        uint8_t startIndex = token[0]-97;
        uint8_t endIndex = token[2]-97;
        uint8_t piece = 0;
        startIndex += 8*(8-(token[1]-48));
        endIndex += 8*(8-(token[3]-48));
        if(token.length() == 5) {
            if(game.isWhiteTurn) {
                switch(token[4]) {
                    case 'n': piece = WHITE_KNIGHT; break;
                    case 'b': piece = WHITE_BISHOP; break;
                    case 'r': piece = WHITE_ROOK; break;
                    case 'q': piece = WHITE_QUEEN; break;
                }
            }
            else {
                switch(token[4]) {
                    case 'n': piece = BLACK_KNIGHT; break;
                    case 'b': piece = BLACK_BISHOP; break;
                    case 'r': piece = BLACK_ROOK; break;
                    case 'q': piece = BLACK_QUEEN; break;
                }
            }
        }
        
        Move moves[200] = {0}; 
        getLegalMoves(game, moves);
        Game nextGame;
        if(piece) {
            for(Move move : moves) {
                if(move.piece) {
                    if(move.startTile == startIndex && move.endTile == endIndex && move.piece == piece) {
                        doMove(game, nextGame, move);
                        break;
                    }
                }
            }
        }
        else {
            for(Move move : moves) {
                if(move.piece) {
                    if(move.startTile == startIndex && move.endTile == endIndex) {
                        doMove(game, nextGame, move);
                        break;
                    }
                }
            }
        }
        
        game = nextGame;
    }
}

// only works with infinite
void handleGo(std::list<std::string> tokens) {
    std::string token = tokens.front();
    tokens.pop_front();
    
    std::string returnStr = "";
    
    if(token == "infinite") {
        searchThread = std::jthread(timedSearch, 0x7FFFFFFFFFFFFFFFLL, std::cref(game));
    }
    else if(token == "movetime") {
        std::string timeMS = tokens.front();
        searchThread = std::jthread(timedSearch, std::stoi(timeMS), std::cref(game));
    }
}

// unimplemented
void handlePonderHit() {
    return;
}

void handleStop() {
    if(searchThread.joinable()) {
        searchThread.request_stop();
    }
}

void handleQuit() {
    isActive = 0;
}

std::string handleLine(std::string input) {
    std::list<std::string> tokens = inputToTokens(input);
    std::string command = tokens.front();
    tokens.pop_front();
    
    std::string returnStr = "";
    if(command == "uci") {
        returnStr = handleUci();
    }
    else if(command == "debug") {
        handleDebug(tokens);
    }
    else if(command == "isready") {
        returnStr = handleIsReady();
    }
    else if(command == "setoption") {
        handleSetOption(tokens);
    }
    else if(command == "register") {
        handleRegister(tokens);
    }
    else if(command == "ucinewgame") {
        handleUciNewGame();
    }
    else if(command == "position") {
        handlePosition(tokens);    
    }
    else if(command == "go") {
        handleGo(tokens);   
    }
    else if(command == "ponderhit") {
        handlePonderHit();
    }
    else if(command == "stop") {
        handleStop();
    }
    else if(command == "quit") {
        handleQuit();    
    }
    
    return returnStr;
}

void run() {
    std::string input;
    isActive = 1;
    while(isActive) {
        std::getline(std::cin, input);
        std::string response = handleLine(input);
        if(!response.empty()) {
            std::cout << response << std::endl;
        }
    }
}

#endif

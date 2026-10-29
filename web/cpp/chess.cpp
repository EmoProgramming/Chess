#include <iostream>
#include <vector>
#include <array>
#include <stack>

#include "position.h"

using namespace std;

#include <emscripten.h>

Position position;

extern "C" {

EMSCRIPTEN_KEEPALIVE
int allocateMemory(int size) {
    return reinterpret_cast<int>(malloc(size));
}

EMSCRIPTEN_KEEPALIVE
void freeMemory(int ptr) {
    free(reinterpret_cast<void*>(ptr));
}

EMSCRIPTEN_KEEPALIVE
int getPieceJS(int row, int col) {
    return static_cast<int>(position.getPiece(row, col));
}

EMSCRIPTEN_KEEPALIVE
bool isWhiteToMoveJS() {
    return position.isWhiteToMove();
}

EMSCRIPTEN_KEEPALIVE
bool isWhitePieceJS(int row, int col) {
    return position.isWhitePiece(row, col);
}

EMSCRIPTEN_KEEPALIVE
bool isBlackPieceJS(int row, int col) {
    return position.isBlackPiece(row, col);
}


EMSCRIPTEN_KEEPALIVE
int generateLegalMovesJS(int* output) {
    vector<Move> legalMoves = position.generateLegalMoves();

    for (int i = 0; i < legalMoves.size(); i++) {
        output[i * 6 + 0] = legalMoves[i].fromY;
        output[i * 6 + 1] = legalMoves[i].fromX;
        output[i * 6 + 2] = legalMoves[i].toY;
        output[i * 6 + 3] = legalMoves[i].toX;
        output[i * 6 + 4] = static_cast<int>(legalMoves[i].type);
        output[i * 6 + 5] = static_cast<int>(legalMoves[i].promotionPiece);
    }

    return legalMoves.size();
}


EMSCRIPTEN_KEEPALIVE
void doMoveJS(int fromX, int fromY, int toX, int toY, int type, int piece) {
    Move move = {fromY, fromX, toY, toX, static_cast<Type>(type), static_cast<Piece>(piece)};
    position.doMove(move);
}

// STATE OF GAME

EMSCRIPTEN_KEEPALIVE
bool whiteCheckmatedJS() {
    return position.whiteCheckmated();
}

EMSCRIPTEN_KEEPALIVE
bool blackCheckmatedJS() {
    return position.blackCheckmated();
}

EMSCRIPTEN_KEEPALIVE
bool patJS() {
    return position.pat();
} 

EMSCRIPTEN_KEEPALIVE
bool isKingInCheckJS(bool whiteToMove) {
    return position.isKingInCheck(whiteToMove);
}

EMSCRIPTEN_KEEPALIVE
int evaluateJS() {
    return position.evaluate();
}

EMSCRIPTEN_KEEPALIVE
int minimaxJS(int depth, int alpha, int beta) {
    return position.minimax(depth, alpha, beta);
}


}
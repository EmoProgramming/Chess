#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <stack>



using namespace std;

enum Piece {
    EMPTY,
    WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK
};

struct State {
    Piece capturedPiece;
    bool bCastleQueenState, bCastleKingState;
    bool wCastleQueenState, wCastleKingState;
    pair<int, int> enpassantSquareState;
};

enum Color {
    WHITE, BLACK
};

enum Type {
    NORMAL, PROMOTION, ENPASSANT, CASTLE
};

struct Move {
    int fromY, fromX;
    int toY, toX;
    Type type;
    Piece promotionPiece;
};

vector<Piece> whitePieces = {WP, WN, WB, WR, WQ, WK};

char PieceToChar(Piece piece) {
    switch(piece) {
        case WP: return 'P';
        case WN: return 'N';
        case WB: return 'B';
        case WR: return 'R';
        case WQ: return 'Q';
        case WK: return 'K';

        case BP: return 'p';
        case BN: return 'n';
        case BB: return 'b';
        case BR: return 'r';
        case BQ: return 'q';
        case BK: return 'k';

        default: return '.';
    }
}

string typeToString(Type type) {
    switch(type) {
        case NORMAL: return "Normal";
        case ENPASSANT: return "Enpassant";
        case CASTLE: return "Castle";
        case PROMOTION: return "Promotion";
        default: return "Nonetype";
    }
}

class Position {
    public:
    stack<State> stateStack; 
    bool whiteToMove = true;
    bool wCastleKing = true;
    bool wCastleQueen = true;
    bool bCastleQueen = true;
    bool bCastleKing = true;
    pair<int, int> enpassantSquare = {-1, -1};
    vector<Move> moves;

    /*
    Piece board[8][8] = {
        {BB, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WR, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY}
    };
    */
    
    Piece board[8][8] = {
        {BR, BN, BB, BQ, BK, BB, BN, BR},
        {BP, BP, BP, BP, BP, BP, BP, BP},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {WP, WP, WP, WP, WP, WP, WP, WP},
        {WR, WN, WB, WQ, WK, WB, WN, WR}
    };

    // FOR JS WEB

    Piece getPiece(int row, int col) {
        return board[row][col];
    }

    bool isWhiteToMove() {
        return whiteToMove;
    }

    bool isEmpty(int row, int col) {
        return board[row][col] == EMPTY;
    }

    bool isWhitePiece(int row, int col) {
        for (Piece piece : whitePieces) {
            if (board[row][col] == piece) {
                return true;
            }
        }
        return false;
    }

    bool isBlackPiece(int row, int col) {
        return (!isWhitePiece(row, col) && !isEmpty(row, col));
    }

    // TILL HERE FOR JS WEB

    void printBoard() {
        for (int i = 0; i < 8; i++) {
            cout << 8-i << " | ";
            for (int j = 0; j < 8; j++) {
                cout << PieceToChar(board[i][j]) << "  ";
            }
            cout << "|" << 8-i << endl;
            if (i == 7) {continue;}
            cout << "  |                        |";
            cout << endl;
        }
        cout << "   ________________________" << endl;
        cout << "    a  b  c  d  e  f  g  h\n";
        cout << endl;
    }



    void chessCoords(int row, int col) {
        string chessColl;
        switch(col) {
            case 0: chessColl = "A"; break;
            case 1: chessColl = "B"; break;
            case 2: chessColl = "C"; break;
            case 3: chessColl = "D"; break;
            case 4: chessColl = "E"; break;
            case 5: chessColl = "F"; break;
            case 6: chessColl = "G"; break;
            case 7: chessColl = "H"; break;
        }
        cout << chessColl << 8-row;
    }

    
    void printMove(Move move) {
        chessCoords(move.fromY, move.fromX);
        cout << "->";
        chessCoords(move.toY, move.toX);
        cout << " " << typeToString(move.type) << " " << move.promotionPiece;
        cout << endl;
    }

    void printMoves(vector<Move> moves) {
        for (int i = 0; i < moves.size(); i++) {
            printMove(moves[i]);
        }
    }

    bool inSameTeam(int p1y, int p1x,  int p2y, int p2x) {
        if ((isupper(PieceToChar(board[p1y][p1x]))) && isupper(PieceToChar(board[p2y][p2x]))  ||  (islower(PieceToChar(board[p1y][p1x])) && islower(PieceToChar(board[p2y][p2x])))) {
            return true;
        } 
        else {return false;}
    }

    bool onBoard(int row, int col) {
        if (!(row < 8 && row >= 0)) {
                return false;
            }

        if (!(col < 8 && col >= 0)) {
                return false;
            }
        return true;
    }

    void whiteMoves(Piece piece, int row, int col) {
        switch (piece) {
            case WP: whitePawnMoves(row, col); break;
            case WR: rookMoves(row, col); break;
            case WN: knightMoves(row, col); break;
            case WB: bishopMoves(row, col); break;
            case WQ: queenMoves(row, col); break;
            case WK: kingMoves(row, col); break;
            default: return;
        }
    }

    void blackMoves(Piece piece, int row, int col) {
        switch (piece) {
            case BP: blackPawnMoves(row, col); break;
            case BR: rookMoves(row, col); break;
            case BN: knightMoves(row, col); break;
            case BB: bishopMoves(row, col); break;
            case BQ: queenMoves(row, col); break;
            case BK: kingMoves(row, col); break;
            default: return;
        }
    }

    void pseudoLegalMoves() {
        moves.clear();

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (whiteToMove) {
                    whiteMoves(board[i][j], i, j);
                } else {
                    blackMoves(board[i][j], i, j);
                }
                
            }
        }
    }

    void knightMoves(int row, int col) {
        array<pair<int, int>, 8> jumps = {{
            {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
            {2, 1}, {2, -1}, {-2, 1}, {-2, -1}  }};

        for (int i = 0; i < 8; i++) {
            Move move;
            move.fromY = row;
            move.fromX = col;
            move.toY = move.fromY + jumps[i].first;
            move.toX = move.fromX + jumps[i].second;
            move.type = NORMAL;

            if (move.toX >= 8 || move.toX < 0 || move.toY >= 8 || move.toY < 0) {
                continue;
            }

            if (inSameTeam(move.fromY, move.fromX, move.toY, move.toX)) {
                continue;
            }

            moves.push_back(move);
            
        }                              
    }

    void straightPath(int row, int col, int y, int x, int inc_y, int inc_x) {
        while (true) {
            x += inc_x;
            y += inc_y;

            if (!(onBoard(y, x))) {
                break;
            }
            
            if (board[y][x] != EMPTY) {
                if (!inSameTeam(row, col, y, x)) {
                    moves.push_back({row, col, y, x, NORMAL});
                }
                break;
            }
            moves.push_back({row, col, y, x, NORMAL});
        }
    }

    void rookMoves(int row, int col) {
        straightPath(row, col, row, col, 0, 1);
        straightPath(row, col, row, col, 1, 0);
        straightPath(row, col, row, col, -1, 0);
        straightPath(row, col, row, col, 0, -1);

    }

    void bishopMoves(int row, int col) {
        straightPath(row, col, row, col, 1, -1);
        straightPath(row, col, row, col, -1, 1);
        straightPath(row, col, row, col, 1, 1);
        straightPath(row, col, row, col, -1, -1);
    }

    void queenMoves(int row, int col) {
        straightPath(row, col, row, col, 0, 1);
        straightPath(row, col, row, col, 1, 0);
        straightPath(row, col, row, col, -1, 0);
        straightPath(row, col, row, col, 0, -1);
        straightPath(row, col, row, col, 1, -1);
        straightPath(row, col, row, col, -1, 1);
        straightPath(row, col, row, col, 1, 1);
        straightPath(row, col, row, col, -1, -1);
    }

    void genCastlingMoves() {
        if (whiteToMove) {
            if (wCastleQueen && board[7][3] == EMPTY && board[7][2] == EMPTY && board[7][1] == EMPTY &&
                !isSquareAttacked(7,4,BLACK) &&
                !isSquareAttacked(7,3,BLACK) &&
                !isSquareAttacked(7,2,BLACK)) {
                    moves.push_back({7, 4, 7, 2, CASTLE});
            }

            if (wCastleKing && board[7][5] == EMPTY && board[7][6] == EMPTY &&
                !isSquareAttacked(7,4,BLACK) &&
                !isSquareAttacked(7,5,BLACK) &&
                !isSquareAttacked(7,6,BLACK)) {
                    moves.push_back({7, 4, 7, 6, CASTLE});
                }
        } else {
            if (bCastleQueen && board[0][3] == EMPTY && board[0][2] == EMPTY && board[0][1] == EMPTY &&
                !isSquareAttacked(0,4,WHITE) &&
                !isSquareAttacked(0,3,WHITE) &&
                !isSquareAttacked(0,2,WHITE)) {
                    moves.push_back({0, 4, 0, 2, CASTLE});
            }

            if (bCastleKing && board[0][5] == EMPTY && board[0][6] == EMPTY &&
                !isSquareAttacked(0,4,WHITE) &&
                !isSquareAttacked(0,5,WHITE) &&
                !isSquareAttacked(0,6,WHITE)) {
                    moves.push_back({0, 4, 0, 6, CASTLE});
                }
        }
    }

    void kingMoves(int row, int col) {
        array<pair<int, int>, 8> jumps = {{
            {1, 1}, {-1, 1}, {1, -1}, {-1, -1},
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}  }};
    
    for (int i = 0; i < 8; i++) {
        Move move;
        move.fromY = row;
        move.fromX = col;
        move.toY = move.fromY + jumps[i].first;
        move.toX = move.fromX + jumps[i].second;
        move.type = NORMAL;

        if (move.toX >= 8 || move.toX < 0 || move.toY >= 8 || move.toY < 0) {
            continue;
        }

        if (inSameTeam(move.fromY, move.fromX, move.toY, move.toX)) {
            continue;
        }

        moves.push_back(move);
        
        }

        genCastlingMoves();
        
    }

    void pawnPromotion(int fromY, int fromX, int toY, int toX) {
        if (toY < fromY) {
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, WN});
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, WR});
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, WB});
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, WQ});
        } else {
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, BN});
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, BR});
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, BB});
        moves.push_back({fromY, fromX, toY, toX, PROMOTION, BQ});
        }
    }



    void pawnMoves(int row, int col, int y) {

        if (col-1 >= 0 && board[row+y][col-1] != EMPTY && !(inSameTeam(row, col, row+y, col-1))) {
            if (row+y == 0 || row + y == 7) {
                pawnPromotion(row, col, row+y, col-1);
            } else {
                moves.push_back({row, col, row+y, col-1, NORMAL});
            }
        }

        if (col+1 < 8 && board[row+y][col+1] != EMPTY && !(inSameTeam(row, col, row+y, col+1))) {
            if (row+y == 0 || row + y == 7) {
                pawnPromotion(row, col, row+y, col+1);
            } else {
                moves.push_back({row, col, row+y, col+1, NORMAL});
            }
        }

        if ((row + y == 0 || row + y == 7) && board[row+y][col] == EMPTY) {
            pawnPromotion(row, col, row+y, col);
            return;
        }

        if (col - 1 >= 0 && col - 1 == enpassantSquare.second && row + y == enpassantSquare.first) {
            moves.push_back({row, col, row+y, col-1, ENPASSANT});
        }

        if (col + 1 >= 0 && col + 1 == enpassantSquare.second && row + y == enpassantSquare.first) {
            moves.push_back({row, col, row+y, col+1, ENPASSANT});
        }

        if (board[row+y][col] == EMPTY) {
            moves.push_back({row, col, row+y, col, NORMAL});
        }

        if (y == -1 && row == 6 && (board[row+y][col] == EMPTY  && board[row + 2*y][col] == EMPTY)) {
            moves.push_back({row, col, row + 2*y, col, NORMAL});
        }

        if (y == 1 && row == 1 && (board[row+y][col] == EMPTY  && board[row + 2*y][col] == EMPTY)) {
            moves.push_back({row, col, row + 2*y, col, NORMAL});
        }
    }

    void whitePawnMoves(int row, int col) {
        pawnMoves(row, col, -1);
    }

    void blackPawnMoves(int row, int col) {
        pawnMoves(row, col, 1);
    }

    bool enPassantPossible(Move move) {
        if ((move.fromY == 1 || move.fromY == 6) && (move.toY == 3 || move.toY == 4) &&
        (board[move.toY][move.toX] == BP || board[move.toY][move.toX] == WP)) {
            return true;
        } else {return false;}
    }

    bool attackedByPawn(int row, int col, Color attackingColor) {
        if (attackingColor == BLACK) {
            if ((onBoard(row-1, col-1) && board[row-1][col-1] == BP) || (onBoard(row-1, col+1) && board[row-1][col+1] == BP)) {
                return true;
            }
        } else {
            if ((onBoard(row+1, col-1) && board[row+1][col-1] == WP) || (onBoard(row+1, col+1) && board[row+1][col+1] == WP)) {
                return true;
            }
        }
        return false;
    }

    bool attackedByKnight(int row, int col, Color attackingColor) {
            array<pair<int, int>, 8> jumps = {{
                {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
                {2, 1}, {2, -1}, {-2, 1}, {-2, -1}  }};

            for (int i = 0; i < 8; i++) {
                if (onBoard(row+jumps[i].first, col+jumps[i].second)) {
                    if (attackingColor == BLACK && board[row+jumps[i].first][col+jumps[i].second] == BN){
                        return true;
                    }
                    if (attackingColor == WHITE && board[row+jumps[i].first][col+jumps[i].second] == WN){
                        return true;
                    }
                }
            }
            return false;
    }

    bool attackedByRookAndQueen(int row, int col, Color attackingColor) {
        array<pair<int, int>, 4> dirs = {{
                {-1, 0}, {1, 0}, {0, 1}, {0, -1}  }};


        int y, x;
        for (auto [yDir, xDir] : dirs) {
            y = row+yDir;
            x = col+xDir;
            while (onBoard(y, x)) {

                if (board[y][x] != EMPTY) {
                    if (attackingColor == BLACK && (board[y][x] == BR || board[y][x] == BQ)) {
                        return true;
                    }
                    if (attackingColor == WHITE && (board[y][x] == WR || board[y][x] == WQ)) {
                        return true;
                    }
                    break;
                }
                y += yDir;
                x += xDir;
            }
        }
        return false;
        
    }

    bool attackedByBishopAndQueen(int row, int col, Color attackingColor) {
        array<pair<int, int>, 4> dirs = {{
                {-1, 1}, {1, 1}, {-1, -1}, {1, -1}  }};


        int y, x;
        for (auto [yDir, xDir] : dirs) {
            y = row+yDir;
            x = col+xDir;
            while (onBoard(y, x)) {

                if (board[y][x] != EMPTY) {
                    if (attackingColor == BLACK && (board[y][x] == BB || board[y][x] == BQ)) {
                        return true;
                    }
                    if (attackingColor == WHITE && (board[y][x] == WB || board[y][x] == WQ)) {
                        return true;
                    }
                    break;
                }
                y += yDir;
                x += xDir;
            }
        }
        return false;
        
    }

    bool attackedByKing(int row, int col, Color attackingColor) {
        array<pair<int, int>, 8> dirs = {{
            {1, 1}, {-1, 1}, {1, -1}, {-1, -1},
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}  }};
            
        for (auto [yDir, xDir] : dirs) {
            if (! onBoard(row+yDir, col+xDir)) {
                continue;
            }
            if (attackingColor == BLACK && board[row+yDir][col+xDir] == BK) {
                return true;
            }
            if (attackingColor == WHITE && board[row+yDir][col+xDir] == WK) {
                return true;
            }
        }
        return false;
    }

    bool isSquareAttacked(int row, int col, Color attackingColor) {

        if (attackedByPawn(row, col, attackingColor)) {
            return true;
        }
        if (attackedByKnight(row, col, attackingColor)) {
            return true;
        }

        if (attackedByBishopAndQueen(row, col, attackingColor)) {
            return true;
        }

        if (attackedByRookAndQueen(row, col, attackingColor)) {
            return true;
        }

        if (attackedByKing(row, col, attackingColor)) {
            return true;
        } 

        return false;
    }



    void doMove(Move move) {

        State state;
        state.bCastleKingState = bCastleKing;
        state.bCastleQueenState = bCastleQueen;
        state.wCastleKingState = wCastleKing;
        state.wCastleQueenState = wCastleQueen;
        

        if (board[move.fromY][move.fromX] == WK) {
            wCastleKing = false;
            wCastleQueen = false;
        }

        if (board[move.fromY][move.fromX] == BK) {
            bCastleKing = false;
            bCastleQueen = false;
        }

        if (move.fromY == 0 && move.fromX == 0) {
            bCastleQueen = false;
        }
        if (move.fromY == 0 && move.fromX == 7) {
            bCastleKing = false;
        }
        if (move.fromY == 7 && move.fromX == 0) {
            wCastleQueen = false;
        }
        if (move.fromY == 7 && move.fromX == 7) {
            wCastleKing = false;
        }
        if (move.type == NORMAL) {
            state.capturedPiece = board[move.toY][move.toX];
            board[move.toY][move.toX] = board[move.fromY][move.fromX];
            board[move.fromY][move.fromX] = EMPTY;
        } 
        else if (move.type == PROMOTION) {
            state.capturedPiece = board[move.toY][move.toX];
            board[move.toY][move.toX] = move.promotionPiece;
            board[move.fromY][move.fromX] = EMPTY;
        }
        else if (move.type == ENPASSANT) {
            board[move.toY][move.toX] = board[move.fromY][move.fromX];
            board[move.fromY][move.fromX] = EMPTY;
            state.capturedPiece = board[move.fromY][move.toX];
            board[move.fromY][move.toX] = EMPTY;

        } else if (move.type == CASTLE) {
            board[move.toY][move.toX] = board[move.fromY][move.fromX];
            board[move.fromY][move.fromX] = EMPTY;

            if (move.toX == 2) {
                board[move.toY][move.toX+1] = board[move.fromY][0];
                board[move.fromY][0] = EMPTY;
            } else if (move.toX == 6) {
                board[move.toY][move.toX-1] = board[move.fromY][7];
                board[move.fromY][7] = EMPTY;
            }
        }

        if (enPassantPossible(move)) {
            enpassantSquare = {(move.fromY+move.toY) / 2, move.fromX};
            state.enpassantSquareState = {(move.fromY+move.toY) / 2, move.fromX};
        } else {
            enpassantSquare = {-1, -1};
            state.enpassantSquareState = {-1, -1};
            }

        whiteToMove = !whiteToMove;

        stateStack.push(state);
    }

    void undoMove(Move move) {

        State state = stateStack.top();
        stateStack.pop();

        wCastleKing = state.wCastleKingState;
        wCastleQueen = state.wCastleQueenState;
        bCastleKing = state.bCastleKingState;
        bCastleQueen = state.bCastleQueenState;

        if (move.type == NORMAL) {
            board[move.fromY][move.fromX] = board[move.toY][move.toX];
            board[move.toY][move.toX] = state.capturedPiece;

        } else if (move.type == PROMOTION) {
            if (whiteToMove) {
                board[move.fromY][move.fromX] = BP;
            } else {
                board[move.fromY][move.fromX] = WP;
            }
            
            board[move.toY][move.toX] = state.capturedPiece;

        } else if (move.type == ENPASSANT) {
            board[move.fromY][move.fromX] = board[move.toY][move.toX];
            board[move.toY][move.toX] = EMPTY;
            board[move.fromY][move.toX] = state.capturedPiece;

        } else if (move.type == CASTLE) {
            board[move.fromY][move.fromX] = board[move.toY][move.toX];
            board[move.toY][move.toX] = EMPTY;
            if (move.toX == 2) {
                board[move.fromY][0] = board[move.toY][move.toX+1];
                board[move.toY][move.toX+1] = EMPTY;
            }
            else if (move.toX == 6) {
                board[move.fromY][7] = board[move.toY][move.toX-1];
                board[move.toY][move.toX-1] = EMPTY;
            }
        }

        enpassantSquare = state.enpassantSquareState;
        whiteToMove = !whiteToMove;
    }

    bool isKingInCheck(bool white) {
        Piece king = (white ? WK:BK);
        int kingY, kingX;

        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (board[y][x] == king) {
                    kingY = y;
                    kingX = x;
                }
            }
        }
        if (isSquareAttacked(kingY, kingX, (white ? BLACK:WHITE))) {
            return true;
        } else {
            return false;
        }
    }

    vector<Move> generateLegalMoves() {

        pseudoLegalMoves();
        vector<Move> legalMoves;

        for (auto move : moves) {

            doMove(move);

            if (!isKingInCheck(!whiteToMove)) {
                legalMoves.push_back(move);
            }

            undoMove(move);
        }

        return legalMoves;

    }


    void printState(const State& s) {
        cout << "=== State info ===" << endl;
        cout << "Captured piece: " << s.capturedPiece << endl;
        cout << "White can castle king-side: " << (s.wCastleKingState ? "Yes" : "No") << endl;
        cout << "White can castle queen-side: " << (s.wCastleQueenState ? "Yes" : "No") << endl;
        cout << "Black can castle king-side: " << (s.bCastleKingState ? "Yes" : "No") << endl;
        cout << "Black can castle queen-side: " << (s.bCastleQueenState ? "Yes" : "No") << endl;
        cout << "En passant square: (" << s.enpassantSquareState.first << ", " << s.enpassantSquareState.second << ")" << endl;
        cout << "==================" << endl;
    }

    int pieceValue(Piece p) {
        switch (p) {
            case WP: return 100;
            case WN: return 320;
            case WB: return 330;
            case WR: return 500;
            case WQ: return 900;
            case WK: return 20000;

            case BP: return -100;
            case BN: return -320;
            case BB: return -330;
            case BR: return -500;
            case BQ: return -900;
            case BK: return -20000;

            default: return 0;
        }
    }

    bool whiteCheckmated() {
        return (whiteToMove && generateLegalMoves().empty() && isKingInCheck(true));
    }

    bool blackCheckmated() {
        return (!whiteToMove && generateLegalMoves().empty() && isKingInCheck(false));
    }

    bool pat() {
        return ((whiteToMove && generateLegalMoves().empty() && !isKingInCheck(true))
                || (!whiteToMove && generateLegalMoves().empty() && !isKingInCheck(false)));
    }

    int evaluate() {
        int value = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                value += pieceValue(board[i][j]);
            }
        }

        if (whiteCheckmated()) {
            value -= 100000;
            }
        if (blackCheckmated()) {
            value += 100000;
        }

        return value;
    }

    int minimax(int depth,  int alpha, int beta) {
        const int infinity = 100000;

        vector<Move> legalMoves = generateLegalMoves();

        if (depth == 0 || legalMoves.empty()) {
            return evaluate();
        }

        if (whiteToMove) {
            int max_evaluate = -infinity;

            for (auto move:legalMoves) {
                doMove(move);
                int eval = minimax(depth - 1, alpha, beta);
                undoMove(move);

                max_evaluate = max(max_evaluate, eval);
                alpha = max(alpha, eval);

                if (beta <= alpha) {
                    break;
                    }
                }

            return max_evaluate;

        } else {
            int min_evaluate = infinity;

            for (auto move:legalMoves) {
                doMove(move);
                int eval = minimax(depth - 1, alpha, beta);
                undoMove(move);

                min_evaluate = min(min_evaluate, eval);
                beta = min(beta, eval);

                if (beta <= alpha) {
                    break;
                    }
            }
            return min_evaluate;
        }
    }

    Move bestMove(int depth) {
        int infinity = 10000000;
        Move bestM;
        int alpha = -100000;
        int beta = 100000;


        vector<Move> legalMoves = generateLegalMoves();

        int curr_evaluate = (whiteToMove ? -infinity:infinity);

        for (auto move:legalMoves) {
            doMove(move);
            int new_evaluate = minimax(depth-1, alpha, beta);
            undoMove(move);

            if (new_evaluate > curr_evaluate && whiteToMove) {
                curr_evaluate = new_evaluate;
                bestM = move;
            } 
            if (new_evaluate < curr_evaluate && !whiteToMove) {
                curr_evaluate = new_evaluate;
                bestM = move;
            }

        }
        return bestM;
    }

    void BestMoves(int depth) {
        for (int i = depth; i >= 0; i--) {
            Move bestM = bestMove(i);
            printMove(bestM);
            doMove(bestM);
        }

    }

    int charToInt(char inputMove) {
        inputMove = tolower(inputMove);
        switch(inputMove) {
            case 'a': return 0;
            case 'b': return 1;
            case 'c': return 2;
            case 'd': return 3;
            case 'e': return 4;
            case 'f': return 5;
            case 'g': return 6;
            case 'h': return 7;
            default: return -1;
        }
    }

    Move stringToMove(string inputMoveFrom, string inputMoveTo) {
        int rowFrom, colFrom, rowTo, colTo;
        colFrom = charToInt(inputMoveFrom[0]);
        colTo = charToInt(inputMoveTo[0]);
        rowFrom = 8 - (inputMoveFrom[1] - '0');
        rowTo = 8 - (inputMoveTo[1] - '0');

        return {rowFrom, colFrom, rowTo, colTo};
    }

    pair<bool, Move> moveExists(Move move) {
        vector<Move> legalMoves = generateLegalMoves();

        for (auto legalMove:legalMoves) {
            //cout << move.fromY << " " << legalMove.fromY << endl;
            if (move.fromY == legalMove.fromY && move.fromX == legalMove.fromX && move.toY == legalMove.toY && move.toX == legalMove.toX) {
                return {true, legalMove};
            }
        }
        return {false, {0, 0, 0, 0}};
    }
};

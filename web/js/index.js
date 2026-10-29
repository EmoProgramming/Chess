import createModule from "./chess.js";

const Module = await createModule();

// console.log(Object.keys(Module));

let whiteToMove = Module._isWhiteToMoveJS();
let clicked = false;
let moves = [];
let movesFromChosenPosition = [];

const board = document.getElementById("board");

const piecesFromCpp = {
    0: "",
    1: "♙",
    2: "♘",
    3: "♗",
    4: "♖",
    5: "♕",
    6: "♔",
    7: "♟",
    8: "♞",
    9: "♝",
    10: "♜",
    11: "♛",
    12: "♚"
};

// BOARD RENDERING

function updateSquare(row, col) {
    const piece = Module._getPieceJS(row, col);
    const square = document.getElementById(String(row)+String(col));
    square.textContent = piecesFromCpp[piece];
}

function updateBoard() {
    for (let row = 0; row < 8; row++) {
        for (let col = 0; col < 8; col++) {
            updateSquare(row, col);
        }
    }
}

function addSquare(row, col) {
    const square = document.createElement("div");
    if ((row + col) % 2 == 0) {
        square.classList.add("oddSquare");
    } else {
    square.classList.add("evenSquare");
    }

    square.classList.add("square");
    square.id = String(row)+String(col);

    board.appendChild(square);

    return square;
}

function changeOpacityOfSquareFromMoves(moves, opacity) {
    for (let i = 0; i < moves.length; i++) {
        const move = moves[i];
        const fromRow = move.fromY;
        const fromCol = move.fromX;
        const toRow = move.toY;
        const toCol = move.toX;
        const toSquare = document.getElementById(String(toRow)+String(toCol));
        const fromSquare = document.getElementById(String(fromRow)+String(fromCol));
        toSquare.style.opacity = opacity;
        fromSquare.style.opacity = opacity;
    }
}

function showMoves(moves) {
    changeOpacityOfSquareFromMoves(moves, "0.7");
}

function hideMoves(moves) {
    changeOpacityOfSquareFromMoves(moves, "1");
}

function createBoard() {

    for (let row = 0; row < 8; row++) {
        for (let col = 0; col < 8; col++) {

            const square = addSquare(row, col);

            updateSquare(row, col);

            handleClick(row, col);

        }
    }

}

// END OF BOARD RENDERING

// STATE HANDLING

function blackCheckmated() {
    return Module._blackCheckmatedJS();
}

function whiteCheckmated() {
    return Module._whiteCheckmatedJS();
}

function pat() {
    return Module._patJS();
}

function logClickInfo(row, col) {
    console.log("Klikol som na:", row, col);
    const result = Module._getPieceJS(row, col);
    console.log("Klikol som na:", piecesFromCpp[result]);
}

// END OF STATE HANDLING

// MOVES HANDLING

function getLegalMoves() {
    const pointer = Module._allocateMemory(256 * 6 * 4);

    const count = Module._generateLegalMovesJS(pointer);

    const moves = [];

    for (let i = 0; i < count; i++) {
        moves.push({
            fromY: Module.HEAP32[(pointer >> 2) + i * 6],
            fromX: Module.HEAP32[(pointer >> 2) + i * 6 + 1],
            toY: Module.HEAP32[(pointer >> 2) + i * 6 + 2],
            toX: Module.HEAP32[(pointer >> 2) + i * 6 + 3],
            type: Module.HEAP32[(pointer >> 2) + i * 6 + 4],
            promotionPiece: Module.HEAP32[(pointer >> 2) + i * 6 + 5]
        });
    }

    Module._freeMemory(pointer);

    return moves;
}

function filterMovesFromChosenPosition(row, col, moves) {
    const movesFromChosenPosition = [];
    for (let i = 0; i < moves.length; i++) {
        const move = moves[i];
        if (move.fromY == row && move.fromX == col) {
            movesFromChosenPosition.push(move);
        }
    }
    return movesFromChosenPosition;
}

function getIndexOfValidMove(clickedRow, clickedCol, moves) {
    
    for (let i = 0; i < moves.length; i++) {
        const move = moves[i];
        const toRow = move.toY;
        const toCol = move.toX;
        if (clickedRow == toRow && clickedCol == toCol) {
            console.log("sprav tah");
            return i;
        }
    }
    return -1;
}

function doMoveOnBoard(move) {
    const fromX = move.fromX;
    const fromY = move.fromY;
    const toX = move.toX;
    const toY = move.toY;
    const type = move.type;
    const piece = move.promotionPiece;
    Module._doMoveJS(fromX, fromY, toX, toY, type, piece);
    updateBoard();
}

function isValidIndex(indexOfMove) {
    return (indexOfMove != -1);
}

function getMove(indexOfMove) {
    return movesFromChosenPosition[indexOfMove];
}

function handleMove(indexOfMove) {
    const move = getMove(indexOfMove);
    doMoveOnBoard(move);
    whiteToMove = Module._isWhiteToMoveJS();
    moves = getLegalMoves();
}

// END OF MOVE HANDLING

// CLICK HANDLING

function handleClick(row, col) {
    const square = document.getElementById(String(row)+String(col));

    square.addEventListener("click", () => {

        logClickInfo(row, col);

        if (clicked) {

            hideMoves(movesFromChosenPosition);

            const indexOfMove = getIndexOfValidMove(row, col, movesFromChosenPosition);

            if (isValidIndex(indexOfMove)) {
                handleMove(indexOfMove);
            }

            clicked = false;

            if (blackCheckmated()) {
                console.log("BLACK CHECKMATED");
            }

            if (whiteCheckmated()) {
                console.log("white CHECKMATED");
            }

            if (pat()) {
                console.log("PAT");
            }

            console.log(Module._evaluateJS());

            console.log(Module._minimaxJS(4, -100000, 100000));

        } else {
                clicked = true;
                movesFromChosenPosition = filterMovesFromChosenPosition(row, col, moves);
                showMoves(movesFromChosenPosition);
            }
    });
}

// END OF CLICK HANDLING


function main() {
    moves = getLegalMoves();
    console.log(moves);
    createBoard();
}

main();

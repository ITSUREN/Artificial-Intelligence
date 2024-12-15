const readline = require('readline');

const MAXNODES = 1000;
const HorizontalRule = 30;

class GameState {
    constructor(board, blank) {
        this.board = board;
        this.blank = blank;
    }
}

class QueueNode {
    constructor(state, previous, move, depth) {
        this.state = state;
        this.previous = previous;
        this.move = move;
        this.depth = depth;
    }
}

let front = 0, rear = 0, vIndex = 0;
const queue = new Array(MAXNODES);
const visited = new Array(MAXNODES);

function isSameState(state1, state2) {
    for (let i = 0; i < state1.board.length; i++) {
        for (let j = 0; j < state1.board[i].length; j++) {
            if (state1.board[i][j] !== state2.board[i][j]) {
                return false;
            }
        }
    }
    return true;
}

function isVisited(current, visited) {
    for (let i = 0; i < vIndex; i++) {
        if (isSameState(current, visited[i].state)) {
            return true;
        }
    }
    return false;
}

function backtrace(node) {
    if (node === null) return;
    backtrace(node.previous);
    console.log(`Depth ${node.depth} [Move ${node.move}]`);
    printBoard(node.state);
}

function isSafe(x, y, size) {
    return x >= 0 && x < size && y >= 0 && y < size;
}

function swap(board, x1, y1, x2, y2) {
    const temp = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;
}

function printBoard(state) {
    for (let i = 0; i < state.board.length; i++) {
        console.log(state.board[i].join(' '));
    }
    console.log('');
}

function inputBoard(rl, prompt, callback) {
    const size = 3;
    const board = [];
    let blank = { x: 0, y: 0 };
    console.log(`Enter for ${prompt}:\n`);
    function inputElement(i, j) {
        if (i >= size) {
            return callback(new GameState(board, blank));
        }
        if (j >= size) {
            return inputElement(i + 1, 0);
        }

        rl.question(`Enter element for position [${i}][${j}]: `, (input) => {
            const element = parseInt(input);
            if (!board[i]) board[i] = [];
            board[i][j] = element;
            if (element === 0) {
                blank = { x: i, y: j };
            }
            inputElement(i, j + 1);
        });
    }

    inputElement(0, 0);
}

function solvePuzzle(initial, goal) {
    queue[rear] = new QueueNode(initial, null, "Initial State Inclusion", 0);
    rear++;

    while (front < rear && rear < MAXNODES) {
        const current = queue[front].state;
        visited[vIndex] = new QueueNode(current, queue[front].previous, queue[front].move, queue[front].depth);

        const depth = queue[front].depth;
        front++;
        vIndex++;

        if (isSameState(current, goal)) {
            console.log("\nBacktrace Path:\n");
            backtrace(visited[vIndex - 1]);
            console.log(`\nSolution Found at depth ${depth}\n\n`);
            return;
        }

        const { x, y } = current.blank;
        const dy = [-1, 1, 0, 0];
        const dx = [0, 0, -1, 1];
        const moves = ["Left", "Right", "Up", "Down"];

        for (let i = 0; i < 4; i++) {
            const newx = x + dx[i];
            const newy = y + dy[i];

            if (isSafe(newx, newy, current.board.length)) {
                const newState = new GameState(current.board.map(row => row.slice()), { x: newx, y: newy });
                swap(newState.board, x, y, newx, newy);

                if (!isVisited(newState, visited)) {
                    queue[rear] = new QueueNode(newState, visited[vIndex - 1], moves[i], depth + 1);
                    rear++;
                }
            }
        }
    }

    console.log("\nSolution Not Found within Limits.\n\n");
}

function main() {
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    inputBoard(rl, 'Initial', (initial) => {
        console.clear();
        printBoard(initial);
        inputBoard(rl, 'Goal', (goal) => {
            console.clear();
            printBoard(initial);
            printBoard(goal);
            for (let i = 0; i < HorizontalRule; i++) {
                process.stdout.write("[]");
            }
            solvePuzzle(initial, goal);
            rl.close();
        });
    });
}

// Call the main function to run the program
main();

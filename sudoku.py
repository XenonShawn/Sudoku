from sys import argv, stderr
from time import time

def main() -> None:
    # Get the board
    board = input_board()
    print("Input Board:")
    print_board(board)
    start = time()

    if check_board_validity(board):
        solve(board)
        print("Result:")
        print_board(board)
    else:
        print("Impossible Board")

    print("Time Taken", time() - start)

def solve(board) -> bool:
    # We start solving from the cell which has the least number of possible values
    sorted_possible_values = sorted(get_possible_values(board).items(), key=lambda x: len(x[1]))

    # No more empty cells, board is solved!
    if not sorted_possible_values:
        return True

    # Extract out the cell with the least number of possible values
    idx, possible_values = sorted_possible_values[0]
    for i in possible_values:
        board[idx] = i
        # For each possible value of the cell, try further solving it
        if solve(board):
            return True
            
    # All possible values don't work - reset the cell value back to None
    board[idx] = None
    return False

def input_board() -> list:
    board = list()
    with open(argv[1]) as f:
        for row in f:
            board += list(map(lambda x: int(x) if x.isdigit() else None, row[:9].ljust(9, ' ')))
    return board
    
def print_board(board) -> None:
    for i in range(9):
        print('|', end='')
        for j in range(9):
            number = board[9 * i + j]
            print(' ' if number is None else number, end='')
            print('|', end='')
        print()

def get_possible_values(board) -> dict:
    # Initialise with a full set of possible values, for each unfilled cell (ie v is None)
    result = {idx: {1, 2, 3, 4, 5, 6, 7, 8, 9} for idx, v in enumerate(board) if v is None}
    for idx, possible_values in result.items():
        row, col = divmod(idx, 9)

        # Check row and column
        for i in range(9):
            possible_values.discard(board[i * 9 + col])
            possible_values.discard(board[row * 9 + i])

        # Remove number from 3x3 square
        squareRow = row - row % 3
        squareCol = col - col % 3
        for i in range(squareRow, squareRow + 3):
            for j in range(squareCol, squareCol + 3):
                possible_values.discard(board[i * 9 + j])
    return result

def check_board_validity(board) -> bool:
    """Initially check if the board is valid. Not necessary, but helps speed up "solve" time for impossible boards."""
    for idx, cell in enumerate(board):
        if cell is None:
            continue
        
        row, col = divmod(idx, 9)
        for i in range(9):
            # Check row
            if i != col:
                if board[row * 9 + i] == cell:
                    return False
            # Check column
            if i != row:
                if board[i * 9 + col] == cell:
                    return False

        # Check 3x3 square
        squareRow = row - row % 3
        squareCol = col - col % 3
        for i in range(squareRow, squareRow + 3):
            for j in range(squareCol, squareCol + 3):
                if i != row or j != col:
                    if board[i * 9 + j] == cell:
                        return False
    return True 

if __name__ == "__main__":
    if len(argv) != 2:
        print(f"Usage: python {argv[0]} sudoku_file", file=stderr)
        exit(1)
    main()
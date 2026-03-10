def main():
    n = int(input())
    if n <= 0:
        return
    knight_row, knight_col = map(int, input().split())
    knight_possible_moves = (
        (2, 1), (1, 2), (-1, 2), (-2, 1),
        (-2, -1), (-1, -2), (1, -2), (2, -1)
    )
    knight_next_positions = []
    chess_table: list[list[int | str]] = [[0] * n for _ in range(n)]

    if (knight_row >= n) or (knight_row < 0) or (knight_col >= n) or (knight_col < 0):
        return

    chess_table[knight_row][knight_col] = 'K'

    for move_row, move_col in knight_possible_moves:
        if (0 <= knight_row + move_row < n) and (0 <= knight_col + move_col < n):
            knight_next_positions.append((knight_row + move_row, knight_col + move_col))

    for next_row, next_col in knight_next_positions:
        move_count = 0
        for move_row, move_col in knight_possible_moves:
            if (0 <= next_row + move_row < n) and (0 <= next_col + move_col < n):
                if move_row + next_row == knight_row and move_col + next_col == knight_col:
                    continue
                move_count += 1
        chess_table[next_row][next_col] = move_count

    for i, row in enumerate(chess_table):
        print(*row, end='\n' if i != len(chess_table) - 1 else '')

if __name__ == '__main__':
    main()
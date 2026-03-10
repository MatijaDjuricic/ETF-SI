def main():
    empty_field = '_'
    coin_field = 'o'
    player_position_field = 'x'
    move_left = 'L'
    move_right = 'R'
    map_input = list(input().split())
    movements = list(input().split())
    player_position = int(input())
    map_length = len(map_input)
    coin_count = 0
    if player_position < 0 or player_position >= map_length:
        return
    for move in movements:
        if coin_field == map_input[player_position]:
            coin_count += 1
            map_input[player_position] = empty_field
        if move == move_left:
            if player_position == 0:
                continue
            player_position -= 1
        elif move == move_right:
            if player_position == map_length - 1:
                continue
            player_position += 1
    if coin_field == map_input[player_position]:
        coin_count += 1
        map_input[player_position] = empty_field
    map_input[player_position] = player_position_field
    print(f"{' '.join(field for field in map_input)}\n{coin_count}", end='')
if __name__ == '__main__':
    main()
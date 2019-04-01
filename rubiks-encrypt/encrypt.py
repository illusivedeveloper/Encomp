import move


def encrypt(cube, moves):
    for rotation in moves:
        if rotation.type == move.Move.rotation_row:
            cube.rotate_row(rotation.row_or_column, rotation.times)
        elif rotation.type == move.Move.rotation_column:
            cube.rotate_column(rotation.row_or_column, rotation.times)
        elif rotation.type == move.Move.rotation_clockwise:
            cube.rotate_clockwise()
        elif rotation.type == move.Move.rotation_anti_clockwise:
            cube.rotate_anti_clockwise()


def decrypt(cube, moves):
    for rotation in reversed(moves):
        if rotation.type == move.Move.rotation_row:
            cube.rotate_row(rotation.row_or_column, cube.matrix.__len__() - rotation.times)
        elif rotation.type == move.Move.rotation_column:
            cube.rotate_column(rotation.row_or_column, cube.matrix[0].__len__() - rotation.times)
        elif rotation.type == move.Move.rotation_clockwise:
            cube.rotate_anti_clockwise()
        elif rotation.type == move.Move.rotation_anti_clockwise:
            cube.rotate_clockwise()

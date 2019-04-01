from enum import Enum
import random


class Move(Enum):
    rotation_row = 1
    rotation_column = 2
    rotation_clockwise = 3
    rotation_anti_clockwise = 4


class Movement:
    type = None
    times = 0
    row_or_column = 0

    def __init__(self, rows, columns):
        self.type = random.choice(list(Move))
        if self.type == Move.rotation_column:
            self.times = random.randint(1, columns-1)
            self.row_or_column = random.randint(0, columns-1)
        elif self.type == Move.rotation_row:
            self.times = random.randint(1, rows-1)
            self.row_or_column = random.randint(0, rows-1)
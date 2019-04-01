import random
import numpy
import sys
import copy


class Cube:
    matrix = []

    def __init__(self, *args):
        if args.__len__() == 2:
            rows = args[0]
            columns = args[1]
            self.matrix = [[0 for x in range(columns)] for x in range(rows)]
            for i in range(rows):
                for j in range(columns):
                    self.matrix[i][j] = random.randint(0, 255)
        elif args.__len__() == 1:
            if type(args[0]) is list:
                self.matrix = copy.deepcopy(args[0])
            else:
                self.matrix = copy.deepcopy(args[0].matrix)
        else:
            sys.exit('Too few arguments given to Cube constructor.')

    def print_cube(self):
        for i in range(self.matrix.__len__()):
            for j in range(self.matrix[0].__len__()):
                if self.matrix[i][j] < 100:
                    print('0', end="")
                if self.matrix[i][j] < 10:
                    print('0', end="")

                print(str(self.matrix[i][j]) + " ", end="")
            print()

    def rotate_clockwise(self):
        self.matrix = numpy.rot90(self.matrix, -1)

    def rotate_anti_clockwise(self):
        self.matrix = numpy.rot90(self.matrix, 1)

    def rotate_column(self, column, times):
        self.matrix = numpy.transpose(self.matrix)
        numpy.roll(self.matrix[column], times)
        self.matrix = numpy.transpose(self.matrix)

    def rotate_row(self, row, times):
        self.matrix[row] = numpy.roll(self.matrix[row], times)
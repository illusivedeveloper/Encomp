import rubiks
import move
import numpy
from scipy import misc
import matplotlib.pyplot as plt
import encrypt
import urllib.request

url = urllib.request.urlretrieve("http://i.imgur.com/v0cB99O.jpg", "v0cB99O.jpg")
img = misc.imread(url[0], flatten=True)[0:1000, 0:1000]

increment = 250

moves = []
for i in range(10000):
    moves.append(move.Movement(increment, increment))

iterations = range(0, 1000, increment)

encrypted_image = numpy.array([x[:] for x in [[0] * 1000] * 1000])
decrypted_image = numpy.array([x[:] for x in [[0] * 1000] * 1000])

for X in iterations:
    for Y in iterations:
        cube = rubiks.Cube(list(img[X:X + increment, Y:Y + increment]))
        encrypt.encrypt(cube, moves)
        encrypted_image[X:X + increment, Y:Y + increment] = cube.matrix
        encrypt.decrypt(cube, moves)
        decrypted_image[X:X + increment, Y:Y + increment] = cube.matrix

img = img.astype(numpy.uint16)
encrypted_image = encrypted_image.astype(numpy.uint16)
decrypted_image = decrypted_image.astype(numpy.uint16)

# Assert that the dimensions of original and decrypted are equal
assert img.__len__() == decrypted_image.__len__()
assert img.__len__() == decrypted_image[0].__len__()

# Assert that each pixel in original and decrypted are the same
dimensions = [img.__len__(), img[0].__len__()]
for i in range(dimensions[0]):
    for j in range(dimensions[1]):
        assert img[i][j] == decrypted_image[i][j]

# Plot all three figures showing the original, encrypted, and decrypted
f1 = plt.figure()
plt.imshow(img, cmap='gray')
plt.title("Original")

f2 = plt.figure()
plt.imshow(encrypted_image, cmap='gray')
plt.title("Encrypted")

f3 = plt.figure()
plt.imshow(decrypted_image, cmap='gray')
plt.title("Decrypted")

plt.show()
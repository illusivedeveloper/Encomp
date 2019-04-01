convert IllusiveMan.jpg -set colorspace Gray -separate -average IllusiveMan2.jpg
convert lena.png -set colorspace Gray -separate -average lena2.png
convert blackbuck.png -set colorspace Gray -separate -average blackbuck2.png
convert baboon.png -set colorspace Gray -separate -average baboon2.png
convert cat1.png -set colorspace Gray -separate -average cat2.png
convert IllusiveMan.png -set colorspace Gray -separate -average IllusiveMan2.png
convert Mona_Lisa.png -set colorspace Gray -separate -average Mona_Lisa2.png


convert lena2.png -encipher passphrase.txt lena3.png
convert blackbuck2.png -encipher passphrase.txt blackbuck3.png
convert baboon2.png -encipher passphrase.txt baboon3.png
convert cat2.png -encipher passphrase.txt cat3.png
convert IllusiveMan2.jpg -encipher passphrase.txt IllusiveMan3.jpg
convert IllusiveMan2.png -encipher passphrase.txt IllusiveMan3.png
convert Mona_Lisa2.png -encipher passphrase.txt Mona_Lisa3.png

convert lena3.png -decipher passphrase.txt lena4.png
convert blackbuck3.png -decipher passphrase.txt blackbuck4.png
convert baboon3.png -decipher passphrase.txt baboon4.png
convert cat3.png -decipher passphrase.txt cat4.png
convert IllusiveMan3.jpg -decipher passphrase.txt IllusiveMan4.jpg
convert IllusiveMan3.png -decipher passphrase.txt IllusiveMan4.png
convert Mona_Lisa3.png -decipher passphrase.txt Mona_Lisa4.png

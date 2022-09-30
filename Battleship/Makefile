
quadtree: main.o utils.o game.o io.o player.o quadtree.o MAPQUADTREE cell.o piece.o bitmap.o point.o
	gcc -std=c99 *.o -o game

matrix: main.o utils.o game.o io.o player.o MAPMATRIX cell.o piece.o bitmap.o
	gcc -std=c99 *.o -o game

main.o: main.c
	gcc -std=c99 -Wall -c main.c

utils.o: utils.c utils.h
	gcc -std=c99 -Wall -c utils.c

game.o: game.c game.h
	gcc -std=c99 -Wall -c game.c

io.o: io.c io.h
	gcc -std=c99 -Wall -c io.c

player.o: player.c player.h
	gcc -std=c99 -Wall -c player.c

quadtree.o: quadtree.c quadtree.h
	gcc -std=c99 -Wall -c quadtree.c

MAPMATRIX: map.c map.h
	gcc -std=c99 -Wall -c -D MATRIX map.c

MAPQUADTREE: map.c map.h
	gcc -std=c99 -Wall -c map.c

cell.o: cell.c cell.h
	gcc -std=c99 -Wall -c cell.c

piece.o: piece.c piece.h
	gcc -std=c99 -Wall -c piece.c

bitmap.o: bitmap.c bitmap.h
	gcc -std=c99 -Wall -c bitmap.c

point.o: point.c point.h
	gcc -std=c99 -Wall -c point.c

clean:
	rm *.o game

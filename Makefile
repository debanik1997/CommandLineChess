CC = g++
0;136;0cCONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)


chess: main.o Board.o Chess.o CreatePiece.o Bishop.o Queen.o King.o Knight.o Pawn.o Rook.o
	$(CC) -o chess main.o Board.o Chess.o CreatePiece.o Bishop.o Queen.o King.o Knight.o Pawn.o Rook.o $(CFLAGS)

Board.o: Board.cpp Bishop.h Board.h King.h Knight.h Mystery.h Pawn.h Piece.h CreatePiece.h Queen.h Rook.h Terminal.h
	$(CC) -c Board.cpp $(CFLAGS)

Bishop.o: Bishop.cpp Piece.h Bishop.h
	$(CC) -c Bishop.cpp $(CFLAGS)

Chess.o: Chess.cpp Board.h Chess.h Piece.h
	$(CC) -c Chess.cpp $(CFLAGS)

Queen.o: Queen.cpp Queen.h Piece.h
	$(CC) -c Queen.cpp $(CFLAGS)

King.o: King.cpp Piece.h King.h
	$(CC) -c King.cpp $(CFLAGS)

Rook.o: Rook.cpp Rook.h Piece.h
	$(CC) -c Rook.cpp $(CFLAGS)

Pawn.o: Pawn.cpp Pawn.h Piece.h
	$(CC) -c Pawn.cpp $(CFLAGS)

Knight.o: Knight.cpp Knight.h Piece.h
	$(CC) -c Knight.cpp $(CFLAGS)

CreatePiece.o: CreatePiece.cpp Bishop.h Board.h Chess.h King.h Knight.h Mystery.h Pawn.h Piece.h CreatePiece.h Queen.h Rook.h
	$(CC) -c CreatePiece.cpp $(CFLAGS)

main.o: main.cpp Bishop.h Board.h Chess.h King.h Knight.h Mystery.h Pawn.h Piece.h CreatePiece.h Queen.h Rook.h
	$(CC) -c main.cpp $(CFLAGS)

.PHONY: clean all
clean:
	rm -f *.o chess test1

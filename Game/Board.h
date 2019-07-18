#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include <map>
#include <vector>
#include "Piece.h"
#include "Pawn.h"
#include "Mystery.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"



class Board
{
	// Throughout, we will be accessing board positions using an std::pair< char , char >.
	// The assumption is that the first value is the row with values in {'A','B','C','D','E','F','G','H'} (all caps)
	// and the second is the column, with values in {'1','2','3','4','5','6','7','8'}

public:
	// Default constructor
	Board( void );

	//Board deconstructor
	~Board( void ) {
          std::vector<std::pair<char,char>> to_delete;
	  for(std::map<std::pair<char,char>, Piece*>::iterator it = _occ.begin();
	      it != _occ.end(); ++it) {
	    to_delete.push_back(it->first);
	  }
	  for(std::vector<std::pair<char,char>>::iterator it = to_delete.begin();
	      it !=  to_delete.end(); ++it) {
	    _occ.erase(*it);
	  }
	}

	// Board copy constructor
	Board(const Board& copy) : _occ(copy._occ) {
	 Board* board = new Board();
	  for(std::map<std::pair<char,char> , Piece*>::iterator it = _occ.begin();
	      it != _occ.end(); ++it) {
	    board->add_piece(it->first, it->second->to_ascii());
	  }
	}

	// overloading = operator
	Board& operator=(const Board& o) {
	  std::vector<std::pair<char,char>> to_delete;
          for(std::map<std::pair<char,char>, Piece*>::iterator it = _occ.begin();
              it != _occ.end(); ++it) {
            to_delete.push_back(it->first);
          }
          for(std::vector<std::pair<char,char>>::iterator it = to_delete.begin();
              it !=  to_delete.end(); ++it) {
            _occ.erase(*it);
          }
	  _occ = o._occ;
	  Board* board = new Board();
	  for(std::map<std::pair<char,char>, Piece*>::const_iterator it = _occ.cbegin();
	      it != _occ.cend(); ++it) {
	    board->add_piece(it->first, it->second->to_ascii());
	  }
	  return *this;
	}

	// Returns a const pointer to the piece at a prescribed location if it exists, or a NULL pointer if there is nothing there.
	const Piece* operator() ( std::pair< char , char > position ) const;

	// Attempts to add a new piece with the specified designator, at the given location.
	// Returns false if:
	// -- the designator is invalid,
	// -- the specified location is not on the board, or
	// -- if the specified location is occupied
	bool add_piece( std::pair< char , char > position , char piece_designator );

	//Attempts to remove a piece from the board.
	//It return false if there is no piece there.
	bool delete_piece(std::pair<char,char> position);

	//Returns the occurunce of all of the pieces on the board.
	//Used when outside of Board since _occ is private.
	std::map<std::pair<char,char>, Piece*> get_occ() const;

	//Checks for any pieces in between a move being made.
	bool check_for_obstruction(char piece, int row_dif, int col_dif, std::pair<char,char> start) const; 


	// Displays the board by printing it to stdout
	void display( void ) const;

	// Returns true if the board has the right number of kings on it
	bool has_valid_kings( void ) const;

private:
	// The sparse map storing the pieces, keyed off locations
	std::map< std::pair< char , char > , Piece* > _occ;
};

// Write the board state to an output stream
std::ostream& operator << ( std::ostream& os , const Board& board );

std::istream& operator >> (std::istream& is, Board& board);

#endif // BOARD_H

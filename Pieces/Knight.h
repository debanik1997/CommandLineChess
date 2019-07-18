#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include <stdlib.h>
class Knight : public Piece
{
public:

  ~Knight ( void ) {}
  bool legal_move_shape( std::pair< char , char > start , std::pair< char , char > end ) const;
	
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	char to_ascii( void ) const { return is_white() ? 'N' : 'n'; }

private:
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Knight( bool is_white ) : Piece( is_white ) {}

	friend Piece* create_piece( char );
};

#endif // KNIGHT_H

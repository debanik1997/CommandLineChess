/////////////////////////////////
// DO NOT MODIFY THIS FILE!!!! //
/////////////////////////////////
#ifndef MYSTERY_H
#define MYSTERY_H

#include "Piece.h"

class Mystery : public Piece
{
public:
  bool legal_move_shape( std::pair< char , char > start , std::pair< char , char > end ) const {
    char col_start = start.first;
  char row_start = start.second;
  char col_end = end.first;
  char row_end = end.second;
  char col_dif = abs(col_start - col_end);
  char row_dif = abs(row_start - row_end);
  if ((col_dif == 3 && row_dif == 3)) { // || (col_dif == 2 && row_dif == 3)) {                                                                                                                             
    return true;
  }
  else {
    return false;
  }
  }

	char to_ascii( void ) const { return is_white() ? 'M' : 'm'; }

private:
	Mystery( bool is_white ) : Piece( is_white ) {}

	friend Piece* create_piece( char );
};

#endif // MYSTERY_H

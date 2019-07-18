#include <iostream>
#include <string>
#include "Piece.h"
#include "Rook.h"

using std::pair;
using std::make_pair;
using std::string;

bool Rook::legal_move_shape(std::pair< char , char > start , std::pair< char ,char > end) const {
  char col_start = start.first;
  char row_start = start.second;
  char col_end = end.first;
  char row_end = end.second;
  if(col_start == col_end && row_start == row_end) {
    return false;
  }
  if (col_start == col_end  || row_start == row_end) {
    return true;
  }
  else {
    return false;
  }
}

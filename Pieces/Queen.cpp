#include <iostream>
#include <string>
#include "Piece.h"
#include "Queen.h"

using std::pair;
using std::make_pair;
using std::string;

bool Queen::legal_move_shape(std::pair< char , char > start , std::pair< char , char > end) const {
  char col_start = start.first;
  char row_start = start.second;

  char col_end = end.first;
  char row_end = end.second;
  char col_dif = abs(col_start - col_end);
  char row_dif = abs(row_start - row_end);

  if(col_dif == 0 && row_dif == 0) {
    return false;
  }
  if (col_dif == row_dif || col_start == col_end  || row_start == row_end) {
    return true;
  }
  else {
    return false;
  }
}


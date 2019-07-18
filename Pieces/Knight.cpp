#include <iostream>
#include <string>
#include "Piece.h"
#include "Knight.h"

using std::pair;
using std::make_pair;
using std::string;

bool Knight::legal_move_shape(std::pair< char , char > start , std::pair< char , char > end) const {
  char col_start = start.first;
  char row_start = start.second;
  char col_end = end.first;
  char row_end = end.second;
  char col_dif = abs(col_start - col_end);
  char row_dif = abs(row_start - row_end);
  if ((col_dif == 2 && row_dif == 1) || (col_dif == 1 && row_dif == 2)) {
    return true;
  }
  else {
    return false;
  }
}

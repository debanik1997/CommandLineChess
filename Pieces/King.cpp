#include <iostream>
#include <string>
#include "Piece.h"
#include "King.h"

using std::pair;
using std::make_pair;
using std::string;

bool King::legal_move_shape(std::pair< char , char > start , std::pair< char , char > end) const {
  char col_start = start.first;
  char row_start = start.second;

   char col_end = end.first;
   char row_end = end.second;

   char col_dif = abs(col_start - col_end);
   char row_dif = abs(row_start - row_end);

   if (col_dif > 1 || row_dif > 1) {
     return false;
   }
   if(col_dif == 0 && row_dif == 0) {
     return false;
   }
   else {
     return true;
   }
}



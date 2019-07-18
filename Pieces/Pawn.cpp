#include <iostream>
#include <string>
#include "Piece.h"
#include "Pawn.h"

using std::pair;
using std::make_pair;
using std::string;

bool Pawn::legal_move_shape(std::pair< char , char > start , std::pair< char , char > \
end) const {
  char col_start = start.first;
  char row_start = start.second;
  char col_end = end.first;
  char row_end = end.second;
  int col_dif = col_end - col_start;
  int row_dif = row_end - row_start;
  if (col_dif != 0) {
    return false;
  }
  if(row_dif == 0) {
    return false;
  }
  if(this->is_white() && row_dif < 0) {
    return false;
  }
  if(!(this->is_white()) && row_dif > 0) {
    return false;
  }
  if(this->is_white() && row_start == '2') {
    if(row_dif > 2) {
      return false;
    }
    return true;
  }
  if(!(this->is_white()) && row_start == '7') {
    if(row_dif < -2) {
      return false;
    }
    return true;
  }
  if(abs(row_dif) > 1) {
    return false;
  }
  return true;
}

bool Pawn::legal_capture_shape(std::pair< char , char > start , std::pair< char ,char > end )const {
  int col_dif = end.first - start.first;
  int row_dif = end.second - start.second;
  if(row_dif == 1 && (col_dif == -1 || col_dif == 1) && this->is_white()) {
    return true;
  }
  if(row_dif == -1 && (col_dif == -1 || col_dif == 1) && !this->is_white()) {
    return true;
  }
  return false;
}



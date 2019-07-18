#include <iostream>
#include <utility>
#include <map>
#include <string>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"

using std::map;
using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::make_pair;


/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Board::Board( void ){}

const Piece* Board::operator()( std::pair< char , char > position ) const
{
  map<pair<char,char> , Piece*>::const_iterator it = _occ.find(position);
  if(it == _occ.end()) {
    return NULL;
  }
  else {
    return it->second;
  }
}

bool Board::add_piece( std::pair< char , char > position , char piece_designator )
{
  if(position.first < 'A' || position.first > 'H') {
    return false;
  }
  if(position.second < '1' || position.second > '8') {
    return false;
  }
  if(_occ.find(position) != _occ.end()) {
    return false;
  }
  if(create_piece(piece_designator) == NULL) {
    return false;
  }
   _occ[ position ] = create_piece( piece_designator );
   return true;
}

bool Board::delete_piece(std::pair<char,char> position) {
  if(_occ.find(position) != _occ.end()) {
    Piece* piece = _occ.find(position)->second;
    _occ.erase(position);
    return true;
  }
  else {
    return false;
  }
}

std::map<std::pair<char,char>, Piece*> Board::get_occ() const{
  std::map<std::pair<char,char>, Piece*> occ;
  for(std::map<std::pair<char,char>, Piece*>::const_iterator it = _occ.cbegin();
      it != _occ.cend();
      ++it) {
    occ[it->first] = it->second;
  }
  return occ;
}


bool Board::has_valid_kings( void ) const
{
  bool whiteK = false;
  bool blackK = false;
  int numKing = 0;
  for(map<pair<char,char>, Piece*>::const_iterator it = _occ.cbegin(); it != _occ.end(); ++it) {
    char  ascii = it->second->to_ascii();
    if(ascii == 'K') {
      whiteK = true;
      numKing++;
    }
    if(ascii == 'k') {
      blackK = true;
      numKing++;
    }
  }
  if(whiteK && blackK && numKing == 2) {
    return true;
  }
  return false;
}

void unicode(char c) {
  switch(c) {
      case 'K' :
	cout << "♔ ";
	break;
      case 'Q' :
        cout << "♕ ";
        break;
      case 'R' :
        cout << "♖ ";
        break;
      case 'B' :
        cout << "♗ ";
        break;
      case 'N' :
        cout << "♘ ";
        break;
      case 'P' :
        cout << "♙ ";
        break;
      case 'k' :
        cout << "♚ ";
        break;
      case 'q' :
        cout << "♛ ";
        break;
      case 'r' :
	cout << "♜ ";
	break;
      case 'b' :
	cout << "♝ ";
	break;
      case 'n' :
	cout << "♞ ";
	break;
      case 'p' :
	cout << "♟ ";
        break;
   	
  default :
    cout << c << " ";
    break;
  }
}


void Board::display( void ) const {
  int color = 0;
  cout << " " <<  endl;
  cout << "  A B C D E F G H" << endl;
  for(char r= '8'; r >= '1'; r--) {
    cout << r << " ";
    for(char c= 'A'; c <= 'H'; c++) {
      pair<char, char> p = make_pair(c, r);
      if (color % 2) {
	Terminal::color_bg(Terminal::BLACK);
      } else {
	Terminal::color_bg(Terminal::RED);
      }
      color++;
      if (_occ.find(p) != _occ.end()) { //piece exists
	const Piece* piece = _occ.at(p);
	char c = piece->to_ascii();
	unicode(c);

      } else {
	cout << "  ";
      }
    }
    color++;
    Terminal::set_default();
    cout << endl;
  }
  Terminal::set_default();
  cout << endl;
}

bool Board::check_for_obstruction(char piece, int row_dif, int col_dif, std::pair<char,char> start) const {
  std::pair<char,char> middle;
  if(piece == 'n'|| piece == 'N') {
    return true;
  }
  if((piece == 'M' || piece == 'm') && (abs(row_dif) != abs(col_dif)) && (col_dif != 0 && row_dif != 0)) {
    return true;
  }
  if(col_dif == 0 && row_dif != 0) {
    if(row_dif < 0) {
      for(int i = 1; i < abs(row_dif); i++) {
        middle = make_pair(start.first , start.second - i);
	if(_occ.find(middle) != _occ.end()) {
          return false;
        }
      }
    }
    else {
      for(int i = 1; i < row_dif; i++) {
        middle = make_pair(start.first , start.second + i);
        if(_occ.find(middle) != _occ.end()) {
          return false;
        }
      }
    }
  }
  if(col_dif != 0 && row_dif == 0) {
    if(col_dif < 0) {
      for(int i = 1; i < abs(col_dif); i++) {
        middle = make_pair(start.first - i , start.second);
        if(_occ.find(middle) != _occ.end()) {
          return false;
        }
      }
    }
    for(int i = 1; i < col_dif; i++) {
      middle = make_pair(start.first + i , start.second);
      if(_occ.find(middle) != _occ.end()) {
        return false;
      }
    }
  }
  if(col_dif != 0 && row_dif != 0) { //Checks for diagonal movement
    if(col_dif < 0 && row_dif < 0) { //Moving towards bottom left corner
      for(int i = 1; i < abs(col_dif); i++) {
        middle = make_pair(start.first - i , start.second - i);
        if(_occ.find(middle) != _occ.end()) {
          return false;
        }
      }
    }
    if(col_dif < 0 && row_dif > 0) { //Moving toward top left corner
      for(int i = 1; i < abs(col_dif); i++) {
        middle = make_pair(start.first - i , start.second + i);
        if(_occ.find(middle) != _occ.end()) {
	   return false;
        }
      }
    }
    if(col_dif > 0 && row_dif < 0) { //Moving towards bottom right corner
      for(int i = 1; i < abs(col_dif); i++) {
        middle = make_pair(start.first + i , start.second - i);
        if(_occ.find(middle) != _occ.end()) {
          return false;
        }
      }
    }
    if(col_dif > 0 && row_dif > 0) { //Moving towards top right corner
      for(int i = 1; i < abs(col_dif); i++) {
        middle = make_pair(start.first + i , start.second + i);
        if(_occ.find(middle) != _occ.end()) {
          return false;
        }
      }
    }
  }
  return true;
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator << ( std::ostream& os , const Board& board )
{
	for( char r='8' ; r>='1' ; r-- )
	{
		for( char c='A' ; c<='H' ; c++ )
		{
			const Piece* piece = board( std::pair< char , char >( c , r ) );
			if( piece ) os << piece->to_ascii();
			else        os << '-';
		}
		os << std::endl;
	}
	return os;
}

std::istream& operator >> (std::istream& is, Board& board) {

  // Read board from file
  char p;
  for( char r='8' ; r>='1' ; r-- ) {
    for( char c='A' ; c<='H' ; c++ ) {
      is.get(p);
      if (p == '\n') {
        is.get(p);
      }

      board.delete_piece(std::pair< char , char >( c , r ));
      if (p != '-') {
        board.add_piece(std::pair< char , char >( c , r ), p);
      }
    }
  }

  return is;
}

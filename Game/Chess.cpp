#include "Chess.h"
#include <vector>

using std::make_pair;
using std::pair;
using std::vector;

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Chess::Chess( void ) : _turn_white( true )
{
	// Add the pawns
	for( int i=0 ; i<8 ; i++ )
	{
		_board.add_piece( std::pair< char , char >( 'A'+i , '1'+1 ) , 'P' );
		_board.add_piece( std::pair< char , char >( 'A'+i , '1'+6 ) , 'p' );
	}

	// Add the rooks
	_board.add_piece( std::pair< char , char >( 'A'+0 , '1'+0 ) , 'R' );
	_board.add_piece( std::pair< char , char >( 'A'+7 , '1'+0 ) , 'R' );
	_board.add_piece( std::pair< char , char >( 'A'+0 , '1'+7 ) , 'r' );
	_board.add_piece( std::pair< char , char >( 'A'+7 , '1'+7 ) , 'r' );

	// Add the knights
	_board.add_piece( std::pair< char , char >( 'A'+1 , '1'+0 ) , 'N' );
	_board.add_piece( std::pair< char , char >( 'A'+6 , '1'+0 ) , 'N' );
	_board.add_piece( std::pair< char , char >( 'A'+1 , '1'+7 ) , 'n' );
	_board.add_piece( std::pair< char , char >( 'A'+6 , '1'+7 ) , 'n' );

	// Add the bishops
	_board.add_piece( std::pair< char , char >( 'A'+2 , '1'+0 ) , 'B' );
	_board.add_piece( std::pair< char , char >( 'A'+5 , '1'+0 ) , 'B' );
	_board.add_piece( std::pair< char , char >( 'A'+2 , '1'+7 ) , 'b' );
	_board.add_piece( std::pair< char , char >( 'A'+5 , '1'+7 ) , 'b' );

	// Add the kings and queens
	_board.add_piece( std::pair< char , char >( 'A'+3 , '1'+0 ) , 'Q' );
	_board.add_piece( std::pair< char , char >( 'A'+4 , '1'+0 ) , 'K' );
	_board.add_piece( std::pair< char , char >( 'A'+3 , '1'+7 ) , 'q' );
	_board.add_piece( std::pair< char , char >( 'A'+4 , '1'+7 ) , 'k' );
}

bool Chess::make_move( std::pair< char , char > start , std::pair< char , char > end )
{
  //We use the .get_occ() in order to get the map of the board since
  //that is a private variable. 
  std::map<std::pair<char,char>, Piece*> occ = _board.get_occ();
  
  int col_dif = end.first - start.first;
  int row_dif = end.second - start.second;

  //This block of code checks for things that would would make the move illegal.
  //Making the move first is not necessary to check for these.
  if(occ.find(start) == occ.end()) { //Checks if there is an actual piece at start.
    return false;
  }
  if(_turn_white) { // Checks that you don't move a black piece on white turn.
    if(!occ.find(start)->second->is_white()) {
      return false;
    }
  }
  if(!(_turn_white)) {// Checks that you don't move a white piece on black turn.
    if(occ.find(start)->second->is_white()) {
      return false;
    }
  }
  if(end.first < 'A' || end.first > 'H') {//Checks if your ending is in a real column.
    return false;
  }
  if(end.second < '1' || end.second > '8') {//Checks if your end is in a real row.
    return false;
  }

  //piece_char is the character that the piece we are moving is.
  char piece_char = occ.find(start)->second->to_ascii();

  //piece is the actual piece that we are moving.
  Piece* piece = occ.find(start)->second;

  //Checks to see if there are any pieces in the way of the move being made
  //This method is defined in Board.cpp
  bool obstruction = _board.check_for_obstruction(piece_char, row_dif, col_dif, start);
  if(!obstruction) {
    return false;
  }
  

  //Checks if there is a piece to be captured at the ending location.
  std::map<std::pair<char,char> , Piece*>::iterator captured = occ.find(end);

  //If no piece is found just call normal legal move shape.
  if(occ.find(end) == occ.end()) {
    if(!(piece->legal_move_shape(start,end))) {
      return false;
    }
  }
  
  //If there is a piece and the ending call for a legal capture shape.
  //If the colors of the pieces are the same then the capture is false.
  else {
    if(piece->is_white() == occ.find(end)->second->is_white()) {
      return false;
    }
    if(!(piece->legal_capture_shape(start,end))) {
      return false;
    }
     _board.delete_piece(end);
  }
  _board.add_piece(end , piece->to_ascii());

  //This block will do the pawn promotion
  if(piece->to_ascii() == 'P' || piece->to_ascii() == 'p') {
    if(piece->is_white() && end.second == '8') {
      _board.delete_piece(end);
      _board.add_piece(end, 'Q');
    }
    if(!piece->is_white() && end.second == '1') {
      _board.delete_piece(end);
      _board.add_piece(end, 'q');
      }
  }
  _board.delete_piece(start);

  //Checks to see if we are still in check after the move has been made.
  //Reverts move if we are still in check.
  if(in_check(_turn_white)) {
    _board.delete_piece(end);
    _board.add_piece(start, piece->to_ascii());
    if(captured != occ.end()) {
      _board.add_piece(end, captured->second->to_ascii());
    }
    return false;
  }
   _turn_white = !_turn_white;
   return true;
}

// Similar to in_check, but allows copies of Board to be passed
bool Chess::in_check_copy( bool white , Board& board ) const {
  std::pair<char,char> w_king_loc, b_king_loc;
  int col_dif, row_dif;
  bool obstruction;

  std::map<std::pair<char,char>, Piece*> occ = board.get_occ();

  //Loops to find locations for the white and black king.
  for(std::map<std::pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
        it != occ.cend(); ++it) {
    if(it->second->to_ascii() == 'K') {
      w_king_loc = it->first;
    }
    if(it->second->to_ascii() == 'k') {
      b_king_loc = it->first;
    }
  }

 //If it's white's turn, check if there is a piece that can attack the white king on the black side.
  if(white) {
    for(std::map<std::pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
        it != occ.cend(); ++it) {
      if(!it->second->is_white()) {
	 int col_dif = w_king_loc.first - it->first.first;
        int row_dif = w_king_loc.second - it->first.second;
        obstruction = board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif,
 it->first);
        if(obstruction) {
          if(it->second->legal_capture_shape(it->first,w_king_loc)) {
            return true;
	  }
	}
      }
    }
  }

  // If it's black's turn, check if there is a piece that can attack the black king on the white side. 
  else {
    for(std::map<std::pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
        it != occ.cend(); ++it) {
      if(it->second->is_white()) {
        int col_dif = b_king_loc.first - it->first.first;
        int row_dif = b_king_loc.second - it->first.second;
        obstruction = board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif,
 it->first);
        if(obstruction) {
          if(it->second->legal_capture_shape(it->first,b_king_loc)) {
            return true;
          }
        }
      }
    }
 }
  return false;
}


//This method is used to see is the board is in check or not.
//Returns true if we are in check and false if not. 
bool Chess::in_check( bool white ) const
{
  std::map<std::pair<char,char>, Piece*> occ = _board.get_occ();
  std::pair<char,char> w_king_loc, b_king_loc;
  int col_dif, row_dif;
  bool obstruction;

  //Loops to find locations for the white and black king.
  for(std::map<std::pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
        it != occ.cend(); ++it) {
    if(it->second->to_ascii() == 'K') {
      w_king_loc = it->first; 
    }
    if(it->second->to_ascii() == 'k') {
      b_king_loc = it->first;
    }
  }
  
 //If it's white's turn, check if there is a piece that can attack the white king  on
  //the black side.
  if(white) {
    for(std::map<std::pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
        it != occ.cend(); ++it) {
      if(!it->second->is_white()) {
        int col_dif = w_king_loc.first - it->first.first;
        int row_dif = w_king_loc.second - it->first.second;
        obstruction = _board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif,
 it->first);
        if(obstruction) {
          if(it->second->legal_capture_shape(it->first,w_king_loc)) {
            return true;
	  }
        }
      }
    }
  }

  //If it's black's turn, check if there is a piece that can attack the black king on the
  //white side.
  else {
    for(std::map<std::pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
        it != occ.cend(); ++it) {
      if(it->second->is_white()) {
	int col_dif = b_king_loc.first - it->first.first;
        int row_dif = b_king_loc.second - it->first.second;
        obstruction = _board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif, it->first);
        if(obstruction) {
          if(it->second->legal_capture_shape(it->first,b_king_loc)) {
            return true;
	  }
	}
      }
    }
 }
  return false;
}


//Checks if all possible moves are available and if there is a move
//we return false. If all moves are false and you are in check it returns true.
bool Chess::in_mate( bool white ) const {

  // copy of board
  Board board = _board;
  //Gets all of the pieces that are on the board.
  std::map<std::pair<char,char> , Piece*> occ = board.get_occ();
  std::pair<char,char> w_king , b_king;
  vector<std::pair<char,char>> coord;
  
  //Makes all possible coordinates that can be made on the board.
  //Goes from A1-H8. It puts it in a vector called coord.
  for(char i = 'A'; i <= 'H'; i++) {
    for(char j = '1'; j <= '8'; j++) {
      std::pair<char,char> loc = std::make_pair(i , j);
      coord.push_back(loc);
    }
  }
  
  for(std::map<std::pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
      it != occ.cend();
      ++it) {
    if(white) {
      
      //Looks for all possible white pieces and checks if they have any
      //moves that can move them out of check.
      if(isupper(it->second->to_ascii())) {
	for(int i = 0; i < coord.size(); i++) {
	  // check if legal move from piece to current loc in loop
	  if(it->second->legal_move_shape(it->first, coord[i]) || it->second->legal_capture_shape(it->first, coord[i])) {
	    // Exception for pawns
	    if(it->second->legal_capture_shape(it->first, coord[i]) && occ.find(coord[i]) == occ.end() && it->second->to_ascii() == 'P') {
	      continue;
	    }
	    if(it->second->legal_move_shape(it->first, coord[i]) && occ.find(coord[i]) != occ.end() && it->second->to_ascii() == 'P') {
              continue;
            }
	    int row_dif = coord[i].second - it->first.second;
	    int col_dif = coord[i].first - it->first.first;
	    // check board for obstruction
	    if(board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif, it->first)) {
	      if(occ.find(coord[i]) == occ.end() || islower(occ.find(coord[i])->second->to_ascii())) {
		// check for possibility of capture
		std::map<std::pair<char,char>, Piece*>::iterator cap = occ.find(coord[i]);
		// place piece at it into loc in loop
		board.delete_piece(coord[i]);
	        board.add_piece(coord[i], it->second->to_ascii());
	        board.delete_piece(it->first);
	        if(!in_check_copy(white, board)) {
		  // if new move causes not check, undo
		  board.delete_piece(coord[i]);
		  board.add_piece(it->first, it->second->to_ascii());

		  // there is a capture!
		  if(cap != occ.end()) {
		    board.add_piece(coord[i], cap->second->to_ascii());
		  }
		  // 
		  return false;
		}
		// undo move
		board.delete_piece(coord[i]);
                board.add_piece(it->first, it->second->to_ascii());
                if(cap != occ.end()) {
                  board.add_piece(coord[i], cap->second->to_ascii());
		}
	      }
	    }
	  }	     
	}
      }
    }
    if(!white) {

      //Looks at all black pieces to see if they have any possible moves that they can make.
      if(islower(it->second->to_ascii())) {
        for(int i = 0; i < coord.size(); i++) {
          if(it->second->legal_move_shape(it->first, coord[i]) || it->second->legal_capture_shape(it->first, coord[i])) {
	    if(it->second->legal_capture_shape(it->first, coord[i]) && occ.find(coord[i]) == occ.end() && it->second->to_ascii() == 'p') {
              continue;
            }
            if(it->second->legal_move_shape(it->first, coord[i]) && occ.find(coord[i]) != occ.end() && it->second->to_ascii() == 'p') {
              continue;
            }
            int row_dif = coord[i].second - it->first.second;
            int col_dif = coord[i].first - it->first.first;
            if(board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif, it->first)) {
	      if(occ.find(coord[i]) == occ.end() || isupper(occ.find(coord[i])->second->to_ascii())) {
	        std::map<std::pair<char,char>, Piece*>::iterator cap = occ.find(coord[i]);
	        board.delete_piece(coord[i]);
	        board.add_piece(coord[i], it->second->to_ascii());
                board.delete_piece(it->first);
		if(!in_check_copy(white, board)) {
                  board.delete_piece(coord[i]);
                  board.add_piece(it->first, it->second->to_ascii());
                  if(cap != occ.end()) {
                    board.add_piece(coord[i], cap->second->to_ascii());
                  }
		  return false;
		 }
	         board.delete_piece(coord[i]);
                 board.add_piece(it->first, it->second->to_ascii());
                 if(cap != occ.end()) {
                   board.add_piece(coord[i], cap->second->to_ascii());
		 }
	      }
	    }
	  }
	}
      }
    }
  }
  
  //If all moves are unavaible and we are in check then we return true.
  if(!in_check(white)) {
    return false;
  }
  return true;
}


//Chekcs to see if there are any possible moves that can be made without moving into check.
//If there is a possible move return false. If there isn't and the player is not in
//check then return true.
bool Chess::in_stalemate( bool white ) const {
  // copy of board
  // similar to check mate except at end
  Board board = _board;
  std::map<std::pair<char,char> , Piece*> occ = board.get_occ();
  vector<std::pair<char,char>> coord;
  for(char i = 'A'; i <= 'H'; i++) {
    for(char j = '1'; j <= '8'; j++) {
      std::pair<char,char> loc = std::make_pair(i , j);
      coord.push_back(loc);
    }
  }
  for(std::map<std::pair<char,char> , Piece*>::iterator it = occ.begin(); it != occ.end();
      ++it) {
    if(white) {
      if(isupper(it->second->to_ascii())) {
        for(int i = 0; i < coord.size(); i++) {
          if(it->second->legal_move_shape(it->first, coord[i]) || it->second->legal_capture_shape(it->first, coord[i])) {
	    if(it->second->legal_move_shape(it->first, coord[i]) && occ.find(coord[i]) != occ.end() && it->second->to_ascii() == 'P') {
	      continue;
	    }
	    if(it->second->legal_capture_shape(it->first, coord[i]) && occ.find(coord[i]) ==occ.end() && it->second->to_ascii() == 'P') {
              continue;
            }
            int row_dif = coord[i].second - it->first.second;
            int col_dif = coord[i].first - it->first.first;
            if(board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif, it->first)) {
              if(occ.find(coord[i]) == occ.end() || islower(occ.find(coord[i])->second->to_ascii())) {
                std::map<std::pair<char,char>, Piece*>::iterator cap = occ.find(coord[i]);
                board.delete_piece(coord[i]);
                board.add_piece(coord[i], it->second->to_ascii());
                board.delete_piece(it->first);
                if(!in_check_copy(white, board)) {
                  board.delete_piece(coord[i]);
                  board.add_piece(it->first, it->second->to_ascii());
                  if(cap != occ.end()) {
                    board.add_piece(coord[i], cap->second->to_ascii());
                  }
                  return false;
                }
                board.delete_piece(coord[i]);
                board.add_piece(it->first, it->second->to_ascii());
                if(cap != occ.end()) {
                  board.add_piece(coord[i], cap->second->to_ascii());
                }
              }
            }
          }
        }
      }
    }
   if(!white) {
      if(islower(it->second->to_ascii())) {
        for(int i = 0; i < coord.size(); i++) {
          if(it->second->legal_move_shape(it->first, coord[i]) || it->second->legal_capture_shape(it->first, coord[i])) {
	    if(it->second->legal_move_shape(it->first, coord[i]) && occ.find(coord[i]) != occ.end() && it->second->to_ascii() == 'p') {
              continue;
            }
            if(it->second->legal_capture_shape(it->first, coord[i]) && occ.find(coord[i]) == occ.end() && it->second->to_ascii() == 'p') {
              continue;
            }
            int row_dif = coord[i].second - it->first.second;
            int col_dif = coord[i].first - it->first.first;
            if(board.check_for_obstruction(it->second->to_ascii(), row_dif, col_dif, it->first)) {
              if(occ.find(coord[i]) == occ.end() || isupper(occ.find(coord[i])->second->to_ascii())) {
                std::map<std::pair<char,char>, Piece*>::iterator cap = occ.find(coord[i]);
                board.delete_piece(coord[i]);
                board.add_piece(coord[i], it->second->to_ascii());
                board.delete_piece(it->first);
                if(!in_check_copy(white, board)) {
                  board.delete_piece(coord[i]);
                  board.add_piece(it->first, it->second->to_ascii());
                  if(cap != occ.end()) {
                    board.add_piece(coord[i], cap->second->to_ascii());
                  }
                  return false;
                }
                board.delete_piece(coord[i]);
                board.add_piece(it->first, it->second->to_ascii());
                if(cap != occ.end()) {
                  board.add_piece(coord[i], cap->second->to_ascii());
                }
	      }
	    }
	  }
	}
      }
   }
  }

  // if there are no possible moves and player is not in check, game is in stalemate, return true
  if(!in_check_copy(white, board)) {
    return true;
  }
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
std::ostream& operator << ( std::ostream& os , const Chess& chess )
{
	// Write the board out and then either the character 'w' or the character 'b', depending on whose turn it is
	return os << chess.board() << ( chess.turn_white() ? 'w' : 'b' );
}

std::istream& operator >> ( std::istream& is , Chess& chess )
{
  // read board from file
  is >> chess._board;

  // determine turn
  char p;
  is.get(p);
  is.get(p);
  if (p == 'w') {
    chess._turn_white = true;
  } else {
    chess._turn_white = false;
  }
  return is;
}

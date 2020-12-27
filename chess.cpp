#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<vector>
using namespace std;
class piece;
class Pair     // Class used to represent position on the chess board
{
public:
	int x;		//x coordinate
	int y;		//y coordinate
	Pair(int a, int b) :x(a), y(b) {}
};
class path		//Class to the hold the path in which the piece is being moved
{
public: vector<Pair> route;
		int status;
		path() { status = 0; }
		void build(int a, int b) { Pair temp(a, b);route.push_back(temp); } //adding positions to the path
		void reset() { route.clear();status = 0; }  //deleting previously present positions
};
class undo_obj    //Format that is to be used when backing up a move for undo operation
{
public:
	int x, y;
	int fx, fy;
	piece *loc;
	undo_obj(piece *temp, int a, int b, int c, int d) :loc(temp), x(a), y(b), fx(c), fy(d) {}
};
class piece      //Base class containing data common to all objects/pieces
{
public:
	int x, y, points, color; //1=>white,2=>black  //0->white ,1->black 
	int dir, org_x, org_y, has_been_moved; //dir->direction
	void(*disp)(int, int, int);
	/*checkmove is different for every class as every piece moves differently(Chess rules)
	 Hence it is declared as virtual so that it should be redefined in every class.
	It returns the path that the piece will take in moving from (x,y) --> (fx,fy)
	*/
	virtual path checkmove(int, int, piece *[8][8]);
	void clear();			//used to clear the piece in the UI
	void(*clearbox)(int, int);
	void display();          //used to display the piece in the UI
	piece(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int));
	piece* move(int, int, piece *[8][8]);		//function to move the piece from (x,y)-->(fx,fy)
	virtual ~piece() {}
};

piece::piece(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int))
{
	x = org_x = ix;
	y = org_y = iy;
	dir = d;
	points = p;
	color = c;
	has_been_moved = 0;
	disp = displ;
	clearbox = clearb;
	if (disp != NULL)
		disp(x, y, color);

}

piece* piece::move(int fx, int fy, piece *board[8][8])
{
	piece *ret = board[fx][fy];
	if (board[fx][fy] != NULL)
		board[fx][fy]->clear();			//clear piece in display
	board[fx][fy] = board[x][y];
	if (!(x == fx && y == fy))
		board[x][y] = NULL;				//clear piece in chess 2D array
	(*clearbox)(x, y);
	board[fx][fy]->x = fx;
	board[fx][fy]->y = fy;
	board[fx][fy]->has_been_moved = 1;
	board[fx][fy]->display();			//diaply piece in final position
	return ret;
}
void piece::clear()
{
	if (clearbox != NULL)
		(*clearbox)(x, y);
}
void piece::display()
{
	if (disp != NULL)
		(*disp)(x, y, color);
}
path piece::checkmove(int fx, int fy, piece *board[8][8])
{
	path ret0, ret;		//ret0 corresponds to empty path,ret corresponds to non empty path
	ret.build(x, y);		//build path
	if (fx >= 8 || fx < 0 || fy >= 8 || fy < 0)
		return ret0;
	if (fx == x && fy == y)
		return ret0;
	if (board[fx][fy] != NULL)
		if (board[fx][fy]->color == color)
			return ret0;
	ret.status = 1;
	return ret;
}

class rook :virtual public piece
{
public: rook(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int)) :piece(ix, iy, p, c, d, displ, clearb) {}
		path checkmove(int, int, piece *[8][8]);
		~rook() {}
};
path rook::checkmove(int fx, int fy, piece *board[8][8])
{
	path ret0, ret = piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (fx != x && fy != y)
		return ret0;
	//Check all horizontal and vertical positions and build path if destination is found in correct direction
	for (int i = fy + 1;i < y;i++)
		if (board[x][i] == NULL) ret.build(x, i); else return ret0;
	for (int i = y + 1;i < fy;i++)
		if (board[x][i] == NULL) ret.build(x, i); else return ret0;
	for (int i = fx + 1;i < x;i++)
		if (board[i][y] == NULL) ret.build(i, y); else return ret0;
	for (int i = x + 1;i < fx;i++)
		if (board[i][y] == NULL) ret.build(i, y); else return ret0;
	return ret;
}
class bishop : virtual public piece
{
public: bishop(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int)) :piece(ix, iy, p, c, d, displ, clearb) {}
		path checkmove(int, int, piece *[8][8]);
		~bishop() {}
};
path bishop::checkmove(int fx, int fy, piece *board[8][8])
{
	path ret0, ret = piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (!(x + y == fx + fy || x - y == fx - fy))
		return ret0;
	//Check all primary and secondary diagonal positions and build path if destination is found in correct direction
	for (int i = x + 1, j = y + 1;i < fx && j < fy;i++, j++)
		if (board[i][j] == NULL) ret.build(i, j); else return ret0;
	for (int i = x + 1, j = y - 1;i<fx && j>fy;i++, j--)
		if (board[i][j] == NULL) ret.build(i, j); else return ret0;
	for (int i = x - 1, j = y + 1;i > fx && j < fy;i--, j++)
		if (board[i][j] == NULL) ret.build(i, j); else return ret0;
	for (int i = x - 1, j = y - 1;i > fx && j > fy;i--, j--)
		if (board[i][j] == NULL) ret.build(i, j); else return ret0;
	return ret;
}
class queen :public rook, public bishop
{
public:queen(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int)) :rook(ix, iy, p, c, d, displ, clearb), bishop(ix, iy, p, c, d, displ, clearb), piece(ix, iy, p, c, d, displ, clearb) {}
	   path checkmove(int, int, piece *[8][8]);
	   ~queen() {}
};
path queen::checkmove(int fx, int fy, piece *board[8][8])
{
	/*Queen can move like a rook and a bishop.So checking if move is legal wrt bishop or rook
	if legal, the path obtained is returned as the final path*/
	path ret1 = rook::checkmove(fx, fy, board);
	path ret2 = bishop::checkmove(fx, fy, board);
	if (ret1.status == 1)
		return ret1;
	else
		return ret2;
}
class knight : virtual public piece
{
public: knight(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int)) :piece(ix, iy, p, c, d, displ, clearb) {}
		path checkmove(int, int, piece *[8][8]);
};
path knight::checkmove(int fx, int fy, piece *board[8][8])
{
	//Knight can move in total 8 possible postions. All are checked. Path consists of only one position
	int pos[8][2] = { { x - 1,y + 2 },{ x - 2,y + 1 },{ x + 1,y + 2 },{ x + 2,y + 1 },{ x - 1,y - 2 },{ x - 2,y - 1 },{ x + 1,y - 2 },{ x + 2,y - 1 } };
	path ret0, ret = piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	for (int i = 0;i < 8;i++)
		if (fx == pos[i][0] && fy == pos[i][1])
		{
			return ret;
		}
	return ret0;
}
class pawn : public piece
{
public: pawn(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int)) :piece(ix, iy, p, c, d, displ, clearb) {}
		path checkmove(int, int, piece *[8][8]);
		void check_promo(int, int, piece *[8][8]);
};
path pawn::checkmove(int fx, int fy, piece *board[8][8])
{
	/*Pawn can perform capture diagonally and move forward wrt it's side.Double move is allowed if the pawn
	is moving for the first time */
	path ret0, ret = piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (x == org_x && y == org_y)          //two step move
		if (abs(fy - y) == 2 && fx == x && board[fx][fy] == NULL && board[fx][y + dir] == NULL)
		{
			ret.build(fx, y + dir);

			return ret;
		}
	if (board[fx][fy] != NULL)			//capture diagnol 
		if (board[fx][fy]->color != color)
			if ((fx == x + 1 || fx == x - 1) && fy == y + dir)
			{
				return ret;
			}
	if (board[fx][fy] == NULL && fx == x && (fy == y + dir))	//one step move
	{
		return ret;
	}
	return ret0;
}
class king : public piece
{
public: king(int ix, int iy, int p, int c, int d, void(*displ)(int, int, int), void(*clearb)(int, int)) :piece(ix, iy, p, c, d, displ, clearb) {}
		path checkmove(int, int, piece *[8][8]);
};
path king::checkmove(int fx, int fy, piece *board[8][8])
{
	//King can move within a radius of one chess box
	path ret0, ret = piece::checkmove(fx, fy, board);
	if (ret.status == 0)
		return ret0;
	if (abs(fx - x) <= 1 && abs(fy - y) <= 1)
	{
		return ret;
	}
	return ret0;
}
class chessboard
{
public:
	int prev_x, prev_y, select_p, turn;
	vector<undo_obj> prev_list;
	piece *player[2][16];
	piece *board[8][8];
	chessboard(void(*skeleton_b)(int, int), void(*clearb)(int, int), void(*highlightb)(int, int), void(*disp)(), void(*msg)(char *));
	void select(int, int);
	void(*skeleton_box)(int, int);
	void(*clearbox)(int, int);
	void(*highlight)(int, int);
	void setking(void(*displ)(int, int, int));
	void setqueen(void(*displ)(int, int, int));
	void setbishop(void(*displ)(int, int, int));
	void setrook(void(*displ)(int, int, int));
	void setknight(void(*displ)(int, int, int));
	void setpawn(void(*displ)(int, int, int));
	void(*dqueen)(int, int, int);
	void(*dking)(int, int, int);
	void(*drook)(int, int, int);
	void(*dbishop)(int, int, int);
	void(*dknight)(int, int, int);
	void(*dpawn)(int, int, int);
	void(*message)(char *);
	void redisplay();
	void remove_piece(piece *val, int, int);
	path check(int);
	void show_list();
	void undo();
	void undo_move();
	int move(int, int);
	int checkmate(int, path);
	void(*display)();
	void check_promo(int, int);
	void remove(piece*);
	void add_piece(piece *);
};
void chessboard::undo_move()   //move is reversed completely. Invoked for undoing a move on user request.
{
	if (prev_list.size() == 0)
		return;
	undo();			//undoing move in the chess engine.
	skeleton_box(prev_x, prev_y);
	display();
	prev_x = prev_y = 9;
	select_p = 0;
	turn = !turn;	//changing turn;
}
void chessboard::undo()    //move is reversed without changing the turn.Used by the chess engine.
{
	if (prev_list.size() == 0)
		return;
	undo_obj temp = prev_list.back();
	if (temp.x == temp.fx && temp.y == temp.fy)
	{
		remove(board[temp.fx][temp.fy]);
		delete board[temp.fx][temp.fy];
		board[temp.fx][temp.fy] = temp.loc;
		add_piece(temp.loc);
		prev_list.pop_back();
		temp = prev_list.back();
	}
	board[temp.fx][temp.fy]->move(temp.x, temp.y, board);
	board[temp.fx][temp.fy] = temp.loc;
	if (temp.loc != NULL)
	{
		for (int i = 0;i < 16;i++)
			if (player[(temp.loc)->color][i] == NULL)
			{
				player[(temp.loc)->color][i] = temp.loc;break;
			}
		board[temp.fx][temp.fy]->display();
	}
	prev_list.pop_back();

}
void chessboard::add_piece(piece * p)	//add piece to player's collection of active pieces on the chess board
{
	for (int i = 0;i < 16;i++)
		if (player[p->color][i] == NULL)
			player[p->color][i] = p;
}
void chessboard::check_promo(int fx, int fy) //check for pawn promotion
{
	if (board[fx][fy]->points == 1 && ((turn == 0 && fy == 7) || (turn == 1 && fy == 0)))
	{
		cout << "CHECK PROMO" << endl;
		int temp1 = prev_x, temp2 = prev_y;
		prev_x = fx;prev_y = fy;
		remove_piece(board[fx][fy], fx, fy);
		prev_x = temp1;prev_y = temp2;
		board[fx][fy] = new queen(fx, fy, 5, turn, turn == 0 ? 1 : -1, dqueen, clearbox);
		board[fx][fy]->clear();
		board[fx][fy]->display();
		add_piece(board[fx][fy]);
	}

}
void chessboard::show_list()
{
	undo_obj temp = prev_list.back();
}
void chessboard::remove(piece *val)	//remove piece from player's collection of active pieces on the chess board
{
	if (val != NULL)
		for (int i = 0;i < 2;i++)
			for (int j = 0;j < 16;j++)
				if (player[i][j] == val)
				{
					player[i][j] = NULL;break;
				}
}
void chessboard::remove_piece(piece *val, int fx, int fy) //remove piece completely
{
	undo_obj temp(val, prev_x, prev_y, fx, fy);
	remove(val);

	prev_list.push_back(temp);
}
void chessboard::redisplay()	//display all pieces in the board in their respective positions
{
	for (int i = 0;i < 8;i++)
		for (int j = 0;j < 8;j++)
			if (board[i][j] != 0)
				board[i][j]->display();
}
chessboard::chessboard(void(*skeleton_b)(int, int), void(*clearb)(int, int), void(*highlightb)(int, int), void(*disp)(), void(*msg)(char *))
{
	for (int i = 0;i < 2;i++)
		for (int j = 0;j < 16;j++)
			player[i][j] = NULL;
	skeleton_box = skeleton_b;
	clearbox = clearb;
	message = msg;
	display = disp;
	select_p = 0;
	turn = 0;
	highlight = highlightb;
	for (int i = 0;i < 8;i++)
		for (int j = 0;j < 8;j++)
			board[i][j] = NULL;
	prev_x = prev_y = 9;
}
path chessboard::check(int turnt)  //Used to check if the King has been given a check
{

	//Note : only 2 players are there. If turn =0, then !turn=1
/*	This is evaluated by checking if any piece of "(!turn)" player can move to the position in which
	the King of the "turn" player is present. If such a move is possible. A check for "turn" player is
	declared. Path of the check is returned
*/
	path ret0, ret;
	if (player[turnt][0] != NULL)
	{
		for (int i = 0;i < 16;i++)
		{
			if (player[!turnt][i] != NULL)
			{
				ret = player[!turnt][i]->checkmove(player[turnt][0]->x, player[turnt][0]->y, board);
				if (ret.status == 1)
				{
					message("CHECK!!");return ret;
				}
			}
		}
	}
	return ret0;
}
int chessboard::move(int x, int y)	//used to perform the move in the chess engine,backup move,display message
{
	path ret;
	if (prev_x == x && prev_y == y)
		return 0;
	ret = board[prev_x][prev_y]->checkmove(x, y, board);
	if (ret.status)
	{
		piece *tt = board[prev_x][prev_y]->move(x, y, board);
		remove_piece(tt, x, y);
		//move should be declared invalid if move results in check to own King
		if (check(turn).status) { undo();message("SELF CHECK!!ILLEGAL MOVE"); return 0; }
		check_promo(x, y);
		ret = check(!turn); //get the path in which check is occuring. Empty if no check has occured.
		if (ret.status)
		{
			message("CHECK!! ");
			if (checkmate(!turn, ret)) //check if the opponent has been checkmated
			{
				message("CHECKMATE!! ");
				return 1;
			}
			else
				message("CHECK!! ");
		}
	}
	else
		return 0;
	return 1;
}
int chessboard::checkmate(int turnt, path ret) //used to check if a checkmate has occured for "turnt" player
{
	/*
		Checkmate is declared if 3 conditions are fullfilled.
		1) The King under check cannot move to any other position(legally) and still not be under check.
		2) No piece belonging to the same player as the King can block the check by going on the path between the
			piece causing the check and the King.
		3) No piece belonging to the same player as the King can capture the piece belonging to the other player
			causing check to the King.

			The second and third points are combined into a single condition by including the position of the
			piece causing the check into the path of the check. If  the king can move or if any piece can be
			moved to this path, no checkmate has occured.
	*/
	int temp_turn = turn;	//backing up current  turn
	int x = player[turnt][0]->x;
	int y = player[turnt][0]->y;
	prev_x = x;
	prev_y = y;
	turn = turnt;
	//checking the first condition in the below code
	int king_move[8][2] = { { x + 1,y },{ x + 1,y + 1 },{ x,y + 1 },{ x - 1,y + 1 },{ x - 1,y },{ x - 1,y - 1 },{ x,y - 1 },{ x + 1,y - 1 } };
	for (int i = 0;i < 8;i++)
		if (move(king_move[i][0], king_move[i][1]) == 1)
		{
			undo();
			turn = temp_turn;
			return 0;
		}
	//Checking second and third conditions in the below code
	for (int i = 1;i < 16;i++)
	{
		if (player[turnt][i] == NULL) continue;
		for (int j = 0;j < ret.route.size();j++)
		{
			path val;
			Pair pos = ret.route.at(j);
			prev_x = player[turnt][i]->x;
			prev_y = player[turnt][i]->y;
			if (move(pos.x, pos.y) == 1)
			{
				undo();turn = temp_turn;return 0;
			}

			turn = turnt;
		}
	}
	turn = temp_turn;
	return 1;
}
void chessboard::select(int x, int y)
//input from the UI is fed here. invokes move function and flushes the changes made in the chess engine to the UI
{
	if (select_p == 0)
	{
		if (prev_x == x && prev_y == y || board[x][y] == NULL)
			return;
		if (board[x][y]->color != turn)
			return;
		if (highlight != NULL)
		{
			highlight(x, y);display();
		}
		prev_x = x;
		prev_y = y;
		select_p = 1;
		message("");
	}
	else if (select_p == 1)
	{
		if (prev_x == 9 || prev_y == 9)
			return;
		if (move(x, y) == 0)
			message("INVALID MOVE!! ");
		else
		{
			turn = !turn;
			//message("");
			display();
		}
		skeleton_box(prev_x, prev_y);
		display();
		prev_x = prev_y = 9;
		select_p = 0;
	}
}

void chessboard::setking(void(*displ)(int, int, int))
// initialise king by using the funtion pointer to display King
{
	dking = displ;
	board[4][0] = player[0][0] = new king(4, 0, 99, 0, 1, displ, clearbox);
	board[4][7] = player[1][0] = new king(4, 7, 99, 1, -1, displ, clearbox);
}
void chessboard::setqueen(void(*displ)(int, int, int))
// initialise queen by using the funtion pointer to display Queen
{
	dqueen = displ;
	board[3][0] = player[0][1] = new queen(3, 0, 5, 0, 1, displ, clearbox);
	board[3][7] = player[1][1] = new queen(3, 7, 5, 1, -1, displ, clearbox);
}
void chessboard::setbishop(void(*displ)(int, int, int))
// initialise bishop by using the funtion pointer to display Bishop
{
	dbishop = displ;
	board[2][0] = player[0][2] = new bishop(2, 0, 4, 0, 1, displ, clearbox);
	board[5][0] = player[0][3] = new bishop(5, 0, 4, 0, 1, displ, clearbox);
	board[2][7] = player[1][2] = new bishop(2, 7, 4, 1, -1, displ, clearbox);
	board[5][7] = player[1][3] = new bishop(5, 7, 4, 1, -1, displ, clearbox);
}
void chessboard::setrook(void(*displ)(int, int, int))
// initialise rook by using the funtion pointer to display Rook
{
	drook = displ;
	board[0][0] = player[0][4] = new rook(0, 0, 2, 0, 1, displ, clearbox);
	board[7][0] = player[0][5] = new rook(7, 0, 2, 0, 1, displ, clearbox);
	board[0][7] = player[1][4] = new rook(0, 7, 2, 1, -1, displ, clearbox);
	board[7][7] = player[1][5] = new rook(7, 7, 2, 1, -1, displ, clearbox);
}
void chessboard::setknight(void(*displ)(int, int, int))
{
	//Initialise knight by using function pointer to display Knight
	dknight = displ;
	board[1][0] = player[0][6] = new knight(1, 0, 3, 0, 1, displ, clearbox);
	board[6][0] = player[0][7] = new knight(6, 0, 3, 0, 1, displ, clearbox);
	board[1][7] = player[1][6] = new knight(1, 7, 3, 1, -1, displ, clearbox);
	board[6][7] = player[1][7] = new knight(6, 7, 3, 1, -1, displ, clearbox);
}
void chessboard::setpawn(void(*displ)(int, int, int))
{
	//Initialise pawn by using funtion pointer to display pawn
	dpawn = displ;
	for (int i = 0;i < 8;i++)
		board[i][1] = player[0][i + 8] = new pawn(i, 1, 1, 0, 1, displ, clearbox);
	for (int i = 0;i < 8;i++)
		board[i][6] = player[1][i + 8] = new pawn(i, 6, 1, 1, -1, displ, clearbox);
}

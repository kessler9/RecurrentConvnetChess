class Figure{
	public:
	    int no;         // 0-King, 1-Pawn, 2-Rook, 3-Bishop, 4-Knight, 5-Queen
	    int index;      // in Set[] table
	    bool color;     // 0-white, 1-black
	    bool alive;
	    Pos curPos;
	    int val;
	    virtual vector <Pos> possible_moves(Figure * Board[]);
	    int ifPossMove( Figure *Board[], bool color, Pos test_Pos, bool _kill = true, bool _nokill = true );
	    int ifKingPossMove( Figure *Board[], bool color, Pos test_Pos, Pos oldPos);
	    virtual void setPos(int x, int y);
	    bool checkBoundaries(const Pos& newPos);
	    int move(Pos newPos);
	    bool changeType(int newType);
	    int getVal();
		Figure();
		~Figure();
};
	
class Pawn : public Figure{
	public:
	    vector <Pos> possible_moves(Figure * Board[]);
		Pawn();
		~Pawn();
};

class Queen : public Figure{
	public:
	    vector <Pos> possible_moves(Figure * Board[]);
		Queen();
		~Queen();
};

class Rook : public Figure{
	public:
	    vector <Pos> possible_moves(Figure * Board[]);
		Rook();
		~Rook();
};

class Bishop : public Figure{
	public:
	    vector <Pos> possible_moves(Figure * Board[]);
		Bishop();
		~Bishop();
};

class Knight : public Figure {
	public:
	    vector <Pos> possible_moves(Figure * Board[]);
		Knight();
		~Knight();
};

class King : public Figure{
	public:
	    vector <Pos> possible_moves(Figure * Board[]);
	    bool not_in_danger( Figure * Board[], bool color);
		King();
		~King();
};

class Blank : public Figure{
	public:
		Blank();
		~Blank();
};

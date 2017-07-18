class Chess{
	private:
	    Figure * Set[32];
	    int Status; // 0-game in progres, 1-king in danger, 2-end of game "PAT", 3-end of game "CHECKMATE"
        void fileDumpHistory(bool victor);
        void saveHistory();
	public:
	    int moveInd;
		void fillSetFromBoard();
	    bool playing;
	    queue <queue <Figure> > History;
	    bool curr_color;
	    Figure * Board[99]; // ind 0-63 <- board (stores * Figure if Figure else Null), ind 64-95 <- Set[32] (static indexed * Figure)
	    int getStatus();
	    void setStatus(int val);
	    bool new_game();
	    int move(Pos oldPos, Pos newPos); // 0-ERROR; 1-MOVED; 2-MOVED & NEED CHANGE PAWN TYPE
	    bool changeType(Pos curPos, int newType);
	    vector <Pos> figures_to_move();
	    vector <Pos> poss_moves(Pos figPos);
	Chess();
	~Chess();
};


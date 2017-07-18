class Pos {
    private:
        int x_val;
        int y_val;
    public:
        int x() const;
        int y() const;
        int index() const;
        void setX(int _x);
        void setY(int _y);
        void set(int _x, int _y);
        void set(const Pos& position);
        void operator= (const Pos& Q);
    Pos();
    Pos(int _x, int _y);
    Pos(const Pos& position);
    ~Pos();
};

typedef struct Move {
	Pos oldPos;
	Pos newPos;
	Move(Pos & oldPos, Pos & newPos){
		this->oldPos = oldPos;
		this->newPos = newPos;
	}
	Move(){};
	~Move(){};
	void printMove(){
#ifdef DEBUG
		std::cout << "(" << oldPos.x() << "," << oldPos.y() << ") -> (" << newPos.x() << "," << newPos.y() << ")" << std::endl;
#endif
	}
} Move;

typedef struct moveWithScore {
	Move move;
	double score;
	moveWithScore() {};
	moveWithScore(Move & move, int & score) : move(move), score(score) {};
} moveWithScore;

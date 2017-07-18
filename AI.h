class AI {
    private:
		int movesMade;
		bool color;
		bool nextMoveSet;
		double separationLine[2];
		double SVMError;
		Move chosenMove;
		vector <Move> allPossibleMoves;
		Chess * chessptr;
		//METHODS
		void setAllPossibleMoves();
		void setChosenMove();
		//UTILITY FUNCTIONS
		double setChosenMoveLayer(int layer, Figure * Board[], Move &move); //HELPS SET CHOSEN MOVE BREADTH SEARCH
		void setClassifierSeparationLine(Figure * Board[]); //LEAST SQUARES 
		double totalSquareDistanceCluster(Figure * Board[]);
		double moveScore(Figure * Board[], Move &move, bool notFirstLayer);
		bool aboveBoundaryLine(const int &x, const int &y);
	public:
		bool makeNextMove();
		AI(bool color, Chess * chessptr);
		~AI();
};

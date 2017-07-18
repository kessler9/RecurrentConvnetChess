#include "AI.h"

AI::AI(bool color, Chess * chessptr) : color(color), chessptr(chessptr), nextMoveSet(false), movesMade(0) {}

AI::~AI(){}

void AI::setAllPossibleMoves(){
	vector <Move> allPossibleMoves;
	vector <Pos> figuresToMove = chessptr->figures_to_move();
	for(unsigned int i=0; i<figuresToMove.size(); i++){
		vector <Pos> figAllPossibleMoves = chessptr->poss_moves(figuresToMove.at(i));
#ifdef DEBUG
		std::cout << "i: " << i << " SIZE OF figAllPossibleMoves: " << figAllPossibleMoves.size() << std::endl;
#endif
		for(unsigned int j=0; j<figAllPossibleMoves.size(); j++){
#ifdef DEBUG
			std::cout << "j: " << j << std::endl;
#endif
			Move move(figuresToMove.at(i), figAllPossibleMoves.at(j));
#ifdef DEBUG
			std::cout << "MOVE CREATED" << std::endl;
#endif
			allPossibleMoves.push_back(move);
		}	
	}
	this->allPossibleMoves = allPossibleMoves;
}

double AI::totalSquareDistanceCluster(Figure * Board[]){ //call with chessptr->Board[]
	double totalSquareDistance = 0, centroid[2];
	int points;
	int totalx = 0, totaly = 0;
	int TwoDBoard[8][8];
	const int color = this->color + 1; //adjust boolean to my scheme
	for(int x=0; x<8; x++){
		for(int y=0; y<8; y++){
			if(Board[x*8+y])
				TwoDBoard[x][y] = Board[x*8+y]->color ? 2 : 1;
			else
				TwoDBoard[x][y] = 0;
		}
	}
//FIND CENTROID
	for(int x=0; x<8; x++){
		for(int y=0; y<8; y++){
			if(TwoDBoard[x][y] == color){
				points += 1;
				totalx += x;
				totaly += y;
			}
		}
	}
	centroid[0] = totalx / (double)points;
	centroid[1] = totaly / (double)points;
	for(int x=0; x<8; x++){
		for(int y=0; y<8; y++){
			if(TwoDBoard[x][y] == color){
				totalSquareDistance += pow(pow(x-centroid[0], 2) + pow(y-centroid[1], 2), 0.5);
			}
		}
	}
	return totalSquareDistance;
}

bool AI::aboveBoundaryLine(const int &x, const int &y){
	return y > separationLine[0] * x + separationLine[1] ? true : false;
}
				
double AI::moveScore(Figure * Board[], Move &move, bool notFirstLayer=false){
	double score = 0;
	//CROSS THE BONDARY LINE?
	if(!notFirstLayer){
		if(aboveBoundaryLine(move.oldPos.x(), move.oldPos.y()) != aboveBoundaryLine(move.newPos.x(), move.newPos.y()))
			score -= CROSS_BOUNDARY_LINE_PENALTY;
	}
	if(Board[move.newPos.x()*8+move.newPos.y()] != NULL)
		score += Board[move.newPos.x()*8+move.newPos.y()]->val;
	return score;		
}

void AI::setChosenMove(){
	setClassifierSeparationLine(chessptr->Board);
	int highScore = 0;
	vector <Move> chosenMoves;
#ifdef DEBUG
	std::cout << "IN setChosenMove()" << std::endl;
#endif
	if(!allPossibleMoves.empty()){
		for(unsigned int i=0; i<allPossibleMoves.size(); i++){
			double score = moveScore(chessptr->Board, allPossibleMoves.at(i));
			if(score > highScore){
				highScore = score;
				chosenMoves.clear();
				chosenMoves.push_back(allPossibleMoves.at(i));
			}
			else if (score == highScore)
				chosenMoves.push_back(allPossibleMoves.at(i));
		}			
	}
#ifdef DEBUG
	std::cout << "GATHERED TIED CHOSEN MOVES" << std::endl;
#endif
	if(chosenMoves.size() == 1){
		chosenMove = chosenMoves.at(0);
		nextMoveSet = true;
	} else {
		int _highScore = 0;
		Move _chosenMove;
#ifdef DEBUG
		for(unsigned int i=0; i<chosenMoves.size(); i++){
			chosenMoves.at(i).printMove();
		}
#endif
		for(unsigned int i=0; i<chosenMoves.size(); i++){
			int score = setChosenMoveLayer(2, chessptr->Board, chosenMoves.at(i));
#ifdef DEBUG
			std::cout << "MOVE SCORED" << std::endl;
#endif
			if(score > _highScore || _highScore == 0){
				_highScore = score;
				_chosenMove = chosenMoves.at(i);
			}
		}
#ifdef DEBUG
		std::cout << "_chosenMove SET" << std::endl;
#endif
		nextMoveSet = true;
		chosenMove = _chosenMove;
	}
}

double AI::setChosenMoveLayer(int layer, Figure * Board[], Move &move){
#ifdef DEBUG
	std::cout << "IN LAYER: " << layer << std::endl;
#endif
	vector <moveWithScore> movesWithScore;
	Chess * hypotheticalChess = new Chess();
	for(int i=0; i<99; i++){
		if(Board[i] != NULL){
			Figure * figure = new Figure();
			memcpy(figure, Board[i], sizeof(*Board[i]));
			hypotheticalChess->Board[i] = figure;
		}
		else
			hypotheticalChess->Board[i] = NULL;
	}
#ifdef DEBUG
	for(int i=0; i<99; i++){
		if(hypotheticalChess->Board[i] != NULL){
			std::cout << i << " | " << hypotheticalChess->Board[i]->no << std::endl;
		}
	}
#endif
	hypotheticalChess->fillSetFromBoard();
	setClassifierSeparationLine(hypotheticalChess->Board);
#ifdef DEBUG
	std::cout << "PASSING hypotheticalChess BOARD AND SET LOADED" << std::endl;
#endif
	hypotheticalChess->curr_color = (layer % 2) ? chessptr->curr_color : !chessptr->curr_color;
	hypotheticalChess->move(move.oldPos, move.newPos);
	vector <Pos> hypotheticalFiguresToMove = hypotheticalChess->figures_to_move();
#ifdef DEBUG
	std::cout << "SIZE OF hypotheticalFiguresToMove: " << hypotheticalFiguresToMove.size() << std::endl;
	std::cout << "PASSING hypotheticalFiguresToMove ALLOCATION" << std::endl;
#endif
	vector <Pos> allPossibleHypotheticalMoves;
	for(unsigned int i=0; i<hypotheticalFiguresToMove.size(); i++){
		vector <Pos> hypotheticalFigureMoves;
#ifdef DEBUG
		std::cout << "i= " << i << std::endl;
		std::cout << "Position X: " << hypotheticalFiguresToMove.at(i).x() << " Y: " << hypotheticalFiguresToMove.at(i).y() << std::endl;
#endif
		int index = hypotheticalFiguresToMove.at(i).index();
#ifdef DEBUG
		std::cout << "LOOKUP INDEX: " << index << std::endl;
#endif
#ifdef SUPER_DEBUG
		for(int j=0; j<99; j++){
			if(!j%8&1) std::cout << std::endl;
			if(hypotheticalChess->Board[i] != NULL)
				std::cout << j << " IS " << hypotheticalChess->Board[i]->no << " ";
			else
				std::cout << j << " IS NULL ";
		}
#endif
		hypotheticalFigureMoves = hypotheticalChess->Board[index]->possible_moves(hypotheticalChess->Board);
#ifdef DEBUG
		std::cout << "GATHERED hypotheticalFigureMoves" << std::endl;
#endif
		for(unsigned int j=0; j<hypotheticalFigureMoves.size(); j++)
			allPossibleHypotheticalMoves.push_back(hypotheticalFigureMoves.at(j));
	}
#ifdef DEBUG
	std::cout << "GATHERED allPossibleHypotheticalMoves" << std::endl;
#endif
	for(unsigned int i=0; i<allPossibleHypotheticalMoves.size(); i++){
		Move _move(move.newPos, allPossibleHypotheticalMoves.at(i));
		int score = moveScore(hypotheticalChess->Board, _move, true);
		score = !(layer & 2) ? -1 * score : score;
		moveWithScore movewscore(_move, score);
		movesWithScore.push_back(movewscore);
	}
	if(layer == MAX_SEARCH_LAYERS){
		double averageScore = 0;
		int scoresCount = 0;
		for(unsigned int i=0; i<movesWithScore.size(); i++){
			averageScore += movesWithScore.at(i).score;
			scoresCount += 1;
		}
#ifdef DEBUG
		std::cout << "FINISHING FINAL LAYER: " << layer << std::endl;
#endif
		return averageScore / scoresCount;
	} else {
		double averageScore = 0;
		int scoresCount = 0;		
		for(unsigned int i=0; i<movesWithScore.size(); i++){
#ifdef DEBUG
			std::cout << "IN LAYER: " << layer << " AT INDEX: " << i << " WITH movesWithScore.size() = " << movesWithScore.size() << std::endl;
#endif
			averageScore += setChosenMoveLayer(layer+1, hypotheticalChess->Board, movesWithScore.at(i).move); //Spawn next layers.  Dangerous recursive function.
			scoresCount += 1;
		}
#ifdef DEBUG
		std::cout << "RETURNING FINAL RESULT" << std::endl;
#endif
		return averageScore / scoresCount;
	}
}

void AI::setClassifierSeparationLine(Figure * Board[]){
	int TwoDBoard[8][8];
	for(int x=0; x<8; x++){
		for(int y=0; y<8; y++){
			if(Board[x*8+y])
				TwoDBoard[x][y] = Board[x*8+y]->color ? 2 : 1;
			else
				TwoDBoard[x][y] = 0;
		}
	}
	double oldSeparationLine[2] = {0,5};
    double separationLine[2] = {0,5};
	int iterations = 0;
    while(true){
		double _totalSquareDistance = 0; //minus epsilon
		double totalSquareDistance_ = 0; //plus epsilon
		double __totalSquareDistance = 0; //minus epsilon B shift
		double totalSquareDistance__ = 0; //plus epsilon B shift
		for(int x=0; x<8; x++){
			for(int y=0; y<8; y++){
				if(TwoDBoard[x][y]&1){
				//FIX THIS... DISTANCE FROM (x_0, y_0) TO y = mx + b:
				//CONVERT SLOPE INTERCEPT FORM TO GENERAL FORM ax + by + c = 0
				//DISTANCE = |ax_0 + by_0 + c|/sqrt(a^2 + b^2)
					_totalSquareDistance += pow(ABS(y - (separationLine[0] - EPSILON) * x - separationLine[1]) / pow(1 + pow(separationLine[0], 2), 0.5), 2);
					totalSquareDistance_ += pow(ABS(y - (separationLine[0] + EPSILON) * x - separationLine[1]) / pow(1 + pow(separationLine[0], 2), 0.5), 2);
					__totalSquareDistance += pow(ABS(y - separationLine[0] * x - (separationLine[1] - EPSILON)) / pow(1 + pow(separationLine[0], 2), 0.5), 2);
					totalSquareDistance__ += pow(ABS(y - separationLine[0] * x - (separationLine[1] + EPSILON)) / pow(1 + pow(separationLine[0], 2), 0.5), 2);
				}
			}
		}
		oldSeparationLine[0] = separationLine[0];
		oldSeparationLine[1] = separationLine[1];
		separationLine[0] += totalSquareDistance_ - _totalSquareDistance <= 0 ? LEARNING_RATE : (-LEARNING_RATE);
		separationLine[1] += totalSquareDistance__ - __totalSquareDistance <= 0 ? LEARNING_RATE : (-LEARNING_RATE);
		if(iterations == MAX_CLASSIFIER_ITERATIONS){
			memcpy(&this->separationLine, &separationLine, 2*sizeof(double));
#ifdef DEBUG
			std::cout << "SEPARATION LINE: y = " << this->separationLine[0] << "x + " << this->separationLine[1] << std::endl;
#endif
			break;
		}
		iterations += 1;
	}
}


bool AI::makeNextMove(){
#ifdef DEBUG
	std::cout << "CHECKPOINT PASSED inside makeNextMove" << std::endl;
#endif
	setAllPossibleMoves();
#ifdef DEBUG
	std::cout << "CHECKPOINT PASSED setAllPossibleMoves" << std::endl;
#endif
	setChosenMove();
#ifdef DEBUG
	std::cout << "CHECKPOINT PASSED setChosenMove" << std::endl;
#endif
	if(nextMoveSet){
		int status = chessptr->move(chosenMove.oldPos, chosenMove.newPos);
		if(status&1){
			if(status==2)
				chessptr->changeType(chosenMove.newPos, QUEEN);
			return true;
		}
		movesMade += 1;
	}
	return false;
}
	

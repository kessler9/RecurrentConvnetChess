#include "chess.h"

Chess::Chess(){
    playing = false;
    for(int i = 0; i<32; i++)
        Set[i]=NULL;
    this->new_game();
}
Chess::~Chess(){
	for(int i=0; i<99; i++){
		if(i<32)
			delete Set[i];
		delete Board[i];
	}
}

bool Chess::new_game(){
    int Kind[]= {KING,QUEEN,BISHOP,BISHOP,KNIGHT,KNIGHT,ROOK,ROOK,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN},
        fval[]={20,9,3,3,3,3,5,5,1,1,1,1,1,1,1,1},
        pos_x[]={4,3,2,5,1,6,0,7},
        pos_y[]={0,1,7,6};

    for(int i = 0; i<99; i++){
        Board[i] = NULL;
    }
    for(int i = 0; i<32; i++){
        if(Set[i]!=NULL)
            delete[] Set[i];
        if(Kind[i%16]==KING){
            King *king = new King();
            Set[i] = king;
        }
        else if(Kind[i%16]==QUEEN){
            Queen *queen = new Queen();
            Set[i] = queen;
        }
        else if(Kind[i%16]==KNIGHT){
            Knight *knight = new Knight();
            Set[i] = knight;
        }
        else if(Kind[i%16]==BISHOP){
            Bishop *bishop = new Bishop();
            Set[i] = bishop;
        }
        else if(Kind[i%16]==ROOK){
            Rook *rook = new Rook();
            Set[i] = rook;
        }
        else{
            Pawn *pawn = new Pawn();
            Set[i] = pawn;
        }
        Set[i]->index = i;
        Set[i]->color = i/16;
        Set[i]->no = Kind[i%16];
        Set[i]->setPos(pos_x[i%8], pos_y[i/8]);
        Set[i]->val = fval[i%16];
        Set[i]->alive = true;
        int currPos = Set[i]->curPos.x()*8+Set[i]->curPos.y();
        Board[currPos] = Set[i];
    }
	for(int i=0; i<32; i++)
		Board[i+64] = Set[i];
    while(!History.empty())
        History.pop();
    curr_color = false;
    playing = true;
    moveInd = 0;
    setStatus(0);
    return true;
}

void Chess::fillSetFromBoard(){
	for(int i=0; i<32; i++)
		Set[i] = Board[i+64];
}

void Chess::setStatus(int val){
    Status = val;
    return;
}

int Chess::getStatus(){
    return Status;
}

vector <Pos> Chess::figures_to_move(){
    vector <Pos> ret, check;
    for(int i=0; i<16; i++){
        if(!Set[i+curr_color*16]->alive)
            continue;	
        check = Set[i+curr_color*16]->	possible_moves(Board);
        if(!check.empty())
            ret.push_back(Set[i+curr_color*16]->curPos);
    }
    return ret;
}

vector <Pos> Chess::poss_moves(Pos figPos){
    if(Board[figPos.x()*8+figPos.y()])
        if(Board[figPos.x()*8+figPos.y()]->color == curr_color) {
            return Board[figPos.x()*8+figPos.y()]->possible_moves(Board);
        }
    vector<Pos> nic;
    return nic;
}

void Chess::saveHistory(){
    queue <Figure> figureQueue;
	for(int i=0; i<99; i++){
		if(Board[i] != NULL){
			figureQueue.push(*Board[i]);
		} else {
			Blank * blank = new Blank();
			figureQueue.push(*blank);
			delete blank;
		}	
	}
    History.push(figureQueue);
}

void Chess::fileDumpHistory(bool victor){
#ifdef DEBUG
	std::cout << "IN Chess::fileDumpHistory()" << std::endl;
#endif
    time_t timestamp; time(&timestamp);
    string prefixstring ("History_");
    std::stringstream ss; ss << timestamp; string timestring = ss.str();
    string _filename = prefixstring + timestring;
    char * filename = new char[_filename.length()+1];
    strcpy(filename, _filename.c_str());
    std::ofstream outstream(filename);
	outstream << "{\"Victor\":" << victor << "}" << std::endl;
#ifdef DEBUG
	std::cout << "HISTORY LENGTH: " << History.size() << std::endl;
#endif
	while(!History.empty()){
#ifdef DEBUG
		std::cout << "DUMPING QUEUE" << std::endl;
#endif
		queue <Figure> board = History.front();
#ifdef DEBUG
		std::cout << "QUEUE LENGTH: " << board.size() << std::endl;
#endif
		break;
		while(!board.empty()){
#ifdef DEBUG
			std::cout << "DUMPING BOARD QUEUE" << std::endl;
#endif
			Figure cell = board.front();
			outstream << (cell.color == 1 ? "W" : "B") << "|" << cell.no << ",";
		}
		outstream << std::endl;
	}
	outstream.close();
	delete filename;
}

int Chess::move(Pos oldPos, Pos newPos){ //Change in position allows lookup for figures.
#ifdef DEBUG
	std::cout << "MOVING FROM (" << oldPos.x() << "," << oldPos.y() << ") TO (" << newPos.x() << "," << newPos.y() << ")" << std::endl;
#endif
    saveHistory();
    Figure *tmp = Board[oldPos.index()]; //Grab figure pointer
    if(!(tmp->move(newPos))) return 0; //Should never be true.  Updates tmp position.
    else if((tmp->no==KING) && (ABS(oldPos.x()-newPos.x())==2)){ //Support "Castleing"
        if(newPos.x() == 2){ //support it for both sides
            Board[0*8+oldPos.y()] = NULL;
        }
        else{
            Board[7*8+oldPos.y()] = NULL;
        }
    }
    //MAKE THE MOVE
    if(Board[newPos.x()*8+newPos.y()]!=NULL){ //Kill piece
        Set[Board[newPos.index()]->index]->alive = false;
    }
    Board[newPos.index()] = tmp; //Sets figure (position) pointer
    Board[oldPos.index()] = NULL;

    //SWAP WHICH PLAYER'S UP
    curr_color = curr_color ? false : true;
    moveInd++;
    //IS THE NEXT PLAYER UP IN CHECK
    King *king = dynamic_cast<King *> (Board[64+curr_color*16]);
    if(king->not_in_danger(Board, curr_color))
        setStatus(0);
    else
        setStatus(1);
    //IS THE NEXT PLAYER UP CHECKMATE
    if(figures_to_move().empty()) {
        setStatus(2+getStatus());
        playing = false;
        saveHistory();
        fileDumpHistory(!curr_color); //write history to file
    }

    if(tmp->no==PAWN && (newPos.y()==0 || newPos.y()==7))
        return 2;
    return 1;
}

bool Chess::changeType(Pos curPos, int newType){
    Figure *old = Board[curPos.index()];
    if(newType==PAWN){
        Pawn *pawn = new Pawn();
        Set[old->index] = pawn;
    }
    else if(newType==QUEEN){
        Queen *queen = new Queen();
        Set[old->index] = queen;
    }
    else if(newType==KNIGHT){
        Knight *knight = new Knight();
        Set[old->index] = knight;
    }
    else if(newType==BISHOP){
        Bishop *bishop = new Bishop();
        Set[old->index] = bishop;
    }
    else if(newType==ROOK){
        Rook *rook = new Rook();
        Set[old->index] = rook;
    }
    else return false;
    Figure *cur = Set[old->index];
    Board[curPos.index()] = cur;
    Board[64+old->index] = cur;

    cur->no = old->no;
    cur->index = old->index;
    cur->color = old->color;
    cur->alive = old->alive;
    cur->curPos = old->curPos;
    cur->val = old->val;
    delete old;

    return Board[curPos.x()*8+curPos.y()]->changeType(newType);
}


#include "figures.h"
#include <iostream>

Figure::Figure(){}
Figure::~Figure(){}

bool Figure::checkBoundaries(const Pos& newPos) {
    if(newPos.x() < 0 || newPos.x() >= 8 || newPos.y() < 0 || newPos.y() >= 8)
        return false;
    return true;
}

int Figure::move(Pos newPos) {
    if(checkBoundaries(newPos)) {
        this->curPos.set(newPos);
        return true;
    }
    return false;
}

void Figure::setPos(int x, int y) {
    this->curPos.set(x,y);
}

vector <Pos> Figure::possible_moves(Figure * Board[]) {
    vector<Pos> empty;
    return empty;
}

bool Figure::changeType(int newType){
    if(no != KING)
        if ( newType!=KING ){
            this->no = newType;
            return true;
        }
    return false;
}

int Figure::ifPossMove( Figure *Board[], bool color, Pos test_Pos, bool _kill, bool _nokill){
    int status = 0;
    King *king = dynamic_cast<King *>(Board[64+color*16]);
    if(!checkBoundaries(test_Pos))
        return 2;
    if(((Board[test_Pos.index()] == NULL)&&(_nokill))||((Board[test_Pos.index()] != NULL)&&((Board[test_Pos.index()]->color != color)&&(_kill)))){
        Figure *help = NULL;
        help = Board[test_Pos.index()];
        Board[test_Pos.index()] = Board[curPos.index()];
        Board[curPos.index()] = NULL;
        status = king->not_in_danger(Board,color);
        Board[curPos.index()] = Board[test_Pos.index()];
        Board[test_Pos.index()] = help;
    }
    if( Board[test_Pos.index()] != NULL )
        status+=2;
    return status;
}

int Figure::ifKingPossMove( Figure *Board[], bool color, Pos test_Pos, Pos oldPos){
    int status = 0;
    King *king = dynamic_cast<King *> (Board[64+color*16]);
    if(!checkBoundaries(test_Pos))
        return 2;
    if((Board[test_Pos.index()] == NULL)||((Board[test_Pos.index()] != NULL)&&((Board[test_Pos.index()]->color != color)))){
        Figure *help = NULL;
        help = Board[test_Pos.index()];
        Board[test_Pos.index()] = Board[oldPos.index()];
        Board[oldPos.index()] = NULL;
        status = king->not_in_danger(Board,color);
        Board[oldPos.index()] = Board[test_Pos.index()];
        Board[test_Pos.index()] = help;
    }
    if( Board[test_Pos.index()] != NULL )
        status+=2;
    return status;
}

///#######################################

Pawn::Pawn(){}
Pawn::~Pawn(){}

vector<Pos> Pawn::possible_moves(Figure *Board[]){
	bool hasNotMoved = (curPos.y() == 1 && color == 0) || (curPos.y() == 7 && color == 1) ? true : false;
	int direction = color == 1 ? -1 : 1;
    vector <Pos> PossMoves;
	Pos test_Pos;
	if(hasNotMoved && Board[curPos.index()+1*direction] == NULL){
#ifdef PAWN_DEBUG
		std::cout << "PAWN HAS NOT MOVED AND IT HAS NOTHING IN FRONT OF IT" << std::endl;
#endif
    	test_Pos.set(curPos.x(), curPos.y()+2*direction);
		if(ifPossMove(Board, color, test_Pos, false, true) == 1){
#ifdef PAWN_DEBUG
		std::cout << "PAWN CAN MOVE TWO STEPS FORWARD" << std::endl;
#endif
			PossMoves.push_back(test_Pos);
		}
	}
	test_Pos.set(curPos.x(), curPos.y()+1*direction);
	if(ifPossMove(Board, color, test_Pos, false, true) == 1){
#ifdef PAWN_DEBUG
		std::cout << "PAWN CAN MOVE ONE STEP FORWARD" << std::endl;
#endif
		PossMoves.push_back(test_Pos);
	}
	test_Pos.set(curPos.x()-1, curPos.y()+1*direction);
	if(ifPossMove(Board, color, test_Pos, true, false) == 1){
#ifdef PAWN_DEBUG
		std::cout << "PAWN CAN MOVE DIAGONALLY ONE STEP TO KILL" << std::endl;
#endif
		PossMoves.push_back(test_Pos);
	}
	test_Pos.set(curPos.x()+1, curPos.y()+1*direction);
	if(ifPossMove(Board, color, test_Pos, true, false) == 1){
#ifdef PAWN_DEBUG
		std::cout << "PAWN CAN MOVE DIAGONALLY ONE STEP TO KILL" << std::endl;
#endif
		PossMoves.push_back(test_Pos);
	}	
    return PossMoves;
}

///########################################

Queen::Queen(){}
Queen::~Queen(){}

vector<Pos> Queen::possible_moves(Figure *Board[]){
    vector <Pos> PossMoves;
    Pos test_Pos;
    for(int z=0; z<9; z++){
        for(int i = 1; i < 8; i++){
            test_Pos.set(curPos.x()+i*(z/3-1), curPos.y()+i*(z%3-1));
            int status = ifPossMove(Board, color, test_Pos);
            if(status&1)
                PossMoves.push_back(test_Pos);
			if(status&2)
				break;
        }
    }
    return PossMoves;
}

///########################################

Rook::Rook(){}
Rook::~Rook(){}

vector <Pos> Rook::possible_moves(Figure *Board[]){
    vector <Pos> PossMoves;
    Pos test_Pos;
    for(int z=1; z<9; z+=2){
        for(int i = 1; i < 8; i++){
            test_Pos.set(curPos.x()+i*(z/3-1),curPos.y()+i*(z%3-1));
            int status = ifPossMove(Board,color,test_Pos);
            if(status&1)
                PossMoves.push_back(test_Pos);
            if(status&2)
                break;
        }
    }
    return PossMoves;
}

///########################################

Bishop::Bishop(){}
Bishop::~Bishop(){}

vector <Pos>Bishop::possible_moves(Figure *Board[]){
    vector <Pos> PossMoves;
    Pos test_Pos;
    for(int z=0; z<9; z+=2){
        for(int i = 1; i < 8; i++){
            test_Pos.set(curPos.x()+i*(z/3-1),curPos.y()+i*(z%3-1));
            int status = ifPossMove(Board,color,test_Pos);
            if(status&1)
                PossMoves.push_back(test_Pos);
            if(status&2)
                break;
        }
    }
    return PossMoves;
}

///########################################

Knight::Knight(){}
Knight::~Knight(){}

vector<Pos> Knight::possible_moves(Figure *Board[]){
    int kn_mvs[8][2]={{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}};
    vector <Pos> PossMoves;
    Pos test_Pos;
    for(int i=0;i<8;i++){	 	
        test_Pos.set(curPos.x()+kn_mvs[i][0],curPos.y()+kn_mvs[i][1]);
        if(ifPossMove(Board,color,test_Pos)&1){
            PossMoves.push_back(test_Pos);
		}
    }
    return PossMoves;
}

///########################################

King::King(){}
King::~King(){}

vector<Pos> King::possible_moves(Figure *Board[]){
    vector <Pos> PossMoves;
    Pos test_Pos, oldPos = curPos;
    for(int i=0;i<9;i++){
        test_Pos.set(oldPos.x()+i/3-1,oldPos.y()+i%3-1);
        curPos = test_Pos;
        if(ifKingPossMove(Board,color,test_Pos,oldPos)&1){
            PossMoves.push_back(test_Pos);
            if(i==7){
                test_Pos.set(oldPos.x()+2,oldPos.y());
                curPos = test_Pos;
                if(ifKingPossMove(Board,color,test_Pos,oldPos)&1)
                    PossMoves.push_back(test_Pos);
            }
            else if(i==1){
                test_Pos.set(oldPos.x()-2,oldPos.y());
                curPos = test_Pos;
                if(ifKingPossMove(Board,color,test_Pos,oldPos)&1)
                    PossMoves.push_back(test_Pos);
            }
        }
    }
    curPos = oldPos;
    return PossMoves;
}

bool King::not_in_danger( Figure *Board[], bool color){
    Pos test_Pos;
    Pos cP = Board[64+color*16]->curPos;	
    for(int i=0;i<9;i++){ 
        test_Pos.set(cP.x()+i/3-1,cP.y()+i%3-1);
        if(checkBoundaries(test_Pos))
            if(Board[(cP.x()+i/3-1)*8+cP.y()+i%3-1]!=NULL)
                if(Board[(cP.x()+i/3-1)*8+cP.y()+i%3-1]->color!=color&&Board[(cP.x()+i/3-1)*8+cP.y()+i%3-1]->no==KING)
                    return false;
    }
    int kn_mvs[8][2]={{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}};
    for(int i = 0; i < 8; i++){ 
        test_Pos.set(cP.x()+kn_mvs[i][0],cP.y()+kn_mvs[i][1]);
        if(checkBoundaries(test_Pos))
            if(Board[(cP.x()+kn_mvs[i][0])*8+cP.y()+kn_mvs[i][1]]!=NULL)
                if(Board[(cP.x()+kn_mvs[i][0])*8+cP.y()+kn_mvs[i][1]]->color!=color&&Board[(cP.x()+kn_mvs[i][0])*8+cP.y()+kn_mvs[i][1]]->no==KNIGHT)
                    return false;
    }
    for(int z=1;z<9;z+=2){
        for(int i=1;i<7;i++){
            test_Pos.set(cP.x()+i*(z/3-1),cP.y()+i*(z%3-1));
            if(checkBoundaries(test_Pos)) {
                if(Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]!=NULL) {
                    if(Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]->color!=color&&
                       (Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]->no==ROOK||Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]->no==QUEEN)) {
                        return false;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    for(int z=0;z<9;z+=2){ 
        for(int i=1;i<7;i++){
            test_Pos.set(cP.x()+i*(z/3-1),cP.y()+i*(z%3-1));
            if(checkBoundaries(test_Pos)) {
                if(Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]!=NULL) {
                    if(Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]->color!=color&&
                       (Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]->no==BISHOP||Board[(cP.x()+i*(z/3-1))*8+cP.y()+i*(z%3-1)]->no==QUEEN)) {
                        return false;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    test_Pos.set(cP.x()+1,cP.y()+1-color*2);
    if(checkBoundaries(test_Pos))
        if(Board[(cP.x()+1)*8+cP.y()+1-color*2]!=NULL)
            if(Board[(cP.x()+1)*8+cP.y()+1-color*2]->color!=color&&Board[(cP.x()+1)*8+cP.y()+1-color*2]->no==PAWN)
                return false;
    test_Pos.set(cP.x()-1,cP.y()+1-color*2);
    if(checkBoundaries(test_Pos))
        if(Board[(cP.x()-1)*8+cP.y()+1-color*2]!=NULL)
            if(Board[(cP.x()-1)*8+cP.y()+1-color*2]->color!=color&&Board[(cP.x()-1)*8+cP.y()+1-color*2]->no==PAWN)
                return false;
    return true;
}

Blank::Blank(){
	no = 10;
}

Blank::~Blank(){}


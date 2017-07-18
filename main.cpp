#include "includes.h"

int main(int argc, char *argv[])
{
	std::cout << "IN MAIN" << std::endl;
	Chess * chess = new Chess();
	std::cout << "CHESS GAME INITIALIZED" << std::endl;
	AI * AI0 = new AI(WHITE, chess);
	AI * AI1 = new AI(BLACK, chess);
	while(chess->playing){
		if(chess->curr_color) AI0->makeNextMove();
		else AI1->makeNextMove();
		std::cout << chess->moveInd << " MOVES MADE!" << std::endl;
		break;
	}
    return 0;
}

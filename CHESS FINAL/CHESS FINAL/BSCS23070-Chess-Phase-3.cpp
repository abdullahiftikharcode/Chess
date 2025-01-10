#include<iostream>
#include<windows.h>
#include<stdlib.h>
#include<conio.h>
#include<cstdlib>
#include<string>
#include<fstream>
#include<mmsystem.h>
#include<Windows.h>
const int rows = 93/2;
const int cols = 168/2;
using namespace std;
#pragma comment (lib,"winmm.lib")
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
struct move2 {
	int rook0[2];
	int king0;
	int rook1[2];
	int king1;
};
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
struct positon {
	int r;
	int c;
};
struct kill {
	char killed;
	positon kill;
	int color;
};
struct players {
	int color;
	string names;
	int moves;
};
struct chess_piece {
	int clr;
	char SYM;
	positon piece;
	int turn;
	positon ori;
	positon desti;
	bool cast;
};
enum {
	YELLOW = 14
};
void gameexit();
void gameexit() {
	gotoRowCol(rows , 0);
	cout << "  _____  _                    _                             __                       _                _                   __ _____              \n";
	cout << " |_   _|| |                  | |                           / _|                     | |              (_)                 / /|____ |             \n";
	cout << "   | |  | |__    __ _  _ __  | | __  _   _   ___   _   _  | |_   ___   _ __   _ __  | |  __ _  _   _  _  _ __    __ _   / /     / /             \n";
	cout << "   | |  | '_ \\  / _` || '_ \\ | |/ / | | | | / _ \\ | | | | |  _| / _ \\ | '__| | '_ \\ | | / _` || | | || || '_ \\  / _` | < <      \\ \\             \n";
	cout << "   | |  | | | || (_| || | | ||   <  | |_| || (_) || |_| | | |  | (_) || |    | |_) || || (_| || |_| || || | | || (_| |  \\ \\ .___/ / _  _  _  _  \n";
	cout << "   \\_/  |_| |_| \\__,_||_| |_|\\_|\\_\\  \\__, | \\___/  \\__,_| |_|   \\___/ |_|    | .__/ |_| \\__,_| \\__, ||_||_| |_| \\__, |   \\_\\____/ (_)(_)(_)(_)\n";
	cout << "                                      __/ |                                  | |                __/ |            __/ |                          \n";
	cout << "                                     |___/                                   |_|               |___/            |___/                           \n";
	_getch();
	exit(1);
}
bool validmove(positon desti, positon ori, int turn, chess_piece** BOARD, players* p, int dimensions, move2 castle);
positon kingsearch(chess_piece** B, int dimensions, int turn, players* p, bool mate);
void highlight(chess_piece** BOARD, positon ori, int dimensions, int turn, players* po, char sym1, char sym2, move2
	castle);
void convert(chess_piece** BOARD, int dimensions, positon co, int& r, int& c) {
	bool find = false;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((co.r == BOARD[i][j].piece.r) && (co.c == BOARD[i][j].piece.c)) {
				r = i;
				c = j;
				find = true;
				break;
			}
		}
		if (find) {
			break;
		}
	}
}
void boardmaking(chess_piece**& BOARD, int dimensions, players* p) {
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (i == 1) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'P';
				BOARD[i][j].clr = p[0].color;
			}
			else if (i == 6) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'P';
				BOARD[i][j].clr = p[1].color;
			}
			else if (i == 7 && j == 3) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'Q';
				BOARD[i][j].clr = p[1].color;
			}
			else if (i == 7 && j == 4) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'K';
				BOARD[i][j].clr = p[1].color;
			}
			else if (i == 0 && j == 3) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'Q';
				BOARD[i][j].clr = p[0].color;
			}
			else if (i == 0 && j == 4) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'K';
				BOARD[i][j].clr = p[0].color;
			}
			else if ((j == 1 && i == 7) || (i == 7 && j == 6)) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'H';
				BOARD[i][j].clr = p[1].color;
			}
			else if ((j == 1 && i == 0) || (i == 0 && j == 6)) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'H';
				BOARD[i][j].clr = p[0].color;
			}
			else if ((j == 0 && i == 0) || (i == 0 && j == 7)) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'R';
				BOARD[i][j].clr = p[0].color;
			}
			else if ((j == 0 && i == 7) || (i == 7 && j == 7)) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'R';
				BOARD[i][j].clr = p[1].color;
			}
			else if ((j == 2 && i == 7) || (i == 7 && j == 5)) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'B';
				BOARD[i][j].clr = p[1].color;
			}
			else if ((j == 2 && i == 0) || (i == 0 && j == 5)) {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = 'B';
				BOARD[i][j].clr = p[0].color;
			}
			else {
				BOARD[i][j].piece.r = (rows / 2) - 12 + (8 * i);
				BOARD[i][j].piece.c = (cols / 2) - 11 + (8 * j);
				BOARD[i][j].SYM = '_';
				BOARD[i][j].clr = 7;
			}
		}
	}
}
void HideConsoleCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void choosecolour(int& clr) {
	int fc = (cols ) - 10, fr = rows;
	int choicer = 0, choicec = 0;
	do {
		system("cls");
		gotoRowCol(fr - 3, fc); cout << " -------------------------- ";
		gotoRowCol(fr - 2, fc); cout << " |    Choose colour       | ";
		gotoRowCol(fr - 1, fc); cout << " --------------------------";
		gotoRowCol(fr + 1, fc); cout << "  --------------------------";
		SetClr(3);
		gotoRowCol(fr + 2, fc); cout << " |         BLUE            |";
		SetClr(7);
		gotoRowCol(fr + 3, fc); cout << " --------------------------";
		SetClr(5);
		gotoRowCol(fr + 4, fc); cout << "|        Purple            |";
		SetClr(7);
		gotoRowCol(fr + 5, fc); cout << " --------------------------";
		SetClr(6);
		gotoRowCol(fr + 6, fc); cout << "|         Orange           |";
		SetClr(7);
		gotoRowCol(fr + 7, fc); cout << " --------------------------";
		SetClr(1);
		gotoRowCol(fr + 8, fc); cout << "|      Dark Blue           |";
		SetClr(7);
		gotoRowCol(fr + 9, fc); cout << " --------------------------";
		getRowColbyLeftClick(choicer, choicec);
		if (choicer == fr + 2) {
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			clr = 3;
			system("cls");
			return;
		}
		if (choicer == fr + 4) {
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			clr = 5;
			system("cls");
			return;
		}
		else if (choicer == fr + 6) {
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			clr = 6;
			system("cls");
			return;
		}
		else if (choicer == fr + 8) {
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			clr = 1;
			system("cls");
			return;
		}
	} while (true);

}
void instructions() {
	system("cls");
	cout << " _________ _        _______ _________ _______           _______ __________________ _______  _        _______ \n"
		"\\__   __/( (    /|(  ____ \\__   __/(  ____ )|\\     /|(  ____ \\__   __/\\__   __/(  ___  )( (    /|(  ____ \\\n"
		"   ) (   |  \\  ( || (    \\/   ) (   | (    )|| )   ( || (    \\/   ) (      ) (   | (   ) ||  \\  ( || (    \\/\n"
		"   | |   |   \\ | || (_____    | |   | (____)|| |   | || |         | |      | |   | |   | ||   \\ | || (_____ \n"
		"   | |   | (\\ \\) |(_____  )   | |   |     __)| |   | || |         | |      | |   | |   | || (\\ \\) |(_____  )\n"
		"   | |   | | \\   |      ) |   | |   | (\\ (   | |   | || |         | |      | |   | |   | || | \\   |      ) |\n"
		"___) (___| )  \\  |      / |   | |   | ) \\ \\__| (___) || (____/\\   | |   ___) (___| (___) || )  \\  |/\\____) |\n"
		"\\_______/|/    )_)     (___)   )_(   |/   \\__/(_______)(_______/   )_(   \\_______/(_______)|/    )_)\\_______)\n";

	cout << endl << "This game of chess is as same as any other games including every rule with all of the legalties you can press space during replay to play game from that point";
	cout << endl << "PRESS ANY KEY TO RETURN TO MENU";
	_getch();
	PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
	return;
}
void continuemsg() {
	gotoRowCol(rows, 0);
	SetClr(2);
	cout << " _____         ____       ____     ______      _____      __      _      _____          _____      ____       __    __      _____" << std::endl;
	cout << "(_   _)       / __ \\     (    )   (_  __ \\    (_   _)    /  \\    / )    / ___ \\        / ___ \\    (    )      \\ \\  / /     / ___/" << std::endl;
	cout << "  | |        / /  \\ \\    / /\\ \\     ) ) \\ \\     | |     / /\\ \\  / /    / /   \\_)      / /   \\_)   / /\\ \\      () \\/ ()    ( (__" << std::endl;
	cout << "  | |       ( ()  () )  ( (__) )   ( (   ) )    | |     ) ) ) ) ) )   ( (  ____      ( (  ____   ( (__) )     / _  _ \\     ) __)" << std::endl;
	cout << "  | |   __  ( ()  () )   )    (     ) )  ) )    | |    ( ( ( ( ( (    ( ( (__  )     ( ( (__  )   )    (     / / \\/ \\ \\   ( (" << std::endl;
	cout << "__| |___) )  \\ \\__/ /   /  /\\  \\   / /__/ /    _| |__  / /  \\ \\/ /     \\ \\__/ /       \\ \\__/ /   /  /\\  \\   /_/      \\_\\   \\ \\___   __   __   __" << std::endl;
	cout << "\\________/    \\____/   /__(  )__\\ (______/    /_____( (_/    \\__/       \\____/         \\____/   /__(  )__\\ (/          \\)   \\____\\ (__) (__) (__) (__) " << std::endl;
	Sleep(3000);
	SetClr(7);
}
void continuenewmsg() {
	gotoRowCol(rows/2, 0);
	SetClr(2);
	cout <<
		" _____         ____       ____     ______      _____      __      _      _____          __      _    _____   ___       ___     \n"
		"(_   _)       / __ \\     (    )   (_  __ \\    (_   _)    /  \\    / )    / ___ \\        /  \\    / )  / ___/  (  (       )  )    \n"
		"  | |        / /  \\ \\    / /\\ \\     ) ) \\ \\     | |     / /\\ \\  / /    / /   \\_)      / /\\ \\  / /  ( (__     \\  \\  _  /  /     \n"
		"  | |       ( ()  () )  ( (__) )   ( (   ) )    | |     ) ) ) ) ) )   ( (  ____       ) ) ) ) ) )   ) __)     \\  \\/ \\/  /      \n"
		"  | |   __  ( ()  () )   )    (     ) )  ) )    | |    ( ( ( ( ( (    ( ( (__  )     ( ( ( ( ( (   ( (         )   _   (       \n"
		"__| |___) )  \\ \\__/ /   /  /\\  \\   / /__/ /    _| |__  / /  \\ \\/ /     \\ \\__/ /      / /  \\ \\/ /    \\ \\___     \\  ( )  /       \n"
		"\\________/    \\____/   /__(  )__\\ (______/    /_____( (_/    \\__/       \\____/      (_/    \\__/      \\____\\     \\_/ \\_/        \n"
		"                                                                                                                                  \n"
		" ______      ____       ____     ______     ______                                  \n"
		"(_   _ \\    / __ \\     (    )   (   __ \\   (_  __ \\                                 \n"
		"  ) (_) )  / /  \\ \\    / /\\ \\    ) (__) )    ) ) \\ \\                                \n"
		"  \\   _/  ( ()  () )  ( (__) )  (    __/    ( (   ) )                               \n"
		"  /  _ \\  ( ()  () )   )    (    ) \\ \\  _    ) )  ) )                               \n"
		" _) (_) )  \\ \\__/ /   /  /\\  \\  ( ( \\ \\_))  / /__/ /   __   __   __   __   __   __  \n"
		"(______/    \\____/   /__(  )__\\  )_) \\__/  (______/   (__) (__) (__) (__) (__) (__) \n"
		<< endl;
	SetClr(7);
	Sleep(3000);
}
void loading(bool& load) {
	bool continue2 = false;
	ifstream rdr("BSCS23070-Chess-Phase-3-savefile.txt");
	string s{};
	while (rdr >> s) {
		continue2 = true;
		break;
	}
	system("cls");
	int fc = (cols)-10, fr = rows;
	bool active;
	system("cls");
	for (int i = 2; i < fr - 15; i++) {
		system("cls");
		SetClr(12);
		gotoRowCol(2 + i, fc - 10);
		cout << " _____    __   __      _____   ______   ______  " << endl;
		gotoRowCol(3 + i, fc - 10);
		cout << "/\\ __/\\  /\\_\\ /_/\\    /\\_____/ / ____/\\ / ____/\\ " << endl;
		gotoRowCol(4 + i, fc - 10);
		cout << ") )__\\/) ( ( (_) ) )  ( (_____/ ) ) __\\/ ) ) __\\/ " << endl;
		gotoRowCol(5 + i, fc - 10);
		cout << "/ / /     \\ \\___/ /    \\ \\__\\    \\ \\ \\   \\ \\ \\   " << endl;
		gotoRowCol(6 + i, fc - 10);
		cout << "\\ \\ \\_    / / _ \\ \\    / /__/_   _\\ \\ \\  _\\ \\ \\  " << endl;
		gotoRowCol(7 + i, fc - 10);
		cout << ") )__\\/ ( (_( )_) )  ( (_____/ )____) ) )____) ) " << endl;
		gotoRowCol(8 + i, fc - 10);
		cout << "\\/___\\/   \\/_/ \\_\\/    \\/_____/ \\____\\/  \\____\\/  " << endl;
		SetClr(14);
		gotoRowCol(3 * rows / 2 - i / 2 + 1, fc-10);
		cout << "                                                 _:_" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 2, fc-10);
		cout << "                                                 '-.-'" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 3, fc-10);
		cout << "                                        ()      __.'.__" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 4, fc - 10);
		cout << "                                     .-:--:-.  |_______|" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 5, fc - 10);
		cout << "                              ()      \\____/    \\=====/ " << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 6, fc - 10);
		cout << "                              /\\      {====}     )___(" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 7, fc - 10);
		cout << "                   (\\=,      //\\\\      )__(     /_____\\ " << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 8, fc - 10);
		cout << "   __    |'-'-'|  //  .\\    (    )    /____\\     |   |" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 9, fc - 10);
		cout << "  /  \\   |_____| (( \\_  \\    )__(      |  |      |   |" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 10, fc - 10);
		cout << "  \\__/    |===|   ))  `\\_)  /____\\     |  |      |   |" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 11, fc - 10);
		cout << " /____\\   |   |  (/     \\    |  |      |  |      |   |" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 12, fc - 10);
		cout << "  |  |    |   |   | _.-'|    |  |      |  |      |   |" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 13, fc - 10);
		cout << "  |__|    )___(    )___(    /____\\    /____\\    /_____\\ " << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 14, fc - 10);
		cout << " (====)  (=====)  (=====)  (======)  (======)  (=======)" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 15, fc - 10);
		cout << " }===={  }====={  }====={  }======{  }======{  }======={" << endl;

		gotoRowCol(3 * rows / 2 - i / 2 + 16, fc - 10);
		cout << "(______)(_______)(_______)(________)(________)(_________)" << endl;
		Sleep(50);
		(SetClr(7));
	}
	
	do {
		active = false;
		int choicer, choicec;
		gotoRowCol(fr -2, fc); cout << " -------------------------- ";
		gotoRowCol(fr - 1, fc); cout << " |      CHOOSE OPTION      | ";
		gotoRowCol(fr , fc); cout << " --------------------------";
		if (continue2) {
			gotoRowCol(fr + 1, fc); cout << "  --------------------------";
			gotoRowCol(fr + 2, fc); cout << " |         CONTINUE         |";
		}
		gotoRowCol(fr + 3, fc); cout << "  --------------------------";
		gotoRowCol(fr + 4, fc); cout << " |       START NEW GAME     |";
		gotoRowCol(fr + 5, fc); cout << "  --------------------------";
		gotoRowCol(fr + 6, fc); cout << " |        Intructions       |";
		gotoRowCol(fr + 7, fc); cout << "  --------------------------";
		gotoRowCol(fr + 8, fc); cout << " |           EXIT           |";
		gotoRowCol(fr + 9, fc); cout << "  --------------------------";
		getRowColbyLeftClick(choicer, choicec);
		if (choicer == fr + 2) {
			load = true;
			system("cls");
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			continuemsg();
			system("cls");
			return;
		}
		else if (choicer == fr + 4) {
			load = false;
			system("cls");
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			continuenewmsg();
			system("cls");
			return;
		}
		else if (choicer == fr + 6) {
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			instructions();
			system("cls");
		}
		else if (choicer == fr + 8) {
			load = false;
			system("cls");
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			gameexit();
		}
	} while (true);

}
void loadboard(chess_piece**& BOARD, int dimensions, players*& p, int& turn, move2 castle) {
	ifstream rdr("BSCS23070-Chess-Phase-3-savefile.txt");
	if (!rdr) {
		cout << "FILE COULDN'T BE LOADED...";
		exit(1);
	}
	rdr >> turn;
	rdr >> p[0].names >> p[0].color >> p[0].moves >> p[1].names >> p[1].color >> p[1].moves;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			rdr >> BOARD[i][j].clr >> BOARD[i][j].SYM >> BOARD[i][j].piece.r >> BOARD[i][j].piece.c;
		}
	}
	rdr >> castle.king0 >> castle.king1 >> castle.rook0[0] >> castle.rook0[1] >> castle.rook1[0] >> castle.rook1[1]
		>>BOARD[0][0].ori.r>> BOARD[0][0].ori.c>> BOARD[0][0].desti.c>> BOARD[0][0].desti.r;
}
void innit(chess_piece**& BOARD, int dimensions, players*& p, chess_piece***& Bhistory, int& move
	, int& turn, move2 castle, kill * &piece) {
	
	bool load = false;
	loading(load);
	p = new players[2]{};
	int clr = 0;
	bool clr2 = 0;
	piece = new kill  [3] {};
	BOARD = new chess_piece * [dimensions] {};
	Bhistory = new chess_piece * *[4] {};
	for (int i = 0; i < 4; i++) {
		Bhistory[i] = new chess_piece * [dimensions] {};
		for (int j = 0; j < dimensions; j++) {
			Bhistory[i][j] = new chess_piece[dimensions]{};
		}
	}
	for (int i = 0; i < dimensions; i++) {
		BOARD[i] = new chess_piece[dimensions]{};
	}
	int lenght = 19;
	char sym = -37;
	if (!load) {
		//choosecolour(clr);
		for (int i = 0; i < 2; i++) {
			gotoRowCol(rows/2 , cols );
			cout << "ENTER PLAYER " << i + 1 << " name :";
			for (int i = 0; i < 20; i++) {
				gotoRowCol(rows/2 + 2, (cols ) + i);
				cout << sym;
			}
			gotoRowCol((rows/2 ) + 3, cols );
			cout << sym;
			gotoRowCol((rows / 2) + 4, cols);
			cout << sym;
			for (int i = 0; i < 20; i++) {
				gotoRowCol(rows/2 + 5, (cols) + i);
				cout << sym;
			}
			gotoRowCol(rows/2  + 3, cols  + 19);
			cout << sym;
			gotoRowCol(rows / 2 + 4, cols + 19);
			cout << sym;
			gotoRowCol(rows/2  + 4, cols  + 2);
			getline(cin, p[i].names);
			PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			do {
				clr2 = false;
				choosecolour(clr);
				p[i].color = clr;
				if (p[1].color == p[0].color) {
					clr2 = true;
					cout << "You can not choose the same colour as other player.....";
				}
			} while (clr2);
		}
		boardmaking(BOARD, dimensions, p);
		for (int i = 0; i < dimensions; i++) {
			for (int j = 0; j < dimensions; j++) {
				Bhistory[move][i][j] = BOARD[i][j];
				Bhistory[move][i][j].turn = turn;
			}
		}
	}
	else {
		loadboard(BOARD, dimensions, p, turn, castle);
		for (int i = 0; i < dimensions; i++) {
			for (int j = 0; j < dimensions; j++) {
				Bhistory[move][i][j] = BOARD[i][j];
				Bhistory[move][i][j].turn = turn;
			}
		}
	}
	
}
void turnmsg(int turn, players* name, char sym = -37) {
	int size1 = name[(turn+1)%2].names.size();
	for (int j = 0; j < 3; j += 2) {
		for (int i = 0; i <= size1 + 10; i++) {
			gotoRowCol(1 + j, 1 + i);
			cout << " ";
		}
	}
	gotoRowCol(2, 0);
	cout << "                                                    ";
	int size = name[turn].names.size();
	gotoRowCol(0, 0);
	cout << "PLAYER TURN :";
	SetClr(name[turn].color);
	for (int j = 0; j < 3; j += 2) {
		for (int i = 0; i <= size + 10; i++) {
			gotoRowCol(1 + j, 1 + i);
			cout << sym;
		}
	}
	gotoRowCol(2, 1);
	cout << sym;
	gotoRowCol(2, size + 11);
	cout << sym;
	SetClr(7);
	gotoRowCol(2, 2);
	cout << name[turn].names << " 's TURN" << endl;
	
}
void pawndisplay(int i, int j, char sym) {
	gotoRowCol(((rows / 2) - 12 + 8 * i), ((cols / 2) - 10 + 8 * j));
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 11 + 8 * j));
	cout << sym << sym << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 12 + 8 * j));
	cout << sym << sym << sym << sym << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 12 + 8 * j));
	cout << sym << sym << sym << sym << sym;
}
void bishopdisplay(int i, int j, char sym) {
	gotoRowCol(((rows / 2) - 12 + 8 * i) - 1, ((cols / 2) - 10 + 8 * j));
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i), ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) + 1);
	cout << sym << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j));
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 4, ((cols / 2) - 10 + 8 * j));
	cout << sym;
}
void queendisplay(int i, int j, char sym) {
	gotoRowCol(((rows / 2) - 12 + 8 * i) - 1, ((cols / 2) - 10 + 8 * j));
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i), ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) + 2);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j));
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j) + 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 10 + 8 * j) + 2);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 4, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym << sym << sym << sym << endl;
}
void kingdisplay(int i, int j, char sym) {
	gotoRowCol(((rows / 2) - 12 + 8 * i), ((cols / 2) - 10 + 8 * j) + 1);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) - 1);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) + 1);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) + 3);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j));
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j) + 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 10 + 8 * j) - 1);
	cout << sym << sym << sym << sym << sym << endl;
}
void knightdisplay(int i, int j, char sym) {
	gotoRowCol(((rows / 2) - 12 + 8 * i) - 1, ((cols / 2) - 10 + 8 * j));
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i), ((cols / 2) - 10 + 8 * j) + 1);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i), ((cols / 2) - 10 + 8 * j) - 1);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 10 + 8 * j) + 1);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 10 + 8 * j) - 1);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j) - 1);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j) + 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) - 1);
	cout << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) + 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 4, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym << sym << sym << sym << sym << endl;
}
void rookdisplay(int i, int j, char sym) {
	gotoRowCol(((rows / 2) - 12 + 8 * i), ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 1, ((cols / 2) - 10 + 8 * j) + 2);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j) - 1);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 2, ((cols / 2) - 10 + 8 * j) + 1);
	cout << sym;
	gotoRowCol(((rows / 2) - 12 + 8 * i) + 3, ((cols / 2) - 10 + 8 * j) - 2);
	cout << sym << sym << sym << sym << sym << endl;
}
void displayboard(chess_piece** BOARD, int  dimensions, int turn, players* p) {
	char sym = -37;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (BOARD[i][j].SYM != '_') {
				SetClr((BOARD[i][j].clr));
				if (BOARD[i][j].SYM == 'P') {
					pawndisplay(i, j, sym);
				}
				else if (BOARD[i][j].SYM == 'B') {
					bishopdisplay(i, j, sym);
				}
				else if (BOARD[i][j].SYM == 'K') {
					kingdisplay(i, j, sym);
				}
				else if (BOARD[i][j].SYM == 'Q') {
					queendisplay(i, j, sym);
				}
				else if (BOARD[i][j].SYM == 'H') {
					knightdisplay(i, j, sym);

				}
				else if (BOARD[i][j].SYM == 'R') {
					rookdisplay(i, j, sym);
				}
				SetClr(7);
			}
			if (BOARD[i][j].SYM == '_') {
				SetClr(7);

			}
		}
		gotoRowCol((rows / 2) - 12 + 8 * i, (cols / 2) + 7 * 8);
		cout << 8 - i;
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < dimensions; i++) {
		gotoRowCol((rows / 2) - 14 + (8 * 8) + 4, (cols / 2) - 11 + 8 * i);
		cout << char(i + 65) << " ";
	}
	turnmsg(turn, p);
}
bool notout(chess_piece** BOARD, int dimensions, positon ori) {
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((ori.r == BOARD[i][j].piece.r) && (ori.c == BOARD[i][j].piece.c)) {
				return true;
			}
		}
	}
	return false;
}
int replacechar(char cha, int  ori) {
	cha = toupper(cha);
	if (cha == 'A') {
		return 0;
	}
	else if (cha == 'B') {
		return 1;
	}
	else if (cha == 'C') {
		return 2;
	}
	else if (cha == 'D') {
		return 3;
	}
	else if (cha == 'E') {
		return 4;
	}
	else if (cha == 'F') {
		return 5;
	}
	else if (cha == 'G') {
		return 6;
	}
	else if (cha == 'H') {
		return 7;
	}
}
void chnagenum(int& ori) {
	ori = 8 - ori - 1 + 1;
}
void next_turn(int& turn) {
	turn = (turn + 1) % 2;
}
void deallocation(chess_piece***& temp, int move, int dimensions) {
	for (int i = 0; i < move + 3; i++) {
		for (int j = 0; j < dimensions; j++) {
			delete[]temp[i][j];
		}
		delete[]temp[i];
	}
	//delete[]temp;
	//temp = nullptr;
}
void saveboard(chess_piece** BOARD, chess_piece***& Bhistory, int turn, int dimensions, int move
	, positon ori, positon desti) {
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			Bhistory[move][i][j] = BOARD[i][j];
			Bhistory[move][i][j].turn = turn;
		}
	}
	if (BOARD[0][0].cast != true) {
		BOARD[0][0].ori = ori;
		BOARD[0][0].desti = desti;
	}
	Bhistory[move][0][0].ori = BOARD[0][0].ori;
	Bhistory[move][0][0].desti = BOARD[0][0].desti;
	chess_piece*** temp = new chess_piece * *[move + 3] {};
	for (int i = 0; i < move + 3; i++) {
		temp[i] = new chess_piece * [dimensions] {};
		for (int j = 0; j < dimensions; j++) {
			temp[i][j] = new chess_piece[dimensions]{};
			for (int k = 0; k < dimensions; k++) {
				temp[i][j][k] = Bhistory[i][j][k];
			}
		}
	}
	deallocation(Bhistory, move, dimensions);
	Bhistory = new chess_piece * *[move + 4] {};
	for (int i = 0; i < move + 4; i++) {
		Bhistory[i] = new chess_piece * [dimensions];
		for (int j = 0; j < dimensions; j++) {
			Bhistory[i][j] = new chess_piece[dimensions];
		}
	}

	for (int i = 0; i < move + 3; i++) {
		for (int j = 0; j < dimensions; j++) {
			for (int k = 0; k < dimensions; k++) {
				Bhistory[i][j][k] = temp[i][j][k];
			}
		}
	}
	deallocation(temp, move, dimensions);
}
void guiremove(chess_piece** BOARD, chess_piece*** Bhistory, int dimensions, int move, int i, int j) {
	int r1 = 0, r2 = 0, c1 = 0, c2 = 0;
	convert(BOARD, dimensions, Bhistory[move][i][j].ori, r2, c2);
	convert(BOARD, dimensions, Bhistory[move][i][j].desti, r1, c1);
	char sym = -37;
	for (int p = Bhistory[move][r1][c1].piece.r - 2; p < Bhistory[move][r1][c1].piece.r + 6; p++) {
		for (int o = Bhistory[move][r1][c1].piece.c - 2; o < Bhistory[move][r1][c1].piece.c + 5; o++)
		{
			if (((r1 + 1) + (c1 + 1)) % 2 == 0) {
				SetClr(7);
			}
			else {
				SetClr(0);
			}
			gotoRowCol(p, o);
			cout << sym;
			SetClr(7);
		}
	}
		for (int p = Bhistory[move][r2][c2].piece.r - 2; p < Bhistory[move][r2][c2].piece.r + 6; p++) {
			for (int o = Bhistory[move][r2][c2].piece.c - 2; o < Bhistory[move][r2][c2].piece.c + 5; o++)
			{
				if (((r2 + 1) + (c2 + 1)) % 2 == 0) {
					SetClr(7);
				}
				else {
					SetClr(0);
				}
				gotoRowCol(p, o);
				cout << sym;
				SetClr(7);
			}
		}
		
}
void pawndisplay2(int rows, positon kill, char sym = -37) {
	gotoRowCol(rows, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c);
	cout << sym;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 1);
	cout << sym << sym << sym;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym;
	gotoRowCol(rows + 3, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym;
}
void queendisplay2(int rows, positon kill, char sym = -37) {
	gotoRowCol(rows, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c);
	cout << sym << endl;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 2);
	cout << sym << endl;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c);
	cout << sym;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 2);
	cout << sym;
	gotoRowCol(rows + 3, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym;
	gotoRowCol(rows + 3, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 2);
	cout << sym << endl;
	gotoRowCol(rows + 4, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym << endl;
}
void bishopdisplay2(int rows, positon kill, char sym = -37) {
	gotoRowCol(rows - 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c);
	cout << sym << endl;
	gotoRowCol(rows, ((3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c) - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 1);
	cout << sym << sym << endl;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c);
	cout << sym;
	gotoRowCol(rows + 3, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(rows + 4, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c);
	cout << sym;
}
void knightdisplay2(int rows, positon kill, char sym = -37) {
	gotoRowCol(rows - 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c);
	cout << sym << endl;
	gotoRowCol(rows, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 1);
	cout << sym << endl;
	gotoRowCol(rows, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 1);
	cout << sym << endl;
	gotoRowCol(rows + 3, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 1);
	cout << sym << endl;
	gotoRowCol(rows + 3, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 1);
	cout << sym << endl;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 1);
	cout << sym << endl;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 2);
	cout << sym;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 1);
	cout << sym << endl;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 2);
	cout << sym;
	gotoRowCol(rows + 4, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym << sym << endl;
}
void rookdisplay2(int rows, positon kill, char sym = -37) {
	gotoRowCol(rows, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym << endl;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym;
	gotoRowCol(rows + 1, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 2);
	cout << sym;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 1);
	cout << sym;
	gotoRowCol(rows + 2, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c + 1);
	cout << sym;
	gotoRowCol(rows + 3, (3 * cols / 2 - 8 * 2) + 5 + 8 * kill.c - 2);
	cout << sym << sym << sym << sym << sym << endl;
}
void onlyboard();
void highlight(chess_piece** BOARD, positon ori, int dimensions, int turn, players* po, char sym1, char sym2, move2
	castle, bool un);
void highlightchechk(chess_piece** B, int dimensions, int turn, players* p, int sym1, int sym2, positon& k
	, bool un);
void undoredo(chess_piece**& BOARD, chess_piece***& Bhistory, int& turn, int dimensions, int& move,
	int& smove, int r, int c, kill*& pieces, players* p, positon killo[], positon desti, move2 castle
	, positon k) {
	bool find = true;
	for (int i = -2; i < 2; i++) {
		for (int j = -2; j < 6; j++) {
			if (move > 0 && ((r == ((rows / 2) + 8) + 5 + i) && (c == (cols / 2) + 8 * 10 - 3 + j))) {
				highlight(BOARD, desti, dimensions, turn, p, -37, -37, castle, true);
				highlightchechk(BOARD, dimensions, turn, p, -37, -37, k, true);
				PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				if (find) {
					find = false;
					smove++;
					if (move > 0) {
						move--;
					}
				}
				for (int i = 0; i < dimensions; i++) {
					for (int j = 0; j < dimensions; j++) {
						BOARD[i][j] = Bhistory[move][i][j];
					}
				}
				next_turn(turn);
				if (BOARD[0][0].cast != true) {
					BOARD[0][0].desti = Bhistory[move][0][0].desti;
					BOARD[0][0].ori = Bhistory[move][0][0].ori;
				}
				if (pieces[move + 1].killed != -1) {
					killo[(turn + 1) % 2].c--;
					if (killo[(turn + 1) % 2].c == 0 && killo[(turn + 1) % 2].r == 1) {
						killo[(turn + 1) % 2].r = 0;
						killo[(turn + 1) % 2].c = 7;
					}
					SetClr(8);
					if (pieces[move + 1].killed == 'P') {
						pawndisplay2(pieces[move + 1].kill.r, pieces[move + 1].kill);
					}
					else if (pieces[move + 1].killed == 'R') {
						rookdisplay2(pieces[move + 1].kill.r, pieces[move + 1].kill);
					}
					else if (pieces[move + 1].killed == 'B') {
						bishopdisplay2(pieces[move + 1].kill.r, pieces[move + 1].kill);
					}
					else if (pieces[move + 1].killed == 'Q') {
						queendisplay2(pieces[move + 1].kill.r, pieces[move + 1].kill);
					}
					else if (pieces[move + 1].killed == 'H') {
						knightdisplay2(pieces[move + 1].kill.r, pieces[move + 1].kill);
					}
					SetClr(7);
				}
				guiremove(BOARD, Bhistory, dimensions, move+1, 0, 0);
				if (Bhistory[move + 1][0][0].cast == true) {
					onlyboard();
				}
				return;
			}
			if (smove > 0 && ((r == ((rows / 2) + 8) + i) && (c == (cols / 2) + 8 * 10 - 3 + j))) {
				PlaySound(TEXT("BSCS23070-Chess-Phase-3-click.wav"), NULL, SND_FILENAME | SND_ASYNC);
				if (find) {
					find = false;
					move++;
				}
				if (smove > 0) {
					smove--;
				}

				for (int i = 0; i < dimensions; i++) {
					for (int j = 0; j < dimensions; j++) {
						BOARD[i][j] = Bhistory[move][i][j];
					}
				}
				next_turn(turn);
				if (pieces[move].killed != -1) {
					killo[(turn + 1) % 2].c++;
					if (killo[(turn + 1) % 2].c == 7 && killo[(turn + 1) % 2].r == 0) {
						killo[(turn + 1) % 2].r = 1;
						killo[(turn + 1) % 2].c = 0;
					}
					SetClr(p[(turn + 1) % 2].color);
					if (pieces[move].killed == 'P') {
						pawndisplay2(pieces[move].kill.r, pieces[move].kill);
					}
					else if (pieces[move + 1].killed == 'R') {
						rookdisplay2(pieces[move].kill.r, pieces[move].kill);
					}
					else if (pieces[move + 1].killed == 'B') {
						bishopdisplay2(pieces[move].kill.r, pieces[move].kill);
					}
					else if (pieces[move + 1].killed == 'Q') {
						queendisplay2(pieces[move].kill.r, pieces[move].kill);
					}
					else if (pieces[move + 1].killed == 'H') {
						knightdisplay2(pieces[move].kill.r, pieces[move].kill);
					}
					SetClr(7);
				}
				BOARD[0][0].desti = Bhistory[move][0][0].desti;
				BOARD[0][0].ori = Bhistory[move][0][0].ori;
				if (Bhistory[move][0][0].cast == true) {
					onlyboard();
				}
				guiremove(BOARD, Bhistory, dimensions, move, 0, 0);
				return;
			}
		}
	}
}
bool valid(chess_piece** BOARD, positon& ori, int dimensions) {
	int r = ori.r;
	int c = ori.c;
	for (int i = r - 4; i < 4 + r; i++) {
		for (int j = c - 5; j < c + 4; j++) {
			ori.r = i;
			ori.c = j;
			if ((notout(BOARD, dimensions, ori))) {
				return false;
			}
		}
	}
	return true;
}
bool saveclick(positon ori) {
	for (int i = -2; i < 2; i++) {
		for (int j = -2; j < 3; j++) {
			if ((ori.r == ((rows / 2) + 8) + 10 + i) && (ori.c == (cols / 2) + (8 * 10) - 3 + j)) {
				return true;
			}
		}
	}
	return false;
}
bool escape(positon ori) {
	for (int i = -2; i < 2; i++) {
		for (int j = -2; j < 3; j++) {
			if ((ori.r == ((rows / 2) + 8) + 15 + i) && (ori.c == (cols / 2) + (8 * 10) - 3 + j)) {
				return true;
			}
		}
	}
	return false;
}
bool resign(positon ori) {
	for (int i = -2; i < 4; i++) {
		for (int j = 0; j < 12; j++) {
			if ((ori.r == ((rows/2) + 2) + i) && (ori.c == j))
			return true;
		}
	}
	return false;
}
void savetofile(chess_piece** BOARD, int dimensions, int turn, players* p, move2 castle);
void source(chess_piece**& BOARD, positon& ori, int dimensions, string move, int& n, chess_piece***& Bhistory
	, int& turn, int& move1, int& smove, players* p,kill *pieces,positon killo[],positon desti,move2 castle
,positon k) {
	int r = 0, c = 0;
	char a = 0;
	do {

		/*gotoRowCol(0, 0);
		cout << "                                          ";
		gotoRowCol(0 , 0);
		cout << "ENTER THE POSITON TO MOVE " << move << ": ";
		cin >> a >> ori.r;
		chnagenum(ori.r);
		ori.c = replacechar(a, ori.c);
		int r = 0, c = 0;
		r = BOARD[ori.r][ori.c].piece.r;
		c = BOARD[ori.r][ori.c].piece.c;
			ori.r = r;
			ori.c = c;
			n++;*/

		int r = 0, c = 0;
		do {
			getRowColbyLeftClick(ori.r, ori.c);
			r = ori.r; c = ori.c;
			undoredo(BOARD, Bhistory, turn, dimensions, move1, smove, r, c,pieces,p,killo,desti,castle,k);
			displayboard(BOARD, dimensions, turn, p);
			if (saveclick(ori)) {
				savetofile(BOARD, dimensions, turn, p, castle);
			}
			else if (escape(ori)) {
				system("cls");
				gameexit();
				_getch();
				exit(1);
			}
			else if (resign(ori)) {
				system("cls");
				gotoRowCol(rows, cols);
				cout << p[turn].names << " has resigned the match..";
				gotoRowCol(rows+1, cols-10);
				cout << " ______   _______   ______  _  _______  _______  _______  ______  ";

				gotoRowCol(rows+2, cols-10);
				cout << "(_____ \\ (_______) / _____)| |(_______)(_______)(_______)(______) ";

				gotoRowCol(rows+3, cols - 10);
				cout << " _____) ) _____   ( (____  | | _   ___  _     _  _____    _     _ ";

				gotoRowCol(rows + 4, cols - 10);
				cout << "|  __  / |  ___)   \\____ \\ | || | (_  || |   | ||  ___)  | |   | |";

				gotoRowCol(rows+5, cols - 10);
				cout << "| |  \\ \\ | |_____  _____) )| || |___) || |   | || |_____ | |__/ / ";

				gotoRowCol(rows+6, cols - 10);
				cout << "|_|   |_|_______)(______/ |_| \\_____/ |_|   |_||_______)|_____/  \n";
				_getch();
				exit(1);
			}
		} while (valid(BOARD, ori, dimensions));
		return;
	} while (notout(BOARD, dimensions, ori));

}

bool notopponent(chess_piece** BOARD, positon ori, int turn, players* p, int dimensions, bool chehck) {
	int r = 0, c = 0;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((ori.r == BOARD[i][j].piece.r) && (ori.c == BOARD[i][j].piece.c)) {
				r = i;
				c = j;
				break;
			}
		}
	}
	if (chehck) {
		if ((BOARD[r][c].clr != (p[turn].color)) || BOARD[r][c].SYM == '-') {
			return false;
		}
		return true;
	}
	if ((BOARD[r][c].clr != (p[turn].color)) || BOARD[r][c].SYM == '-') {
		return true;
	}
	if (!chehck) {
		BOARD[r][c].clr = YELLOW;
	}
	return false;
}
bool notopponent_B(chess_piece** BOARD, positon d, int turn, players* p, int dimensions, positon ori, bool castlech) {
	int r = 0, c = 0, r2 = 0, c2 = 0;

	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((d.r == BOARD[i][j].piece.r) && (d.c == BOARD[i][j].piece.c)) {
				r = i;
				c = j;
			}
			if ((ori.r == BOARD[i][j].piece.r) && (ori.c == BOARD[i][j].piece.c)) {
				r2 = i;
				c2 = j;
			}
		}
	}
	if (!castlech) {
		BOARD[r2][c2].clr = p[turn].color;
		return false;
	}
	if (BOARD[r][c].clr != YELLOW) {
		BOARD[r2][c2].clr = p[turn].color;
		if ((BOARD[r][c].clr != p[turn].color) || BOARD[r][c].SYM == '-') {
			BOARD[r2][c2].clr = p[turn].color;
			return false;
		}
	}
	BOARD[r2][c2].clr = p[turn].color;
	return true;
}

void killdisplay(chess_piece ** B,int dimensions,int r1,int c1,positon killo[], int turn
,players *p,kill * &pieces,int move) {
	move++;
	char sym = -37;
	next_turn(turn);
	if (B[r1][c1].clr==p[turn].color && turn==0) {
		PlaySound(TEXT("BSCS23070-Chess-Phase-3-capture.wav"), NULL, SND_FILENAME | SND_ASYNC);
		pieces[move].color = p[turn].color;
		pieces[move].kill.r = ((rows / 2) - 12 + 8 * 6) + 4 + 6 * killo[0].r;
		pieces[move].kill.c = killo[0].c;
		if (B[r1][c1].SYM=='P') {
			SetClr(p[turn].color);
			pieces[move].killed = 'P';
			pawndisplay2(((rows / 2) - 12 + 8 * 6) + 4 + 6 * killo[0].r, killo[0]);
		}
		else if (B[r1][c1].SYM=='Q') {
			SetClr(p[turn].color);
			pieces[move].killed = 'Q';
			queendisplay2(((rows / 2) - 12 + 8 * 6) + 4 + 6 * killo[0].r, killo[0]);
		}
		else if (B[r1][c1].SYM == 'B') {
			SetClr(p[turn].color);
			pieces[move].killed = 'B';
			bishopdisplay2(((rows / 2) - 12 + 8 * 6) + 4 + 6 * killo[0].r, killo[0]);
		}
		else if (B[r1][c1].SYM == 'H') {
			SetClr(p[turn].color);
			pieces[move].killed = 'H';
			knightdisplay2(((rows / 2) - 12 + 8 * 6) + 4 + 6 * killo[0].r, killo[0]);
		}
		else if (B[r1][c1].SYM == 'R') {
			SetClr(p[turn].color);
			pieces[move].killed = 'R';
			rookdisplay2(((rows / 2) - 12 + 8 * 6) + 4 + 6 * killo[0].r, killo[0]);
		}
		++killo[0].c;
		if (killo[1].c == 6) {
			killo[1].c = killo[1].c - 0.5;
		}
		else if (killo[0].c == 7) {
			killo[0].r += 1;
			killo[0].c = 0;
		}
	}
	else if (B[r1][c1].clr == p[turn].color && turn == 1) {
		PlaySound(TEXT("BSCS23070-Chess-Phase-3-capture.wav"), NULL, SND_FILENAME | SND_ASYNC);
		pieces[move].color = p[turn].color;
		pieces[move].kill.r = ((rows / 2) - 14) + 4 + 6 * killo[1].r;
		pieces[move].kill.c = killo[1].c;
		if (B[r1][c1].SYM == 'P') {
			SetClr(p[turn].color);
			pieces[move].killed = 'P';
			pawndisplay2(((rows / 2) - 14) + 4 + 6 * killo[1].r, killo[1]);
		}
		else if (B[r1][c1].SYM == 'Q') {
			SetClr(p[turn].color);
			pieces[move].killed = 'Q';
			queendisplay2(((rows / 2) - 14) + 4 + 6 * killo[1].r, killo[1]);
		}
		else if (B[r1][c1].SYM == 'B') {
			SetClr(p[turn].color);
			pieces[move].killed = 'B';
			bishopdisplay2(((rows / 2) - 14) + 4 + 6 * killo[1].r, killo[1]);
		}
		else if (B[r1][c1].SYM == 'H') {
			SetClr(p[turn].color);
			pieces[move].killed = 'H';
			knightdisplay2(((rows / 2) - 14) + 4 + 6 * killo[1].r, killo[1]);
		}
		else if (B[r1][c1].SYM == 'R') {
			SetClr(p[turn].color);
			pieces[move].killed = 'R';
			rookdisplay2(((rows / 2) - 14) + 4 + 6 * killo[1].r, killo[1]);
		}
		++killo[1].c;
		if (killo[1].c == 6) {
			killo[1].c=killo[1].c-0.5;
		}
		else if (killo[1].c == 7) {
			killo[1].r += 1;
			killo[1].c = 0;
		}
	}
	else {
		pieces[move].color = -1;
		pieces[move].kill.r = -1;
		pieces[move].kill.c = -1;
		pieces[move].killed = -1;
	}
	kill* temp{};
	temp = new kill[move+3]{};
	for (int i = 0; i < move + 3; i++) {
	    temp[i] = pieces[i];
	}
	pieces = new kill[move + 4];
	for (int i = 0; i < move + 3; i++) {
		pieces[i]=temp[i];
	}
	SetClr(7);
}
void updateboard(chess_piece**& BOARD, positon& desti, positon& ori, int dimensions, players*& play, int turn
	, bool& castlech, move2 castle, int& move,positon kill2[],kill * pieces) {
	int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
	convert(BOARD, dimensions, ori, r2, c2);
	convert(BOARD, dimensions, desti, r1, c1);
	killdisplay(BOARD,dimensions,r1,c1,kill2,turn,play,pieces,move);
	if (BOARD[r2][c2].SYM == 'K' && ((r2 == 0 || r2 == 7) && (c2 == 4))) {
		switch (turn) {
		case 0:
			castle.king0++;
			break;
		case 1:
			castle.king1++;
		}
	}
	if (BOARD[r2][c2].SYM == 'P' && ((r2 == 0 || r2 == 7) && (c2 == 0 || c2 == 7))) {
		switch (turn) {
		case 0:
			if (c2 == 0) {
				castle.rook0[0]++;
			}
			else if (c2 == 7) {
				castle.rook0[1]++;
			}
		case 1:
			if (c2 == 0) {
				castle.rook1[0]++;
			}
			else if (c2 == 7) {
				castle.rook1[1]++;
			}
		}
	}
	char sym = -37;
	for (int p = BOARD[r2][c2].piece.r - 2; p < BOARD[r2][c2].piece.r + 6; p++) {
		for (int o = BOARD[r2][c2].piece.c - 2; o < BOARD[r2][c2].piece.c + 6; o++)
		{
			if (((r2 + 1) + (c2 + 1)) % 2 == 0) {
				SetClr(7);
			}
			else {
				SetClr(0);
			}
			gotoRowCol(p, o);
			cout << sym;
			SetClr(7);
		}
	}
	for (int p = BOARD[r1][c1].piece.r - 2; p < BOARD[r1][c1].piece.r + 6; p++) {
		for (int o = BOARD[r1][c1].piece.c - 2; o < BOARD[r1][c1].piece.c + 6; o++)
		{
			if (((r1 + 1) + (c1 + 1)) % 2 == 0) {
				SetClr(7);
			}
			else {
				SetClr(0);
			}
			gotoRowCol(p, o);
			cout << sym;
			SetClr(7);
		}
	}
	BOARD[r2][c2].clr = play[turn].color;
	if (castlech) {
		BOARD[r1][c1].SYM = BOARD[r2][c2].SYM;
		BOARD[r2][c2].SYM = '_';
		if (BOARD[r2][c2].SYM == '_') {
			BOARD[r1][c1].clr = 7;
		}
		swap(BOARD[r1][c1].clr, BOARD[r2][c2].clr);
		play[turn].moves++;
	}
	else {
		PlaySound(TEXT("BSCS23070-Chess-Phase-3-castle.wav"), NULL, SND_FILENAME | SND_ASYNC);
		if (c1 == 0) {
			BOARD[r2][c2 - 2].SYM = BOARD[r2][c2].SYM;
			BOARD[r1][c1 + 3].SYM = BOARD[r1][c1].SYM;
			BOARD[r2][c2 - 2].clr = BOARD[r2][c2].clr;
			BOARD[r1][c1 + 3].clr = BOARD[r1][c1].clr;
		}
		else if (c1 == 7) {
			BOARD[r2][c2 + 2].SYM = BOARD[r2][c2].SYM;
			BOARD[r1][c1 - 2].SYM = BOARD[r1][c1].SYM;
			BOARD[r2][c2 + 2].clr = BOARD[r2][c2].clr;
			BOARD[r1][c1 - 2].clr = BOARD[r1][c1].clr;
		}
		BOARD[r1][c1].SYM = '_';
		BOARD[r2][c2].SYM = '_';
		BOARD[r1][c1].clr = 7;
		BOARD[r2][c2].clr = 7;
		castlech = true;
		BOARD[0][0].cast = true;
	}
	move++;
	if (turn == 0) {
		play[0].moves++;
	}
	else {
		play[1].moves++;
	}
}

bool isverticalmove(positon ori, positon desti) {
	return desti.c == ori.c;
}
bool ishorizontalmove(positon ori, positon desti) {
	return desti.r == ori.r;
}
bool isdiagonalmove(positon ori, positon desti) {
	int desti1 = desti.r - ori.r;
	int ori1 = desti.c - ori.c;
	return (abs(desti1)) == (abs(ori1));
}
bool isemptyvertical(positon ori, positon desti, chess_piece** BOARD) {
	int direction = desti.r - ori.r;
	if (direction > 0) {
		for (int i = 1; i < direction; i++) {
			if (BOARD[ori.r + i][desti.c].SYM != '_') {
				return false;
			}
		}
		return true;
	}
	else {
		for (int i = 1; i < abs(direction); i++) {
			if (BOARD[ori.r - i][desti.c].SYM != '_') {
				return false;
			}
		}
		return true;
	}
	return false;
}
bool isemptyhorizontal(positon ori, positon desti, chess_piece** BOARD) {
	int direction = desti.c - ori.c;
	if (direction > 0) {
		for (int i = 1; i < direction; i++) {
			if (BOARD[ori.r][ori.c + i].SYM != '_') {
				return false;
			}
		}
		return true;
	}
	else {
		for (int i = 1; i < abs(direction); i++) {
			if (BOARD[ori.r][ori.c - i].SYM != '_') {
				return false;
			}
		}
		return true;
	}
	return false;
}
bool isemptydiagonal(positon ori, positon desti, chess_piece** BOARD) {
	int move_r = desti.r - ori.r;
	int move_c = desti.c - ori.c;
	int o = 1;
	if (abs(move_r) == abs(move_c)) {
		for (int i = 1, j = 1; i < abs(move_r) && j < abs(move_c); i++, j++) {
			if ((move_r < 0 && move_c > 0) && BOARD[ori.r - o][ori.c + o].SYM != '_') {
				return false;
			}
			else if ((move_r > 0 && move_c > 0) && BOARD[ori.r + o][ori.c + o].SYM != '_') {
				return false;
			}
			else if ((move_r < 0 && move_c < 0) && BOARD[ori.r - o][ori.c - o].SYM != '_') {
				return false;
			}
			else if ((move_r > 0 && move_c < 0) && BOARD[ori.r + o][ori.c - o].SYM != '_') {
				return false;
			}
			o++;
		}
		if (BOARD[ori.r][ori.c].clr == BOARD[desti.r][desti.c].clr && abs(move_r) == 1 && abs(move_c) == 1) {
			return false;
		}
		return true;
	}
	return false;
}
void fakeboard(chess_piece**& BOARDCOPY, chess_piece** BOARD, positon desti, positon ori, int dimensions, players* p, int turn) {
	BOARDCOPY = new chess_piece * [dimensions] {};
	for (int i = 0; i < dimensions; i++) {
		BOARDCOPY[i] = new chess_piece[dimensions]{};
	}
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			BOARDCOPY[i][j] = BOARD[i][j];
		}
	}
	int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((desti.r == BOARDCOPY[i][j].piece.r) && (desti.c == BOARDCOPY[i][j].piece.c)) {
				r1 = i;
				c1 = j;
			}
			if ((ori.r == BOARDCOPY[i][j].piece.r) && (ori.c == BOARDCOPY[i][j].piece.c)) {
				r2 = i;
				c2 = j;
			}
		}
	}
	BOARDCOPY[r2][c2].clr = p[turn].color;
	BOARDCOPY[r1][c1].SYM = BOARDCOPY[r2][c2].SYM;
	BOARDCOPY[r2][c2].SYM = '_';
	if (BOARDCOPY[r2][c2].SYM == '_') {
		BOARDCOPY[r1][c1].clr = 7;
	}
	swap(BOARDCOPY[r1][c1].clr, BOARDCOPY[r2][c2].clr);
}
bool chehck(chess_piece** B, int turn, int dimensions, players* p, bool mate, move2 castle) {
	next_turn(turn);
	positon destination = kingsearch(B, dimensions, turn, p, mate);
	next_turn(turn);
	positon s = {};
	//cout << B[destination.r][destination.c].SYM;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			//cout << i<<" "<< j<<" ";
			s.r = B[i][j].piece.r;
			s.c = B[i][j].piece.c;
			if (notopponent(B, s, turn, p, dimensions, true) && validmove(destination, s, turn, B, p, dimensions
				, castle)) {
				return true;
			}
		}
	}
	return false;
}
bool castlesearch(chess_piece** BOARD, int turn, players* p,
	int dimensions, positon ori, positon desti, move2 castle) {
	positon f{}, k{};
	chess_piece** FAKEBOARD{};
	int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
	int c = c2 - c1;
	convert(BOARD, dimensions, ori, r1, c1);
	convert(BOARD, dimensions, desti, r2, c2);
	f.r = r1; f.c = c1; k.r = r2; k.c = c2;
	if (ishorizontalmove(f, k) && (isemptyhorizontal(f, k, BOARD))) {

		for (int i = 0; i < abs(c); i++) {
			if (c > 0) {
				desti.c = BOARD[r1][c1 + i].piece.c;
				fakeboard(FAKEBOARD, BOARD, desti, ori, dimensions, p, turn);
				if (chehck(FAKEBOARD, turn, dimensions, p, false, castle)) {
					return false;
				}
			}
			else {
				desti.c = BOARD[r1][c1 - i].piece.c;
				fakeboard(FAKEBOARD, BOARD, desti, ori, dimensions, p, turn);
				if (chehck(FAKEBOARD, turn, dimensions, p, false, castle)) {
					return false;
				}
			}
		}
		return true;
	}
	return false;
}
positon kingsearch(chess_piece** B, int dimensions, int turn, players* p, bool mate) {
	positon k = {};
	if (mate) {
		next_turn(turn);
	}
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (B[i][j].clr == p[turn].color && B[i][j].SYM == 'K') {
				//cout << i <<" "<< j<<" ";
				k.r = B[i][j].piece.r;
				k.c = B[i][j].piece.c;
				return k;
			}
		}
	}
}
void selfchehckmsg(int i) {
	if (i == 0) {
		gotoRowCol(4, 0);
		cout << "           " << endl << "                                                       ";
		gotoRowCol(4, 0);
		cout << "CHECHK MSG:" << endl << "THIS MOVE IS ILLEGAL AS IT PUTS YOU IN SELF CHECHK....";
	}
	else {
		gotoRowCol(4, 0);
		cout << "           " << endl << "                                                       ";
	}
}

bool selfchehck(chess_piece** B, int turn, int dimensions, players* p, bool ch, move2 castle) {
	next_turn(turn);
	if (chehck(B, turn, dimensions, p, false, castle)) {
		if (!ch) {
			selfchehckmsg(0);
		}
		return true;
	}
	if (!ch) {
		selfchehckmsg(1);
	}
	return false;
}

bool castlechechk(move2 castle, chess_piece** BOARD, positon desti, int turn, players* p,
	int dimensions, positon ori) {
	int r1 = 0, c1 = 0, r2 = 0, c2 = 0, r3 = 0, c3 = 0;
	convert(BOARD, dimensions, ori, r1, c1);
	convert(BOARD, dimensions, desti, r2, c2);
	c3 = (c2 - c1);
	if ((c3 == (3) || c3 == -4) && castlesearch(BOARD, turn, p, dimensions, ori, desti, castle)) {
		if (BOARD[r1][c1].SYM == 'K' && BOARD[r2][c2].SYM == 'R' && BOARD[r2][c2].clr == p[turn].color)
		{
			if (turn == 0) {
				if ((castle.king0 == 0 && c1 == 4) && ((castle.rook0[0] == 0 && c2 == 0) ||
					(castle.rook0[1] == 0 && c2 == 7)) && r2 == 0) {
					return true;
				}
			}
			else if (turn == 1) {
				if ((castle.king1 == 0 && c1 == 4) && ((castle.rook1[0] == 0 && c2 == 0) ||
					(castle.rook1[1] == 0 && c2 == 7)) && r2 == 7) {
					return true;
				}
			}
		}
	}
	return false;
}
bool validmove(positon desti, positon ori, int turn, chess_piece** BOARD, players* p, int dimensions, move2 castle) {
	if (castlechechk(castle, BOARD, desti, turn, p, dimensions, ori)) {
		return true;
	}
	int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if ((desti.r == BOARD[i][j].piece.r) && (desti.c == BOARD[i][j].piece.c)) {
				r1 = i;
				c1 = j;
			}
			if ((ori.r == BOARD[i][j].piece.r) && (ori.c == BOARD[i][j].piece.c)) {
				r2 = i;
				c2 = j;
			}
		}
	}
	ori.r = r2;
	desti.r = r1;
	ori.c = c2;
	desti.c = c1;
	switch (BOARD[r2][c2].SYM) {

	case 'P':
		switch (turn)
		{
		case 1:
		{
			//ori.r = ori.r - 1;
			if (r2 == 6) {
				return (((((r1 - r2 == -1 && c1 - c2 == 1) || (r1 - r2 == -1 && c1 - c2 == -1) && (isemptydiagonal(ori, desti, BOARD)))
					&& (BOARD[r1][c1].clr != 7 && BOARD[r1][c1].clr != p[turn].color)))
					|| (isemptyvertical(ori, desti, BOARD) && isverticalmove(ori, desti) && ((r1 - r2 == -2) || (r1 - r2 == -1))
						&& BOARD[r1][c1].clr != p[(turn + 1) % 2].color));
			}
			else {
				return (((((r1 - r2 == -1 && c1 - c2 == 1) || (r1 - r2 == -1 && c1 - c2 == -1) && (isemptydiagonal(ori, desti, BOARD)))
					&& ((BOARD[r1][c1].clr != 7 && BOARD[r1][c1].clr != p[turn].color)))) ||
					(isemptyvertical(ori, desti, BOARD) && isverticalmove(ori, desti) && (r1 - r2 == -1) &&
						BOARD[r1][c1].clr != p[(turn + 1) % 2].color));
			}
		}
		break;
		case 0:
		{
			//ori.r = ori.r + 1;
			if (r2 == 1) {
				return (((((r1 - r2 == 1 && c1 - c2 == 1) || (r1 - r2 == 1 && c1 - c2 == -1)) && (isemptydiagonal(ori, desti, BOARD))
					&& (BOARD[r1][c1].clr != 7 && BOARD[r1][c1].clr != p[turn].color))) ||
					(isemptyvertical(ori, desti, BOARD) && isverticalmove(ori, desti) && ((r1 - r2 == 2) || (r1 - r2 == 1))
						&& BOARD[r1][c1].clr != p[(turn + 1) % 2].color));
			}
			else {
				return (((((r1 - r2 == 1 && c1 - c2 == 1) || (r1 - r2 == 1 && c1 - c2 == -1)) && (isemptydiagonal(ori, desti, BOARD))
					&& (BOARD[r1][c1].clr != 7 && BOARD[r1][c1].clr != p[turn].color)))
					|| (isemptyvertical(ori, desti, BOARD) && isverticalmove(ori, desti) && (r1 - r2 == 1)
						&& BOARD[r1][c1].clr != p[(turn + 1) % 2].color));
			}
		}
		break;
		}
		break;
	case 'Q':
		return ((isemptyvertical(ori, desti, BOARD) && isverticalmove(ori, desti)) || ((isemptyhorizontal(ori, desti, BOARD)
			&& ishorizontalmove(ori, desti))) || ((isemptydiagonal(ori, desti, BOARD) && (isdiagonalmove(ori, desti)))));
		break;
	case 'K':
	{
		int kr = (r1 - r2);
		int kc = (c1 - c2);
		return (((abs(kr) == 1) && (abs(kc) == 1)) || (abs(kc) == 1 && kr == 0) || (abs(kr) == 1 && kc == 0));
		/*return (((isemptyvertical(ori, desti, BOARD) && isverticalmove(ori, desti)) || (isemptyhorizontal(ori, desti, BOARD)
			&& ishorizontalmove(ori, desti)) || (isemptydiagonal(ori, desti, BOARD) && isdiagonalmove(ori, desti))) && ((r1 - r2) == 1 ||
				(c1 - c2 == 1) || (r1 - r2 == -1) || (c1 - c2 == -1)));*/
	}
	break;
	case 'B':
	{
		return((isemptydiagonal(ori, desti, BOARD) && isdiagonalmove(ori, desti)));
	}
	break;
	case 'R':
	{
		return ((isemptyhorizontal(ori, desti, BOARD) && ishorizontalmove(ori, desti)) || (isemptyvertical(ori, desti, BOARD)
			&& isverticalmove(ori, desti)));
	}
	break;
	case'H':
	{
		int hr = abs(r2 - r1);
		int hc = abs(c2 - c1);
		return ((hr == 2 && hc == 1) || (hr == 1 && hc == 2));
	}
	break;
	case '_':
		return false;
	}

}
bool** high(chess_piece** BOARD, positon ori, int turn, int dimensions, players* p, bool st, move2 castle
	, bool& ch, positon& cas) {
	bool** q;
	positon desti{};
	q = new bool* [dimensions] {};
	for (int i = 0; i < dimensions; i++) {
		q[i] = new bool [dimensions] {};
	}
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			desti.r = BOARD[i][j].piece.r;
			desti.c = BOARD[i][j].piece.c;
			if (castlechechk(castle, BOARD, desti, turn, p, dimensions, ori)) {
				int r1 = 0, c1 = 0, c2 = 0, r2 = 0;
				convert(BOARD, dimensions, ori, r1, c1);
				convert(BOARD, dimensions, desti, r2, c2);
				int c = c2 - c1;
				int o = 0;
				ch = true;
				while (o <= abs(c)) {
					if (c < 0) {
						q[i][c1 - o] = true;
					}
					if (c > 0) {
						q[i][c1 + o] = true;
					}
					if (BOARD[i][j].SYM == 'R') {
						cas.r = i;
						cas.c = j;
					}
					o++;
					j++;
				}

				continue;
			}
			if (ori.r == desti.r && ori.c == desti.c) {
				q[i][j] = false;
				continue;
			}
			if (st) {
				q[i][j] = (!notopponent_B(BOARD, desti, turn, p, dimensions, ori, true) &&
					validmove(desti, ori, turn, BOARD, p, dimensions, castle));
			}
			else {
				q[i][j] = (validmove(desti, ori, turn, BOARD, p, dimensions, castle));
			}
			if (q[i][j]) {
				chess_piece** BOARDCOPY;
				fakeboard(BOARDCOPY, BOARD, desti, ori, dimensions, p, turn);
				bool temp = !(selfchehck(BOARDCOPY, turn, dimensions, p, true, castle));
				q[i][j] = temp;
			}
		}
	}
	return q;
}
void highlight(chess_piece** BOARD, positon ori, int dimensions, int turn, players* po, char sym1, char sym2, move2
	castle, bool un) {
	positon cas{};
	bool ch = false;
	bool** posibo = high(BOARD, ori, turn, dimensions, po, false, castle, ch, cas);
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (posibo[i][j] == true) {
				for (int p = BOARD[i][j].piece.r - 2; p < BOARD[i][j].piece.r + 6; p++) {
					if ((BOARD[i][j].clr == po[(turn+1)%2].color)) {
						if (((i + 1) + (j + 1)) % 2 == 0) {
							if (!un) {
								SetClr(244);
							}
							else {
								SetClr(7);
							}
						}
						else {
							if (!un) {
								SetClr(4);
							}
							else {
								SetClr(0);
							}
						}
						gotoRowCol(p, (BOARD[i][j].piece.c) - 2);
						cout << char(sym2);
						gotoRowCol(p, (BOARD[i][j].piece.c) + 5);
						cout << char(sym2);
						SetClr(7);
					}
					else if ((BOARD[i][j].clr != po[turn].color) || (BOARD[i][j].piece.r == i && BOARD[i][j].piece.c == j)) {
						if (((i + 1) + (j + 1)) % 2 == 0) {
							if (!un) {
								SetClr(250);
							}
							else {
								SetClr(7);
							}
						}
						else {
							if (!un) {
								SetClr(10);
							}
							else {
								SetClr(0);
							}
						}
						gotoRowCol(p, (BOARD[i][j].piece.c) - 2);
						cout << sym2;
						gotoRowCol(p, (BOARD[i][j].piece.c) + 5);
						cout << sym2;
						SetClr(7);
					}
					else if (ch && (cas.r == i && cas.c == j)) {
						if (((i + 1) + (j + 1)) % 2 == 0) {
							if (!un) {
								SetClr(250);
							}
							else {
								SetClr(7);
							}
						}
						else {
							if (!un) {
								SetClr(10);
							}
							else {
								SetClr(0);
							}
						}
						gotoRowCol(p, (BOARD[i][j].piece.c) - 2);
						cout << sym2;
						gotoRowCol(p, (BOARD[i][j].piece.c) + 5);
						cout << sym2;
						SetClr(7);
					}
				}
				for (int p = BOARD[i][j].piece.c - 1; p < BOARD[i][j].piece.c + 5; p++) {
					if (BOARD[i][j].clr == po[(turn + 1) % 2].color) {
							if (((i + 1) + (j + 1)) % 2 == 0) {
								if (!un) {
									SetClr(244);
								}
								else {
									SetClr(7);
								}
							}
							else {
								if (!un) {
									SetClr(4);
								}
								else {
									SetClr(0);
								}
							}
							gotoRowCol((BOARD[i][j].piece.r) - 2, p);
							cout << char(sym1);
							gotoRowCol((BOARD[i][j].piece.r) + 5, p);
							cout << char(sym1);
							SetClr(7);

					}
					else if (BOARD[i][j].clr != po[turn].color || (BOARD[i][j].piece.r == i && BOARD[i][j].piece.c == j)) {
						if (((i + 1) + (j + 1)) % 2 == 0) {
							if (!un) {
								SetClr(250);
							}
							else {
								SetClr(7);
							}
						}
						else {
							if (!un) {
								SetClr(10);
							}
							else {
								SetClr(0);
							}
						}
						gotoRowCol((BOARD[i][j].piece.r) - 2, p);
						cout << sym1;
						gotoRowCol((BOARD[i][j].piece.r) + 5, p);
						cout << sym1;
						SetClr(7);
					}
					else if (ch && cas.r == i && cas.c == j) {
						if (((i + 1) + (j + 1)) % 2 == 0) {
							if (!un) {
								SetClr(250);
							}
							else {
								SetClr(7);
							}
						}
						else {
							if (!un) {
								SetClr(10);
							}
							else {
								SetClr(0);
							}
						}
						gotoRowCol((BOARD[i][j].piece.r) - 2, p);
						cout << sym1;
						gotoRowCol((BOARD[i][j].piece.r) + 5, p);
						cout << sym1;
						SetClr(7);
					}
				}

			}
		}
	}
}
void onlyboard() {
	int o = 0;
	int l = 0;
	char sym = -37;
	for (int i = (rows / 2) - 14; i < (rows / 2) - 12 + 8 * 8; i += 8) {
		if (l >= 64) {
			continue;
		}
		o = 0;
		for (int j = (cols / 2) - 13; j < (cols / 2) - 12 + (8 * 8); j += 8) {
			if (l % 2 == 0 && o < 8) {
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						SetClr(7);
						gotoRowCol(i + k, j + l);
						cout << sym;
						SetClr(7);
					}
				}
			}
			else {
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						SetClr(0);
						gotoRowCol(i + k, j + l);
						cout << sym;
						SetClr(7);
					}
				}
			}
			o++;
			l++;
		}
	}
	for (int i = (rows / 2) - 15; i <= (rows / 2) - 14 + 8 * 8; i++) {
		SetClr(11);
		gotoRowCol(i, (cols / 2) - 17 + (8 * 8) + 4);
		cout << sym;
		SetClr(7);
	}
}
void boardmaking() {
	int l = 0;
	char sym = -37;
	system("cls");
	SetClr(11);
	for (int i = ((rows / 2) + 8) - 3; i < ((rows / 2) + 8) + 20; i++) {
		gotoRowCol(i, (cols / 2) + 8 * 10 - 8);
		cout << sym;
		gotoRowCol(i, (cols / 2) + 8 * 10 + 5);
		cout << sym;
	}
	for (int i = (cols / 2) + 8 * 10 - 8; i <= (cols / 2) + 8 * 10 + 5; i++) {
		gotoRowCol((rows / 2) + 8 - 3, i);
		cout << sym;
		gotoRowCol(((rows / 2) + 8) + 20, i);
		cout << sym;
	}
	SetClr(14);
	for (int i = ((rows / 2) + 8) - 2; i < ((rows / 2) + 8) + 20; i++) {
		for (int j = (cols / 2) + 8 * 10 - 7; j <= (cols / 2) + 8 * 10 + 4; j++) {
			gotoRowCol(i, j);
			cout << sym;
		}
	}
	SetClr(232);
	gotoRowCol(((rows/2) + 8)+5, (cols/2)+8*10 - 3);
	cout << "UNDO";
	gotoRowCol(((rows / 2) + 8) , (cols / 2) + 8 * 10 - 3);
	cout << "REDO";
	gotoRowCol(((rows / 2) + 8) +10, (cols / 2) + 8 * 10 - 3);
	cout << "SAVE";
	gotoRowCol(((rows / 2) + 8) + 15, (cols / 2) + 8 * 10 - 3);
	cout << "EXIT";
	SetClr(224);
	gotoRowCol(((rows / 2) + 8) + 4, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8) + 14, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8) + 16, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8) + 6, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8) + 9, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8) + 11, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8 + 15), (cols / 2) + 8 * 10 - 5);
	cout << "|";
	gotoRowCol(((rows / 2) + 8 + 15), (cols / 2) + (8 * 10)+2);
	cout << "|";
	gotoRowCol(((rows / 2) + 8 + 5), (cols / 2) + 8 * 10 - 5);
	cout << "|";
	gotoRowCol(((rows / 2) + 8 + 5), (cols / 2) + (8 * 10) + 2);
	cout << "|";
	gotoRowCol(((rows / 2) + 8 + 10), (cols / 2) + 8 * 10 - 5);
	cout << "|";
	gotoRowCol(((rows / 2) + 8 + 10), (cols / 2) + (8 * 10) + 2);
	cout << "|";
	gotoRowCol(((rows / 2) + 8) - 1, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8) + 1, (cols / 2) + 8 * 10 - 5);
	cout << "--------";
	gotoRowCol(((rows / 2) + 8), (cols / 2) + 8 * 10 - 5);
	cout << "|";
	gotoRowCol(((rows / 2) + 8), (cols / 2) + (8 * 10) + 2);
	cout << "|";
	SetClr(7);
	for (int i = (cols / 2) - 13; i <= (cols / 2) - 18 + (8 * 8) + 4; i++) {
		gotoRowCol((rows / 2) - 15, i);
		SetClr(11);
		cout << sym;
	}
	for (int i = (cols / 2) - 14; i <= (cols / 2) - 18 + (8 * 8) + 4; i++) {
		SetClr(11);
		gotoRowCol((rows / 2) - 14 + (8 * 8), i);
		cout << sym;
		SetClr(7);
	}
	for (int i = (rows / 2) - 15; i < (rows / 2) - 14 + 8 * 8; i++) {
		SetClr(11);
		gotoRowCol(i, (cols / 2) - 14);
		cout << sym;
		SetClr(7);
	}
	for (int i = (rows / 2) - 15; i <= (rows / 2) - 14 + 8 * 8; i++) {
		SetClr(11);
		gotoRowCol(i, (cols / 2) - 17 + (8 * 8) + 4);
		cout << sym;
		SetClr(7);
	}
	int o = 0;
	for (int i = (rows / 2) - 14; i < (rows / 2) - 12 + 8 * 8; i += 8) {
		if (l >= 64) {
			continue;
		}
		o = 0;
		for (int j = (cols / 2) - 13; j < (cols / 2) - 12 + (8 * 8); j += 8) {
			if (l % 2 == 0 && o < 8) {
				for (int k = 0; k < 8; k++) {
					for (int l = 0; l < 8; l++) {
						SetClr(7);
						gotoRowCol(i + k, j + l);
						cout << sym;
						SetClr(7);
					}
				}
			}
			o++;
			l++;
		}
	}
	for (int i = (rows / 2) - 12 + 8 * 6; i <= (rows / 2) - 12 + 8 * 8; i++) {
		SetClr(11);
		gotoRowCol(i, 3 * cols / 2 - (8 * 2));
		cout << sym;
		gotoRowCol(i, cols + (8 * 10) + 3);
		cout << sym;
	}
	for (int i = (rows / 2) - 14; i <= (rows / 2) - 14 + 8 * 2; i++) {
		gotoRowCol(i, 3 * cols / 2 - (8 * 2));
		cout << sym;
		gotoRowCol(i, cols + (8 * 10) + 3);
		cout << sym;
	}
	for (int i = (3 * cols / 2 - 8 * 2); i < cols + (8 * 10) + 3; i++) {
		gotoRowCol((rows / 2) - 14, i);
		cout << sym;
		gotoRowCol((rows / 2) - 14 + 8 * 2, i);
		cout << sym;
	}
	for (int i = (3 * cols / 2 - 8 * 2); i < cols + (8 * 10) + 3; i++) {
		gotoRowCol((rows / 2) - 12 + 8 * 6, i);
		cout << sym;
		gotoRowCol((rows / 2) - 12 + 8 * 8, i);
		cout << sym;
	}
	for (int i = ((rows / 2) - 12 + 8 * 6) + 1; i < (rows / 2) - 12 + 8 * 8; i++) {
		for (int j = (3 * cols / 2 - 8 * 2) + 1; j < cols + (8 * 10) + 3; j++) {
			SetClr(8);
			gotoRowCol(i, j);
			cout << sym;
		}
	}
	for (int i = ((rows / 2) - 14) + 1; i < (rows / 2) - 14 + 8 * 2; i++) {
		for (int j = (3 * cols / 2 - (8 * 2)) + 1; j < cols + (8 * 10) + 3; j++) {
			gotoRowCol(i, j);
			cout << sym;
		}
	}
	SetClr(7);
	SetClr(14);
	for (int i = rows / 2; i < rows / 2 + 4; i++) {
		for (int j = 2; j < 12; j++) {
			gotoRowCol(i, j);
			cout << sym;
		}
	}
	gotoRowCol(rows / 2 + 2, 3);
	SetClr(244);
	cout << "RESIGN";
}

void chechkmsg(int i) {
	if (i == 0) {
		gotoRowCol(5, 0);
		cout << "               " << endl << "                                                 ";
		gotoRowCol(5, 0);
		cout << "CHECHK MSG:" << endl << "YOU ARE BEING CHEHCKED....";
	}
	else {
		gotoRowCol(5, 0);
		cout << "           " << endl << "                                                         ";
	}
}
void spi(chess_piece** B, int turn, int dimensions, players* p, positon*& mypiecepos) {
	mypiecepos = new positon[16]{};
	int ln = 0;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (B[i][j].clr == p[turn].color) {
				mypiecepos[ln].c = B[i][j].piece.c;
				mypiecepos[ln].r = B[i][j].piece.r;
				ln++;
			}
		}
	}
}
bool chechkmate2(chess_piece** B, int  turn, int  dimensions, players* p, move2 castle) {
	next_turn(turn);
	chess_piece** FB = {};
	positon* mypiecepos = {};
	spi(B, turn, dimensions, p, mypiecepos);
	positon s{};
	positon d{};
	for (int i = 0; i < 16; i++) {
		s.r = mypiecepos[i].r;
		s.c = mypiecepos[i].c;
		for (int j = 0; j < dimensions; j++) {
			for (int k = 0; k < dimensions; k++) {
				d.c = B[j][k].piece.c;
				d.r = B[j][k].piece.r;
				if (!notopponent_B(B, d, turn, p, dimensions, s, true) && validmove(d, s, turn, B, p, dimensions
					, castle)) {
					fakeboard(FB, B, d, s, dimensions, p, turn);
					if (!selfchehck(FB, turn, dimensions, p, true, castle)) {
						return false;
					}
				}
			}
		}
	}
	return true;
}
void chehckmatemsg(int turn, players* p) {
	gotoRowCol(5, 0);
	cout << "           " << endl << "                                                 ";
	gotoRowCol(5, 0);
	cout << "CHECHK MSG:" << endl << p[(turn + 1) % 2].names << " HAS BEEN CHEHCKMATED.....";
}
void chechkclearmsg(int i) {
	if (i == 0) {
		gotoRowCol(5, 0);
		cout << "           " << endl << "                                                 ";
		gotoRowCol(5, 0);
		cout << "CHECHK MSG:" << endl << "Move is illegal as you are still in chechk.....";
		return;
	}
	else {
		gotoRowCol(5, 0);
		cout << "           " << endl << "                                                 ";
	}
}
void highlightchechk(chess_piece** B, int dimensions, int turn, players* p, int sym1, int sym2, positon& k
	, bool un) {
	if (un) {
		next_turn(turn);
	}
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (B[i][j].SYM == 'K' && B[i][j].clr != p[turn].color) {
				k.r = i;
				k.c = j;
				for (int p = B[i][j].piece.r - 2; p < B[i][j].piece.r + 6; p++) {
						if (((i + 1) + (j + 1)) % 2 == 0) {
							if (!un) {
								SetClr(244);
							}
							else {
								SetClr(7);
							}
						}
						else {
							if (!un) {
								SetClr(4);
							}
							else {
								SetClr(0);
							}
						}
						gotoRowCol(p, (B[i][j].piece.c) - 2);
						cout << char(sym2);
						gotoRowCol(p, (B[i][j].piece.c) + 5);
						cout << char(sym2);
						SetClr(7);
					}
				for (int p = B[i][j].piece.c - 1; p < B[i][j].piece.c + 5; p++) {
						if (((i + 1) + (j + 1)) % 2 == 0) {
							if (!un) {
								SetClr(244);
							}
							else {
								SetClr(7);
							}
						}
						else {
							if (!un) {
								SetClr(4);
							}
							else {
								SetClr(0);
							}
						}
						gotoRowCol((B[i][j].piece.r) - 2, p);
						cout << char(sym1);
						gotoRowCol((B[i][j].piece.r) + 5, p);
						cout << char(sym1);
						SetClr(7);
				}
			}
		}
	}
}

void outofchechk(chess_piece** B, int  turn, int  dimensions, players* p, bool& chechk, move2 castle
	, positon k) {
	if (selfchehck(B, turn, dimensions, p, false, castle)) {
		chechkclearmsg(0);
	}
	else {
		chechkclearmsg(1);
		if (chechk) {
			highlightchechk(B, dimensions,turn, p,-37,-37,k, true);
		}
		chechk = false;

	}
}
void inchehck(chess_piece** B, int  turn, int  dimensions, players* p, bool& chechk, move2 castle,
	positon& k) {
	if (chehck(B, turn, dimensions, p, false, castle)) {
		highlightchechk(B, dimensions, turn, p, -37, -37, k,false);
		PlaySound(TEXT("BSCS23070-Chess-Phase-3-notify.wav"), NULL, SND_FILENAME | SND_ASYNC);
		chechkmsg(0);
		chechk = true;
	}
	else {
		chechkmsg(1);
	}
}
void chehckmate(chess_piece** B, int  turn, int  dimensions, players* p, bool& chechkmate, bool chehck, move2 castle) {
	if (chehck && chechkmate2(B, turn, dimensions, p, castle)) {
		chehckmatemsg(turn, p);
		PlaySound(TEXT("BSCS23070-Chess-Phase-3-end.wav"), NULL, SND_FILENAME | SND_ASYNC);
		chechkmate = true;
	}
}
void stalematemsg() {
	gotoRowCol(3, 0);
	cout << "DRAW MSG:" << endl << "The game has ended in a stalemate.....";
	return;
}
bool stalemate(chess_piece** B, int  turn, int  dimensions, players* p, move2 castle) {
	positon s{};
	bool** legalmoves{};
	next_turn(turn);
	bool ch = false;
	positon dumm{};
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (B[i][j].clr == p[turn].color) {
				s.r = B[i][j].piece.r;
				s.c = B[i][j].piece.c;
				if (notopponent(B, s, turn, p, dimensions, true)) {
					legalmoves = high(B, s, turn, dimensions, p, true, castle, ch, dumm);
					cout << endl;
				}
				for (int k = 0; k < dimensions; k++) {
					for (int o = 0; o < dimensions; o++) {
						if (legalmoves[k][o]) {
							return false;
						}
					}
				}
			}
		}
	}
	if (!selfchehck(B, turn, dimensions, p, true, castle)) {
		stalematemsg();
		return true;
	}
	return false;
}
void upperpromo(string sym1, char sym, string promo, positon promo2[], int color) {
	gotoRowCol((rows / 2) - 13, (cols / 2) - 27 + 8 * 1);
	cout << sym1;
	for (int i = (rows / 2) - 13; i < (rows / 2) - 13 + 8 * 4; i++) {
		for (int j = (cols / 2) - 30; j <= (cols / 2) - 30 + 8 * 1; j += 8) {
			gotoRowCol(i, j);
			cout << sym;
		}
	}
	int l = 0;
	for (int i = (rows / 2) - 13; i <= (rows / 2) - 13 + 8 * 4; i += 8) {
		SetClr(color);
		if (promo[l] == 'Q') {
			gotoRowCol(i - 6 - 1, (cols / 2) - 26);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 2);
			cout << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 + 2);
			cout << sym << endl;
			gotoRowCol(i - 6 + 4, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
		}
		else if (promo[l] == 'R') {
			gotoRowCol(i - 6, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 - 1);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 + 1);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
		}
		else if (promo[l] == 'B') {

			gotoRowCol(i - 6 - 1, (cols / 2) - 26);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 2);
			cout << sym << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 1);
			cout << sym << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 4, (cols / 2) - 26);
			cout << sym;
		}
		else if (promo[l] == 'H') {
			gotoRowCol(i - 6 - 1, (cols / 2) - 26);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 + 1);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 + 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 4, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << sym << endl;
		}
		promo2[l].r = i - 2;
		promo2[l].c = (cols / 2) - 27;
		l++;
		SetClr(7);
		for (int j = (cols / 2) - 30; j < (cols / 2) - 30 + 8 * 1; j++) {
			gotoRowCol(i, j);
			SetClr(7);
			cout << sym;
		}

	}
	cout << sym;
}
void lowerpromo(string sym1, char sym, string promo, positon promo2[], int color) {
	gotoRowCol((rows / 2) - 15 + 8 * 8, (cols / 2) - 27 + 8 * 1);
	cout << sym1;
	for (int i = (rows / 2) - 13 + 8 * 4; i < (rows / 2) - 13 + 8 * 8; i++) {
		for (int j = (cols / 2) - 30; j <= (cols / 2) - 30 + 8 * 1; j += 8) {
			gotoRowCol(i, j);
			cout << sym;
		}
	}
	int l = 0;
	for (int i = (rows / 2) - 13 + 8 * 4; i <= (rows / 2) - 13 + 8 * 8; i += 8) {
		SetClr(color);
		if (promo[l] == 'Q') {
			gotoRowCol(i - 6 - 1, (cols / 2) - 26);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 2);
			cout << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 + 2);
			cout << sym << endl;
			gotoRowCol(i - 6 + 4, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
		}
		else if (promo[l] == 'R') {
			gotoRowCol(i - 6, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 2);
			cout << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 - 1);
			cout << sym;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 + 1);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
		}
		else if (promo[l] == 'B') {

			gotoRowCol(i - 6 - 1, (cols / 2) - 26);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 2);
			cout << sym << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 1);
			cout << sym << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26);
			cout << sym;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << endl;
			gotoRowCol(i - 6 + 4, (cols / 2) - 26);
			cout << sym;
		}
		else if (promo[l] == 'H') {
			gotoRowCol(i - 6 - 1, (cols / 2) - 26);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 + 1);
			cout << sym << endl;
			gotoRowCol(i - 6, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 + 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 3, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 2, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 - 1);
			cout << sym << endl;
			gotoRowCol(i - 6 + 1, (cols / 2) - 26 + 2);
			cout << sym;
			gotoRowCol(i - 6 + 4, (cols / 2) - 26 - 2);
			cout << sym << sym << sym << sym << sym << sym << endl;
		}
		promo2[l].r = i - 2;
		promo2[l].c = (cols / 2) - 28;
		l++;
		SetClr(7);
		for (int j = (cols / 2) - 30; j < (cols / 2) - 30 + 8 * 1; j++) {
			gotoRowCol(i, j);
			SetClr(7);
			cout << sym;
		}

	}
	cout << sym;
}
bool pawnssearch(chess_piece** B, int turn, players* p, int dimensions
	, positon& pawn, int posi) {
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			if (B[i][j].SYM == 'P' && (i == posi)) {
				pawn.r = i;
				pawn.c = j;
				return true;
			}
		}
	}
	return false;
}
void selectupper(int turn, players* p, int dimensions, positon promo2[], string
	promo, chess_piece**& BOARD, positon pp) {
	int r = 0, c = 0, r2 = 0, c2 = 0;
	bool found = false;
	while (!found) {
		getRowColbyLeftClick(r, c);
		for (int i = r - 2; i < r + 3; i++) {
			for (int j = c - 2; j < c + 3; j++) {
				r2 = i;
				c2 = j;
				for (int k = 1; k < 6; k++) {
					if (promo2[k].r == r2 && promo2[k].c == c2) {
						BOARD[pp.r][pp.c].SYM = promo[k];
						found = true;
						break;
					}
				}
				if (found) {
					break;
				}
			}
			if (found) {
				break;
			}
		}
	}
}
void pawnpromotion(chess_piece** BOARD, int turn, players* p, int dimensions) {
	int r = 0, c = 0;
	string promo = " QRBH";
	positon promo2[6]{}, pp{};
	if (turn == 1 && pawnssearch(BOARD, turn, p, dimensions, pp, 0)) {
		upperpromo("<-", -37, promo, promo2, p[turn].color);
		selectupper(turn, p, dimensions, promo2, promo, BOARD, pp);
		PlaySound(TEXT("BSCS23070-Chess-Phase-3-promote.wav"), NULL, SND_FILENAME | SND_ASYNC);
		for (int p = BOARD[pp.r][pp.c].piece.r - 2; p < BOARD[pp.r][pp.c].piece.r + 6; p++) {
			for (int o = BOARD[pp.r][pp.c].piece.c - 2; o < BOARD[pp.r][pp.c].piece.c + 5; o++)
			{
				if (((pp.c + 1) + (pp.r + 1)) % 2 == 0) {
					SetClr(7);
				}
				else {
					SetClr(0);
				}
				gotoRowCol(p, o);
				cout << char(-37);
				SetClr(7);
			}
		}
		upperpromo("  ", ' ', promo, promo2, 7);

	}
	else if (turn == 0 && pawnssearch(BOARD, turn, p, dimensions, pp, 7)) {
		lowerpromo("<-", -37, promo, promo2, p[turn].color);
		selectupper(turn, p, dimensions, promo2, promo, BOARD, pp);
		PlaySound(TEXT("BSCS23070-Chess-Phase-3-promote.wav"), NULL, SND_FILENAME | SND_ASYNC);
		for (int p = BOARD[pp.r][pp.c].piece.r - 2; p < BOARD[pp.r][pp.c].piece.r + 6; p++) {
			for (int o = BOARD[pp.r][pp.c].piece.c - 2; o < BOARD[pp.r][pp.c].piece.c + 5; o++)
			{
				if (((pp.c + 1) + (pp.r + 1)) % 2 == 0) {
					SetClr(7);
				}
				else {
					SetClr(0);
				}
				gotoRowCol(p, o);
				cout << char(-37);
				SetClr(7);
			}
		}
		lowerpromo("  ", ' ', promo, promo2, 7);
	}
}
void savetofile(chess_piece** BOARD, int dimensions, int turn, players* p, move2 castle) {
	ofstream rdr("BSCS23070-Chess-Phase-3-savefile.txt");
	if (!rdr.is_open()) {
		cout << "FILE COULDN'T BE SAVED...";
		exit(1);
	}
	rdr << turn << endl;
	rdr << p[0].names << endl << p[0].color << endl << p[0].moves << endl << p[1].names << endl << p[1].color
		<< endl << p[1].moves << endl;
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			rdr << BOARD[i][j].clr << endl << BOARD[i][j].SYM << endl << BOARD[i][j].piece.r << endl <<
				BOARD[i][j].piece.c << endl;
		}
		cout << endl;
	}
	rdr << castle.king0 << endl << castle.king1 << endl << castle.rook0[0] << endl << castle.rook0[1] << endl
		<< castle.rook1[0] << endl << castle.rook1[1] << endl<<BOARD[0][0].ori.r<<endl<<BOARD[0][0].ori.c <<
		endl<<BOARD[0][0].desti.c <<endl<<BOARD[0][0].desti.r;
}
void deallocation2(chess_piece** temp, int dimensions) {
	for (int j = 0; j < dimensions; j++) {
		delete[]temp[j];
	}
	//delete[]temp;
}
int replaygame(chess_piece*** B, int move, int dimensions, players* p, bool& replay2) {
	bool replay = false;
	int fc = (cols / 2) - 10, fr = rows / 2;
	do {
		system("cls");
		int choicer, choicec;
		gotoRowCol(fr - 3, fc); cout << " -------------------------- ";
		gotoRowCol(fr - 2, fc); cout << " |  DO YOU WANT TO REPLAY? | ";
		gotoRowCol(fr - 1, fc); cout << " --------------------------";
		gotoRowCol(fr + 1, fc); cout << "  --------------------------";
		gotoRowCol(fr + 2, fc); cout << " |           YES           |";
		gotoRowCol(fr + 3, fc); cout << "  --------------------------";
		gotoRowCol(fr + 4, fc); cout << " |            NO            |";
		gotoRowCol(fr + 5, fc); cout << "  --------------------------";

		getRowColbyLeftClick(choicer, choicec);
		if (choicer == fr + 2) {
			replay = true;
			boardmaking();
			int r2 = 0, c2 = 0, r1 = 0, c1 = 0;
			for (int i = 0; i <= move; i++) {
				convert(B[i], dimensions, B[i][0][0].ori, r2, c2);
				convert(B[i], dimensions, B[i][0][0].desti, r1, c1);
				char sym = -37;
				for (int p = B[i][r2][c2].piece.r - 2; p < B[i][r2][c2].piece.r + 6; p++) {
					for (int o = B[i][r2][c2].piece.c - 2; o < B[i][r2][c2].piece.c + 5; o++)
					{
						if (((r2 + 1) + (c2 + 1)) % 2 == 0) {
							SetClr(7);
						}
						else {
							SetClr(0);
						}
						gotoRowCol(p, o);
						cout << sym;
						SetClr(7);
					}
				}
				for (int p = B[i][r1][c1].piece.r - 2; p < B[i][r1][c1].piece.r + 6; p++) {
					for (int o = B[i][r1][c1].piece.c - 2; o < B[i][r1][c1].piece.c + 5; o++)
					{
						if (((r1 + 1) + (c1 + 1)) % 2 == 0) {
							SetClr(7);
						}
						else {
							SetClr(0);
						}
						gotoRowCol(p, o);
						cout << sym;
						SetClr(7);
					}
				}
				displayboard(B[i], dimensions, B[i][0][0].turn, p);
				turnmsg(B[i][0][0].turn, p);
				if (_kbhit()) {
					char key = _getch();
					if (key == char(32)) {
						replay2 = true;
						return i;
						_getch();
					}
				}
				Sleep(1000);
			}
			Sleep(1000);
			system("cls");
			return -1;
		}
		if (choicer == fr + 4) {
			replay = false;
			system("cls");
			return -1;
		}
	} while (true);

}
void restoreboard(chess_piece**& BOARD, chess_piece***& Bhistory, int& turn, int rmove, int dimensions) {
	for (int i = 0; i < dimensions; i++) {
		for (int j = 0; j < dimensions; j++) {
			BOARD[i][j] = Bhistory[rmove][i][j];
			turn = Bhistory[rmove][i][j].turn;
		}
	}
}
bool keepplaying() {
	bool replay = false;
	int fc = (cols / 2) - 10, fr = rows / 2;
	bool active;
	do {
		system("cls");
		active = false;
		int choicer, choicec;
		gotoRowCol(fr - 3, fc); cout << " -------------------------- ";
		gotoRowCol(fr - 2, fc); cout << " |DO YOU WANT TO PLAY AGAIN?| ";
		gotoRowCol(fr - 1, fc); cout << " --------------------------";
		gotoRowCol(fr + 1, fc); cout << "  --------------------------";
		gotoRowCol(fr + 2, fc); cout << " |           YES           |";
		gotoRowCol(fr + 3, fc); cout << "  --------------------------";
		gotoRowCol(fr + 4, fc); cout << " |            NO            |";
		gotoRowCol(fr + 5, fc); cout << "  --------------------------";

		getRowColbyLeftClick(choicer, choicec);
		if (choicer == fr + 2) {
			system("cls");
			return true;
		}
		if (choicer == fr + 4) {
			replay = false;
			system("cls");
			return false;
		}
	} while (true);

}

int main() {
	int n;
	move2 castle{};
	int turn = 1;
	positon ori{}, desti{}, k{}, killo[2]{};
	int dimensions = 8;
	chess_piece** BOARD{}, ** BOARDCOPY{};
	chess_piece*** Bhistory{};
	players* p;
	kill* pieces{};
	int no = 0, move = 0, count = 0, rmove = 0;
	PlaySound(TEXT("BSCS23070-Chess-Phase-3-music.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	HideConsoleCursor();
	do {
		innit(BOARD, dimensions, p, Bhistory, move, turn, castle,pieces);
		bool chechk = false, chechkmatee = false, stalematee = false, castlech = true, replay = false;
		boardmaking();
		while (rmove != -1) {
			if (replay) {
				chechk = false, chechkmatee = false, stalematee = false, castlech = true, replay = false;
				restoreboard(BOARD, Bhistory, turn, rmove, dimensions);
				next_turn(turn);
			}
			while (!chechkmatee && !stalematee) {
				displayboard(BOARD, dimensions, turn, p);
				turnmsg(turn, p);
				do {
					do {
						do {
							do {
								do {
									if (no == 1) {
										highlight(BOARD, ori, dimensions, turn, p, -37, -37, castle, true);
										no = 0;
									}
									displayboard(BOARD, dimensions, turn, p);
									source(BOARD, ori, dimensions, "FROM ", n, Bhistory
										, turn, move, count, p,pieces,killo,desti,castle,k);
									highlight(BOARD, ori, dimensions, turn, p, -37, -37, castle, false);
									no++;
								} while (notopponent(BOARD, ori, turn, p, dimensions, false));
								PlaySound(TEXT("BSCS23070-Chess-Phase-3-drop.wav"), NULL, SND_FILENAME | SND_ASYNC);
								displayboard(BOARD, dimensions, turn, p);
								source(BOARD, desti, dimensions, " TO ", n, Bhistory
									, turn, move, count, p,pieces,killo,ori,castle,k);
								if (castlechechk(castle, BOARD, desti, turn, p, dimensions, ori)) {
									castlech = false;
								}
								highlight(BOARD, ori, dimensions, turn, p, -37, -37, castle, true);
							} while (notopponent_B(BOARD, desti, turn, p, dimensions, ori, castlech));
							PlaySound(TEXT("BSCS23070-Chess-Phase-3-drop.wav"), NULL, SND_FILENAME | SND_ASYNC);
						} while (!validmove(desti, ori, turn, BOARD, p, dimensions, castle));
						fakeboard(BOARDCOPY, BOARD, desti, ori, dimensions, p, turn);
					} while (!chechk && selfchehck(BOARDCOPY, turn, dimensions, p, false, castle));
					fakeboard(BOARDCOPY, BOARD, desti, ori, dimensions, p, turn);
					outofchechk(BOARDCOPY, turn, dimensions, p, chechk, castle, k);
				} while (chechk);
				inchehck(BOARDCOPY, turn, dimensions, p, chechk, castle, k);
				updateboard(BOARD, desti, ori, dimensions, p, turn, castlech, castle, move,killo,pieces);
				count = 0;
				displayboard(BOARD, dimensions, turn, p);
				pawnpromotion(BOARD, turn, p, dimensions);
				chehckmate(BOARD, turn, dimensions, p, chechkmatee, chechk, castle);
				if (stalemate(BOARD, turn, dimensions, p, castle)) {
					stalematee = true;
					PlaySound(TEXT("BSCS23070-Chess-Phase-3-end.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
				saveboard(BOARD, Bhistory, turn, dimensions, move, ori, desti);
				next_turn(turn);
			}
			rmove = replaygame(Bhistory, move, dimensions, p, replay);
		}
	} while (keepplaying());
	gameexit();
	deallocation(Bhistory, move, dimensions);
	deallocation2(BOARD, dimensions);
	return 0;
}


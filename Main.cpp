#include "Window.h"

int main(int argc, char** argv[]) {
	HWND hwnd = CreateWin();

	MSGLoop();

	return 0;
}
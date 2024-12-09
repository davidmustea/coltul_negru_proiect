#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <conio.h>

using namespace std;

void drawBackground(const char* imagePath, int screenWidth, int screenHeight) { // functia pentru fundal apelata in fiecare meniu & joc
    readimagefile(imagePath, 0, 0, screenWidth, screenHeight);
}


void startGame(int difficulty) {
    cleardevice();
    cout << "aici scrii cod pentru tabla de joc si tot"; // faci dimensiunea tablei de joc egala cu difficulty (4x4, 6x6, 8x8)
}

void meniuDif(int screenWidth, int screenHeight) {
    cleardevice();

    drawBackground("start_menu_bg.bmp", screenWidth, screenHeight);

    // Button Dimensions
    int buttonWidth = 200;  // Width of the buttons
    int buttonHeight = 50; // Height of the buttons

    // Centered X Coordinates
    int buttonX = screenWidth / 2 - buttonWidth / 2;

    // Display Difficulty Options
    readimagefile("easy_button.bmp", buttonX, 200, buttonX + buttonWidth, 200 + buttonHeight);
    readimagefile("medium_button.bmp", buttonX, 300, buttonX + buttonWidth, 300 + buttonHeight);
    readimagefile("hard_button.bmp", buttonX, 400, buttonX + buttonWidth, 400 + buttonHeight);

    // Check for Clicks
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            // Detect clicks on buttons
            if (x >= buttonX && x <= buttonX + buttonWidth) {
                if (y >= 200 && y <= 200 + buttonHeight) { startGame(4); break; }
                if (y >= 300 && y <= 300 + buttonHeight) { startGame(6); break; }
                if (y >= 400 && y <= 400 + buttonHeight) { startGame(8); break; }
            }
        }
    }
}



void meniuStart(int screenWidth, int screenHeight) {
    cleardevice();

    // Background
    drawBackground("start_menu_bg.bmp", screenWidth, screenHeight);

    // Display Title Image
    readimagefile("coltul_negru_title.bmp", screenWidth / 2 - 200, 50, screenWidth / 2 + 200, 150);

    // Display Start Button Image
    int buttonWidth = 200, buttonHeight = 100;
    int buttonX = screenWidth / 2 - buttonWidth / 2;
    int buttonY = screenHeight / 2 - buttonHeight / 2;

    readimagefile("start_button.bmp", buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Check for Clicks
    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY && y <= buttonY + buttonHeight) {
                meniuDif(screenWidth, screenHeight);
                break;
            }
        }
    }
}


int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, " ");
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    initwindow(screenWidth, screenHeight, "Coltul Negru");

    meniuStart(screenWidth, screenHeight);

    closegraph();
    return 0;
}

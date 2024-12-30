#include <iostream>
#include <graphics.h>
#include <windows.h>
#include <stack>

using namespace std;

void drawBackground(const char* imagePath, int screenWidth, int screenHeight) {
    readimagefile(imagePath, 0, 0, screenWidth, screenHeight);
}

void drawTable(int size, int screenWidth, int screenHeight) {
    cleardevice();
    drawBackground("start_menu_bg.bmp", screenWidth, screenHeight);

    int squareSize = min(screenWidth, screenHeight) / (size + 4); // cu +4 lasam spatiu de la marginea ecranului
    int matrixWidth = size * squareSize;
    int matrixHeight = size * squareSize;

    int startX = (screenWidth - matrixWidth) / 2;
    int startY = (screenHeight - matrixHeight) / 2;

    // tabla joc
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int x1 = startX + j * squareSize;
            int y1 = startY + i * squareSize;
            int x2 = x1 + squareSize;
            int y2 = y1 + squareSize;

            setfillstyle(SOLID_FILL, COLOR(139, 0, 0)); // fill
            bar(x1, y1, x2, y2);
            setcolor(WHITE); // outline
            rectangle(x1, y1, x2, y2);
        }
    }

    // coltul negru
    int x1 = startX + (size - 1) * squareSize;
    int y1 = startY + (size - 1) * squareSize;
    int x2 = x1 + squareSize;
    int y2 = y1 + squareSize;

    setfillstyle(SOLID_FILL, BLACK);
    bar(x1, y1, x2, y2);
    setcolor(WHITE);
    rectangle(x1, y1, x2, y2);
}

void drawPlayerTurn(int screenWidth, int screenHeight, bool isPlayerRed) {
    int statusX = screenWidth - 150;
    int statusY = screenHeight / 2;

    setfillstyle(SOLID_FILL, isPlayerRed ? RED : BLUE);
    bar(statusX, statusY, statusX + 20, statusY + 20);

    setcolor(WHITE);
    outtextxy(statusX + 30, statusY, const_cast<char*>(isPlayerRed ? "Player Red's Turn" : "Player Blue's Turn"));
}

void drawPieces(int screenWidth, int screenHeight, int selectedPiece) {
    int pieceX = 50;
    int pieceYStart = screenHeight / 2 - 100;

    // marime piese
    int pieceWidth = 50, pieceHeight = 50;

    // draw si highlight
    for (int i = 0; i < 3; ++i) {
        int pieceY = pieceYStart + i * 60; // pozitie piesa pe lista

        // highlight piesa
        if (selectedPiece == i + 1) setcolor(YELLOW); //  culoare highlight
        else setcolor(WHITE);

        rectangle(pieceX - 5, pieceY - 5, pieceX + pieceWidth + 5, pieceY + pieceHeight + 5);

        // imagini piesa
        const char* pieceImage =
            i == 0 ? "piece_cross.bmp" :
            i == 1 ? "piece_down_up.bmp" :
            "piece_up_down.bmp";
        readimagefile(pieceImage, pieceX, pieceY, pieceX + pieceWidth, pieceY + pieceHeight);
    }
}

bool checkWin(int board[10][10], int curRow, int curCol, int difficulty) {
    bool visited[10][10] = {false};
    stack<pair<int, int>> s;
    s.push({curRow, curCol});

    while (!s.empty()) {
        int row = s.top().first;
        int col = s.top().second;
        s.pop();

        if ((row == difficulty - 1 && col == difficulty - 2) || // stanga cn
            (row == difficulty - 2 && col == difficulty - 1) || // deasupra cn
            (row == difficulty - 1 && col == difficulty - 1)) { // pe cn
                return true;
}


        if (visited[row][col]) continue;
        visited[row][col] = true;

        // verificam vecinii
        int dx[] = {-1, 1, 0, 0}; // sus, jos, stanga, dreapta
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int newRow = row + dx[i];
            int newCol = col + dy[i];

            if (newRow >= 0 && newRow < difficulty && newCol >= 0 && newCol < difficulty &&
                !visited[newRow][newCol] && board[newRow][newCol] != 0) {
                s.push({newRow, newCol});
            }
        }
    }

    return false;
}

void startGame(int difficulty, int screenWidth, int screenHeight) {
    cleardevice();
    drawBackground("start_menu_bg.bmp", screenWidth, screenHeight);
    drawTable(difficulty, screenWidth, screenHeight);

    int selectedPiece = -1; // nicio piesa cu -1
    bool isPlayerRed = true;

    int squareSize = min(screenWidth, screenHeight) / (difficulty + 4);
    int boardStartX = (screenWidth - squareSize * difficulty) / 2;
    int boardStartY = (screenHeight - squareSize * difficulty) / 2;
    // tabla de joc
    int board[10][10] = {0}; // 0 empty, 1 cross, 2 vertical, 3 horizontal
    int lastRow = 0, lastCol = 0; // last placed
    board[0][0] = 1; // cross topleft

    drawTable(difficulty, screenWidth, screenHeight);

    while (true) {
        drawPlayerTurn(screenWidth, screenHeight, isPlayerRed);
        drawPieces(screenWidth, screenHeight, selectedPiece);

        // draw piese puse
        for (int row = 0; row < difficulty; ++row) {
            for (int col = 0; col < difficulty; ++col) {
                if (board[row][col] > 0) {
                    int x1 = boardStartX + col * squareSize;
                    int y1 = boardStartY + row * squareSize;
                    int x2 = x1 + squareSize;
                    int y2 = y1 + squareSize;

                    const char* pieceImage =
                        board[row][col] == 1 ? "piece_cross.bmp" :
                        board[row][col] == 2 ? "piece_down_up.bmp" : "piece_up_down.bmp";

                    readimagefile(pieceImage, x1, y1, x2, y2);
                }
            }
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            int pieceX = 50, pieceWidth = 50;
            int pieceYStart = screenHeight / 2 - 100, pieceHeight = 50;

            if (selectedPiece == -1) {
                // select piesa
                for (int i = 0; i < 3; ++i) {
                    int pieceY = pieceYStart + i * 60;
                    if (x >= pieceX && x <= pieceX + pieceWidth && y >= pieceY && y <= pieceY + pieceHeight) {
                        selectedPiece = i + 1; // piece sel
                        break;
                    }
                }
            } else {
                int col = (x - boardStartX) / squareSize;
                int row = (y - boardStartY) / squareSize;

                // validplacement
                bool isAdjacent = (abs(row - lastRow) == 1 && col == lastCol) ||
                                  (abs(col - lastCol) == 1 && row == lastRow);

                bool isBlackCorner = (row == difficulty - 1 && col == difficulty - 1);

                if (col >= 0 && col < difficulty && row >= 0 && row < difficulty &&
                    board[row][col] == 0 && isAdjacent && !isBlackCorner) {
                    board[row][col] = selectedPiece; // pune piesa
                    lastRow = row; // update
                    lastCol = col;
                    selectedPiece = -1; // deselect
                    isPlayerRed = !isPlayerRed;

                }
            }
        }

        if (checkWin(board, 0, 0, difficulty)) {
            if(isPlayerRed){
            outtextxy(screenWidth / 2 - 100, screenHeight / 2, "Player Blue Wins!");
            }
            else outtextxy(screenWidth / 2 - 100, screenHeight / 2, "Player Red Wins!");
}

        delay(30);
    }
}


void meniuDif(int screenWidth, int screenHeight) {
    cleardevice();

    drawBackground("start_menu_bg.bmp", screenWidth, screenHeight);

    int buttonWidth = 200;
    int buttonHeight = 50;

    int buttonX = screenWidth / 2 - buttonWidth / 2; // pentru centrare orizontala

    readimagefile("easy_button.bmp", buttonX, 300, buttonX + buttonWidth, 300 + buttonHeight);
    readimagefile("medium_button.bmp", buttonX, 400, buttonX + buttonWidth, 400 + buttonHeight);
    readimagefile("hard_button.bmp", buttonX, 500, buttonX + buttonWidth, 500 + buttonHeight);

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (x >= buttonX && x <= buttonX + buttonWidth) {
                if (y >= 300 && y <= 300 + buttonHeight) {startGame(4,screenWidth,screenHeight); break;}
                if (y >= 400 && y <= 400 + buttonHeight) {startGame(6,screenWidth,screenHeight); break;}
                if (y >= 500 && y <= 500 + buttonHeight) {startGame(8,screenWidth,screenHeight); break;}
            }
        }
    }
}

void meniuStart(int screenWidth, int screenHeight) {
    cleardevice();
    drawBackground("start_menu_bg.bmp", screenWidth, screenHeight);

    readimagefile("coltul_negru_title.bmp", screenWidth/2 - 600, screenHeight/2 - 300, screenWidth/2 + 600, screenHeight / 2 + 300 );

    int buttonWidth = 200, buttonHeight = 100;
    int buttonX = screenWidth / 2 - buttonWidth / 2 - 285;
    int buttonY = screenHeight / 2 - buttonHeight / 2 + 125;

    readimagefile("start_button.bmp", buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

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
    initgraph(&gd, &gm, const_cast<char*>(" "));
    int screenWidth = getmaxwidth();
    int screenHeight = getmaxheight();
    initwindow(screenWidth, screenHeight, "Coltul Negru");

    meniuStart(screenWidth, screenHeight);

    closegraph();
    return 0;
}

#include <Game.hpp>
#include <easyx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

#define ROW 10
#define COL 10
#define IMGW 40

void genMap(int map[ROW][COL]);
void loadImages();
void drawMap(int map[ROW][COL], IMAGE img[]);
void playGame(int map[ROW][COL]);
void mouseEvent(int map[ROW][COL]);
void expand(int map[ROW][COL], int row, int col);
void judge(int map[ROW][COL], int row, int col);
void openNewGame(int map[ROW][COL]);

bool isOver = false;
bool isWin = false;
int curOpenGridCnt = 0;
int const SCREENWIDTH = 400;
int const SCREENHIGHT = 460;
IMAGE img[13];
IMAGE face[4];
IMAGE bombcnt[10];
IMAGE timer[10];
int numbomb1 = 0;
int numbomb2 = 1;
int numbomb3 = 0;

int totalTimes = 0;
int time1 = 0;
int time2 = 0;
int time3 = 0;

int runing = 1;
int faceIndex = 0;

clock_t start_t;
clock_t end_t;
int first = 1;

int mineCnt = 10;
int markMineCnt = 0;

bool isClickFace = false;

void showMap(int map[ROW][COL])
{
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            printf("%2d ", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    Game game = Game();
    HWND window_size = initgraph(SCREENWIDTH, SCREENHIGHT, EX_SHOWCONSOLE);
    setbkcolor(RGB(0, 0, 0));
    cleardevice();

    int32_t map[ROW][COL] = { 0 };
    genMap(map);

    loadImages();

    playGame(map);

    getchar();

    closegraph();

    return 0;
}

void loadImages()
{
    for (int i = 0; i < 13; i++) {
        char fileName[50] = { 0 };
        sprintf_s(fileName, "./images/%d.jpg", i);
        loadimage(img + i, fileName, IMGW, IMGW);
    }

    loadimage(&face[0], "./images/face01.png", 40, 40);
    loadimage(&face[1], "./images/face02.png", 40, 40);
    loadimage(&face[2], "./images/face03.png", 40, 40);
    loadimage(&face[3], "./images/face04.png", 40, 40);

    string base = "./images/1_";
    for (int i = 0; i < 10; i++) {
        string strUrl = base + to_string(i) + ".png";

        loadimage(&bombcnt[i], strUrl.c_str(), 30, 40);
    }
    for (int i = 0; i < 10; i++) {
        string strUrl = base + to_string(i) + ".png";

        loadimage(&timer[i], strUrl.c_str(), 30, 40);
    }
}

void genMap(int map[ROW][COL])
{
    srand((unsigned)time(NULL));
    memset(map, 0, ROW * COL * sizeof(int));

    for (int i = 0; i < 10;) {
        int r = rand() % ROW;
        int c = rand() % COL;
        if (map[r][c] == 0) {
            map[r][c] = -1;
            i++;
        }
    }

    for (int i = 0; i < ROW; i++) {
        for (int k = 0; k < COL; k++) {
            if (map[i][k] == -1) {
                for (int r = i - 1; r <= i + 1; r++) {
                    for (int c = k - 1; c <= k + 1; c++) {
                        if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] != -1) {
                            map[r][c]++;
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < ROW; i++) {
        for (int k = 0; k < COL; k++) {
            map[i][k] += 20;
        }
    }
}

void drawMap(int map[ROW][COL], IMAGE img[])
{
    putimage(SCREENWIDTH / 2 - 20, 10, &face[faceIndex]);
    putimage(10, 10, &bombcnt[numbomb1]);
    putimage(40, 10, &bombcnt[numbomb2]);
    putimage(70, 10, &bombcnt[numbomb3]);
    putimage(SCREENWIDTH - 100, 10, &timer[time1]);
    putimage(SCREENWIDTH - 70, 10, &timer[time2]);
    putimage(SCREENWIDTH - 40, 10, &timer[time3]);

    for (int i = 0; i < ROW; i++) {

        for (int k = 0; k < COL; k++) {

            if (map[i][k] >= 0 && map[i][k] <= 8) {
                putimage(k * IMGW, i * IMGW + 60, img + map[i][k]);
            } else if (map[i][k] == -1) {
                putimage(k * IMGW, i * IMGW + 60, img + 9);
            } else if (map[i][k] >= 19 && map[i][k] <= 28) {
                putimage(k * IMGW, i * IMGW + 60, img + 10);
            } else if (map[i][k] == -100) {
                putimage(k * IMGW, i * IMGW + 60, img + 12);
            } else if (map[i][k] >= 39 && map[i][k] <= 48) {
                putimage(k * IMGW, i * IMGW + 60, img + 11);
            }
        }
    }
}

void playGame(int map[ROW][COL])
{
    while (true) {
        mouseEvent(map);

        drawMap(map, img);

        if (isOver) {
            int ret = MessageBox(GetHWnd(), "YOU SEP ON BOOB,WANA PLAY ANOTHER ROUND?", "hint", MB_OKCANCEL);
            if (ret == IDOK) {
                openNewGame(map);
            } else if (ret == IDCANCEL) {
                exit(1); //退出
            }
        }

        if (isWin) {
            int ret = MessageBox(GetHWnd(), "CONGRATES,YOU WON,WANNA PLAY ANOTHER ROUND?", "hint", MB_OKCANCEL);

            if (ret == IDOK) {
                openNewGame(map);
            } else {
                exit(1);
            }
        }

        end_t = clock();
        int second = (end_t - start_t) / 1000;
        if (runing == 1 && first == 0) {
            time1 = second / 100;
            time2 = second / 10 % 10;
            time3 = second % 10;
        } else {
            time1 = 0;
            time2 = 0;
            time3 = 0;
        }
    }
}

void mouseEvent(int map[ROW][COL])
{
    ExMessage msg;
    if (peekmessage(&msg, EX_MOUSE)) {
        int r = (msg.y - 60) / IMGW;
        int c = msg.x / IMGW;

        switch (msg.message) {
        case WM_LBUTTONDOWN:
            if (msg.x >= 0 && msg.x <= SCREENWIDTH && msg.y >= 60 && msg.y <= SCREENHIGHT && runing) {
                if (first == 1) {
                    start_t = clock();
                    first = 0;
                }

                if (map[r][c] >= 19 && map[r][c] <= 28) {
                    map[r][c] -= 20;
                    if (map[r][c] != -1)
                        curOpenGridCnt++;

                    expand(map, r, c);
                    judge(map, r, c);
                    showMap(map);
                }
            } else if (msg.x >= SCREENWIDTH / 2 - 20 && msg.x <= SCREENWIDTH / 2 + 20 && msg.y >= 20 && msg.y <= 50) {
                faceIndex = 1;
                isClickFace = true;
            }

            break;

        case WM_LBUTTONUP:
            if (runing == 1 && !isOver) {
                faceIndex = 0;
            }

            if (isClickFace) {
                isClickFace = false;
                openNewGame(map);
            }

            break;

        case WM_RBUTTONDOWN:
            if (msg.x >= 0 && msg.x <= SCREENWIDTH && msg.y >= 60 && msg.y <= SCREENHIGHT && runing == 1) {
                if (first == 1) {
                    start_t = clock();
                    first = 0;
                }

                if (map[r][c] <= 28 && map[r][c] >= 19) {
                    map[r][c] += 20;
                    markMineCnt += 1;

                } else if (map[r][c] <= 48 && map[r][c] >= 39) {
                    map[r][c] -= 20;
                    markMineCnt -= 1;
                }

                if (runing == 1 && markMineCnt <= mineCnt && markMineCnt >= 0) {
                    int curMineCnt = mineCnt - markMineCnt;
                    numbomb1 = curMineCnt / 100;
                    numbomb2 = curMineCnt / 10 % 10;
                    numbomb3 = curMineCnt % 10;
                } else {
                    numbomb1 = 0;
                    numbomb2 = 0;
                    numbomb3 = 0;
                }
            }
            break;
        }
    }
}

void expand(int map[ROW][COL], int row, int col)
{
    if (map[row][col] == 0) {

        for (int i = row - 1; i <= row + 1; i++) {
            for (int k = col - 1; k <= col + 1; k++) {
                if ((i >= 0 && i < ROW && k >= 0 && k < COL) && map[i][k] >= 19 && map[i][k] <= 28) {
                    if (map[i][k] > 19)
                        curOpenGridCnt++;

                    map[i][k] -= 20;

                    expand(map, i, k);
                }
            }
        }
    }
}

void judge(int map[ROW][COL], int row, int col)
{
    if (map[row][col] == -1) {
        map[row][col] = -100;
        runing = 0;
        for (int i = 0; i < ROW; i++) {
            for (int k = 0; k < COL; k++) {
                if (map[i][k] == 19) //踩雷
                {
                    map[i][k] = -1;
                }
            }
        }

        isOver = true;
        faceIndex = 2;
    }
    if (curOpenGridCnt == ROW * COL - 10) {

        isWin = true;
        runing = 0;
        faceIndex = 3;
    }
}

void openNewGame(int map[ROW][COL])
{
    genMap(map);
    showMap(map);
    isOver = false;
    isWin = false;
    curOpenGridCnt = 0;
    runing = 1;
    first = 1;
    markMineCnt = 0;

    numbomb1 = mineCnt / 100;
    numbomb2 = mineCnt / 10 % 10;
    numbomb3 = mineCnt % 10;

    faceIndex = 0;
}

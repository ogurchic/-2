//pig.cpp
#include "pig.h"
#include "renderer.h"
#include <cmath> 
#include <math.h>


Pig::Pig(int startX, int startY, int hitX, int hitY, int health) // �����������
{
    x = startX;
    y = startY;
    X = x;
    Y = y;
    health_lvl = health;
}

void Pig::moving() // ����������� �����
{
    Y = screenHeight / 2.35 * cos(X / 20) + screenHeight / 2.2;
    X += 0.2;
}

void Pig::negative_moving() {
    Y = screenHeight / 2.35 * cos(X / 20) + screenHeight / 2.2;
    X -= 0.2;
}

void Pig::draw() // ���������� ����� �� ������
{
    drawString(X + 2, Y, "/\\_____/\\", frontBuffer, FOREGROUND_GREEN);
    drawString(X + 1, Y + 1, "/ __    __\\", frontBuffer, FOREGROUND_GREEN);
    drawString(X, Y + 2, "|   @___@   |", frontBuffer, FOREGROUND_GREEN);
    drawString(X, Y + 3, "|    (oo)   |", frontBuffer, FOREGROUND_GREEN);
    drawString(X + 1, Y + 4, "\\    -    /", frontBuffer, FOREGROUND_GREEN);
    drawString(X + 3, Y + 5, "\\-----/", frontBuffer, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    drawString(X + 1, Y + 6, "/ /     \\ \\", frontBuffer, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    drawString(X, Y + 7, "* / ~~~~~ \\ *", frontBuffer, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    drawString(X + 1, Y + 8, " #########", frontBuffer, FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void Pig::health_down()
{
    health_lvl--;
}

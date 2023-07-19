//pig.h
#ifndef PIG_H
#define PIG_H

const int pigHitX = 14, pigHitY = 9; // �������
const int bigpigHitX = 31, bigpigHitY = 16; // �������

class Pig
{
public:
    int x, y; // ������� ����� �� ���� X � Y
    double X, Y; // ��� ������� �����������
    char health_lvl; // ������� �������� ������
    int direction = 1;

    Pig(int startX, int startY, int hitx, int hitY, int health); // �����������
    void moving(); // ����������� �����
    void negative_moving();
    void draw(); // ���������� ����� �� ������
    void health_down(); // ���������� ��������
};
#endif
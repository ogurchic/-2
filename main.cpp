//main.cpp
#include <conio.h>
#include "ship.h"
#include "renderer.h"
#include "bullet.h"
#include <vector>
#include "pig.h"
#include <string>
#include "bigpig.h"


bool operator==(const Bullet& lhs, const Bullet& rhs) // для устранения ошибки C2678 бинарный "==": не найден оператор, принимающий левый операнд типа "Bullet"
{
    return lhs.bullet_x == rhs.bullet_x && lhs.bullet_y == rhs.bullet_y;

}

int main()
{
    Ship ship(screenWidth / 2 - 6, screenHeight - 13, 3, shipHitX, shipHitY); // создание корабля с начальной позицией в центральной нижней части экрана
    std::vector <Pig> pigs; // создание вектора хранения свиней
    std::vector <Bullet> bullets; // создание вектора для хранения пуль
    std::vector <BigPig> bigpigs;

    int score = 10; // счёт игры

    DWORD lastCollisionTime = 0; // для интервала между нанесением урона кораблю
    DWORD lastBulletStartTime = 0; // для интервала между стрельбой
    DWORD lastPigStartTime = 0; // для интервала между выпусканием новых свиней
    DWORD lastBigpigStartTime = 0;

    // инициализация переднего и заднего буферов
    for (int i = 0; i < screenWidth * screenHeight; i++)
    {
        backBuffer[i].Char.AsciiChar = ' ';
        backBuffer[i].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;

        frontBuffer[i].Char.AsciiChar = ' ';
        frontBuffer[i].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
    }

    while (ship.health_lvl > 0) // игровой цикл
    {
        if (GetTickCount() - lastPigStartTime >= 7000) // если прошло достаточно времени с момента последнего вызова свиньи
        {
            pigs.push_back(Pig(0, 0, pigHitX, pigHitY, 5)); // создание новой свиньи 
            lastPigStartTime = GetTickCount(); // обновление времени последнего вывода свиньи
        }

        if (GetTickCount() - lastBigpigStartTime >= 120000 && score >= 10) {
            bigpigs.push_back(BigPig(screenWidth - 94, screenHeight / 2 - 16, bigpigHitX, bigpigHitY, 10));
            lastBigpigStartTime = GetTickCount();
        }

        if (_kbhit()) // если игрок нажал клавишу
        {
            int key = _getch(); // получить код нажатой клавиши
            if (key == 224) // если нажата клавиша со стрелкой
            {
                key = _getch(); // получить код нажатой клавиши со стрелкой
                if (key == 72) // если нажата клавиша со стрелкой вверх
                {
                    ship.moveUp(); // переместить корабль вверх
                }
                else if (key == 80) // если нажата клавиша со стрелкой вниз
                {
                    ship.moveDown(); // переместить корабль вниз
                }
                else if (key == 75) // если нажата клавиша со стрелкой влево
                {
                    ship.moveLeft(); // переместить корабль влево
                }
                else if (key == 77) // если нажата клавиша со стрелкой вправо
                {
                    ship.moveRight(); // переместить корабль вправо
                }
            }
            else if (key == ' ') // если нажата клавиша пробела (выстрел)
            {
                if (GetTickCount() - lastBulletStartTime >= 1000) // если прошло достаточно времени с момента последнего выстрела
                {
                    bullets.push_back(Bullet(ship.x + 7, ship.y, bulletHitX, bulletHitY)); // создание новой пули с начальной позицией в корабле
                    lastBulletStartTime = GetTickCount(); // обновление времени последнего выстрела
                }
            }
        }

        for (auto it = pigs.begin(); it != pigs.end();) // обход всех свиней 
        {
            if (it->health_lvl <= 0) // если здоровье свиньи равно 0 или меньше
            {
                it = pigs.erase(it); // удаление свиньи из вектора
                score++; // увеличение счёта за мервую свинью
            }

            else
            {
                ++it; // переход к следующей свинье
            }
        }

        for (auto it = bigpigs.begin(); it != bigpigs.end();) // обход всех свиней 
        {
            if (it->health_lvl <= 0) // если здоровье свиньи равно 0 или меньше
            {
                it = bigpigs.erase(it); // удаление свиньи из вектора
                score += 10; // увеличение счёта за мервую свинью
            }
            if (it->bigX <= 0)
                it = bigpigs.erase(it);
            else
            {
                ++it; // переход к следующей свинье
            }
        }



//______________ Отображение игрового экрана___________

        for (BigPig& bigpig : bigpigs) // побход всех big свиней в векторе big свиней
        {
            // проверка на столкновение свиньи и корабля
            if (checkCollision(ship.x, ship.y, shipHitX, shipHitY, bigpig.bigX, bigpig.bigY, bigpigHitX, bigpigHitY)) 
            {
                if (GetTickCount() - lastCollisionTime > 1000) { // интервал между потерей хп
                    ship.healthDown(); // потеря хп
                    bigpig.health_down();
                    lastCollisionTime = GetTickCount(); // обновите время последнего столкновения
                }
            }
            bigpig.moving(); // перемещение свиньи
            bigpig.draw(); // отображение свиньи на экране
        }


        for (Pig& pig : pigs) // обход всех свиней в векторе свиней
        {
            if (checkCollision(ship.x, ship.y, shipHitX, shipHitY, pig.X, pig.Y, pigHitX, pigHitY)) // проверка на столкновение свиньи и корабля
            {
                if (GetTickCount() - lastCollisionTime > 1000) { // интервал между потерей хп
                    ship.healthDown(); // потеря хп
                    pig.health_down();
                    lastCollisionTime = GetTickCount(); // обновите время последнего столкновения
                }
            }
            if (pig.X >= screenWidth - 63)
                pig.direction = -1;
            if (pig.X <= pigHitX)
                pig.direction = 1;

            if (pig.direction == 1)
                pig.moving(); // перемещение свиньи
            if (pig.direction == -1)  
                pig.negative_moving();
            
            pig.draw(); // отображение свиньи на экране
        }

        ship.draw(); // отобразить корабль на экране

        for (Bullet& bullet : bullets) // обход всех пуль 
        {
            bullet.shot(); // выстрел и движение пули
            bullet.drawBullet();// Обновите положение пули
        
            for (Pig& pig : pigs)
            {
                // проверка на столкновение пули и свинньи
                if (checkCollision(bullet.bullet_x, bullet.bullet_y, bulletHitX, bulletHitY, pig.X, pig.Y, pigHitX, pigHitY)) {
                    bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end()); // удаление попавшей пули
                    pig.health_down(); // уменьшение здоровья свиньи
                }
            }

            for (BigPig& bigpig : bigpigs)
            {
                // проверка на столкновение пули и свинньи
                if (checkCollision(bullet.bullet_x, bullet.bullet_y, bulletHitX, bulletHitY, bigpig.bigX, bigpig.bigY, bigpigHitX, bigpigHitY)) {
                    bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end()); // удаление попавшей пули
                    bigpig.health_down(); // уменьшение здоровья свиньи
                }
            }
        }
       
        // рамки и контекст
        for (int i = 0; i <= screenWidth; i++) {
            for (int j = 0; j <= screenHeight; j++) {
                if (i == 0 || j == 0 || i == screenWidth - 1 || j == screenHeight - 1 || i == screenWidth - 49)
                    drawString(i, j, "*", frontBuffer, FOREGROUND_GREEN | FOREGROUND_RED); // заполнение рамками
                if (i == screenWidth - 45 && j == 10) {
                    int num = ship.health_lvl;
                    std::string str = "Health level: " + std::to_string(num);
                    const char* cstr = str.c_str();
                    drawString(i, j, cstr, frontBuffer, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED); //вывод текущего здоровья
                }
                if (i == screenWidth - 45 && j == 15) {
                    std::string str = "Your score: " + std::to_string(score);
                    const char* scoreText = str.c_str();
                    drawString(i, j, scoreText, frontBuffer, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED); // вывод текущего счёта
                }
            }
        }

        // обмен местами переднего и заднего буферов
        swapBuffers();
        // отображение содержимого заднего буфера на экране
        displayBackBuffer();

        Sleep(1000 / 60); // задержка для управления скоростью игрового цикла
    }

    
    return 0;
}
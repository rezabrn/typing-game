#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include "helper_windows.h"

#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define LENGTH 20
#define NUMBER 10

FILE *stream;

COORD c;

char buffer[NUMBER][LENGTH] = {0};
int test[LENGTH] = {0};
char s[LENGTH][NUMBER] = {0};
int m = 0;
int n = 0;
int complecated = 0;
int IceWordControler = 0, HardWordControler = 0, LongWordControler = 0, HonoraryMode = 0, FreezingTime = 0;
int score = 0;

void Body();

void Menu(int*, float*, int*);

void Position(int x, int y);

void my_callback_on_key_arrival(char c)
{
    if (c == buffer[m][n])
    {
        s[m][n] = buffer[m][n];
        n++;
        if (c == '_')
            LongWordControler = 1;
        else if (c == '#')
            IceWordControler = 1;
        else if (c == '@' || c == '$' || c == '%' || c == '^' || c == '&' || c == '!')
            HardWordControler = 1;
    }
    if (buffer[m][n] == 0)
    {
        if (m == 10)
            m = 0;
        test[m] = 1;
        complecated++;
        m++;
        n = 0;
        if (m == 0 || m == 4 || m == 8)
            score += 1;
        if (LongWordControler == 1 && HardWordControler == 1)
        {
            LongWordControler = 0;
            HardWordControler = 0;
            score += 3;
            HonoraryMode = 1;
        }
        else if (IceWordControler == 1)
        {
            IceWordControler = 0;
            FreezingTime = 1;
        }
        else if (HardWordControler == 1)
        {
            HardWordControler = 0;
            score += 3;
        }
        else if (LongWordControler == 1)
        {
            LongWordControler = 0;
            score += 2;
        }
        else
            score += 1;
    }
}

int main()
{
    int Level;
    float LevelTime;
    int ReductionPercent;
    puts("Press one of this keys and then press the enter");
    puts("1. easy");
    puts("2. regular");
    puts("3. hard");
    scanf("%d", &Level);
    system("cls");
    stream = fopen("file_y.txt", "r");

    Menu(&Level, &LevelTime, &ReductionPercent);
    HANDLE thread_id = start_listening(my_callback_on_key_arrival);
    Body();
    for (int wave = 1; LevelTime >= 1000; wave++)
    {
        for (int i = 0; i < NUMBER; i++)
        fscanf(stream, "%s", buffer[i]);
        for (int i = 1; i <= 20 + complecated; i++)
        {
            Position(c.X = 1, c.Y = 1);

            for (int j = i; j > complecated; j--)
            {
                int SideSpace = (LENGTH - strlen(buffer[j-1])) / 2;
                for (int z = 0; z < SideSpace; z++)
                {
                    if ((j == 2 || j == 5 || j == 9) && j - 1 != m)
                        putchar('*');
                    else
                        putchar(' ');
                }
                for (int k = 0; buffer[j - 1][k] != 0 && j <= NUMBER; k++)
                {
                    if ((j == 2 || j == 5 || j == 9) && j - 1 != m)
                        putchar('*');
                    else if (test[j-1] == 0)
                    {
                        if (s[j-1][k] == buffer[j-1][k])
                            printf(GREEN);
                        putchar(buffer[j-1][k]);
                    }
                    printf(RESET);
                }
                if (HonoraryMode == 1 && i > 17)
                {
                    for (int t = 0; m < j - 1 && t < 20; t++) 
                    {
                        Sleep(LevelTime/10);
                        HonoraryMode = 0;
                    }
                }
                if (FreezingTime == 1)
                {
                    FreezingTime = 0;
                    for (int t = 0; m < j - 1 && t < 30; t++)
                        Sleep(LevelTime/10);
                }
                if (j > NUMBER)
                    putchar(' ');
                for (int z = 0; z < SideSpace; z++)
                {
                    if ((j == 2 || j == 5 || j == 9) && j - 1 != m)
                        putchar('*');
                    else
                        putchar(' ');
                    if (strlen(buffer[i]) % 2 == 1 && SideSpace -1 == j)
                    {
                        if (j == 0 || j == 4 || j == 8)
                            putchar('*');
                        else
                            putchar(' ');
                    }
                }
                putchar('\n');
            }
            Position(c.X = 22, c.Y = 23);

            printf("score = %d", score);
            Sleep(LevelTime);
            
            for (int j = 0; j < i && i != NUMBER; j++)
            {
                printf("\033[1K");
                printf("\r                    ");
                for (int k = 0; k < LENGTH; k++)
                    printf("\b");
            }
        }

        if (complecated < 10)
        {
            Position(c.X = 23, c.Y = 22);

            printf("score = %d\n", score);
            puts("You lost!");
            printf("level = %d\n", Level);
            printf("Wave = %d\n", wave);
            return 0;
        }
        for (int i = 0; i < NUMBER; i++)
            test[i] = 0;
        m = 0;
        complecated = 0;
        LevelTime *= (1-(float)ReductionPercent/100);
        if (LevelTime <= 100)
            LevelTime = 100;
    }
    Position(c.X = 22, c.Y = 23);

    printf("score = %d\n", score);
    puts("Your typing speed has improved!");
    Sleep(5000);
    return 0;
    fclose(stream);
    WaitForSingleObject(thread_id, INFINITE);
}

void Menu(int *Level, float *LevelTime, int *ReductionPercent)
{
    if (*Level == 1)
    {
        *LevelTime = 10000;
        *ReductionPercent = 20;
    }
    if (*Level == 2)
    {
        *LevelTime = 8000;
        *ReductionPercent = 30;
    }
    if (*Level == 3)
    {
        *LevelTime = 5000;
        *ReductionPercent = 40;
    }
}

void Body()
{
    for (int k = 0; k < 22; k++)
    {
        if (k == 0 || k == 21)
            puts("######################");
        else
            puts("                      ");
    }
}

void Position(int x, int y)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
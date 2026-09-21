#include<stdio.h>
#include<stdlib.h>
#include "raylib.h"

typedef struct
{
    char name[30];
    int score;
}Player;


void saveScore(char name[], int score)
{
    Player players[100];
    int count = 0;

    FILE *file = fopen("leaderboard.txt", "r");

    // Read old scores
    if (file != NULL)
    {
        while (count < 100 &&
               fscanf(file, "%29[^|]|%d\n",
                      players[count].name,
                      &players[count].score) == 2)
        {
            count++;
        }

        fclose(file);
    }

    // Add current player
    strcpy(players[count].name, name);
    players[count].score = score;
    count++;

    // Sort highest score to lowest
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (players[j].score > players[i].score)
            {
                Player temp = players[i];

                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    // Only keep top 10
    if (count > 10)
        count = 10;

    // Rewrite the file
    file = fopen("leaderboard.txt", "w");

    if (file == NULL)
        return;

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s|%d\n",
                players[i].name,
                players[i].score);
    }

    fclose(file);
}


void drawLeaderboard()
{
    Player players[10];
    int count = 0;

    FILE *file = fopen("leaderboard.txt", "r");

    if (file != NULL)
    {
        while (count < 10 &&
               fscanf(file, "%29[^|]|%d\n",
                      players[count].name,
                      &players[count].score) == 2)
        {
            count++;
        }

        fclose(file);
    }

    ClearBackground(SKYBLUE);

    DrawText("LEADERBOARD", 400, 80, 50, MAROON);

    for (int i = 0; i < count; i++)
    {
        DrawText(
            TextFormat("%d. %s", i + 1, players[i].name),
            300,
            170 + i * 50,
            30,
            BLACK
        );

        DrawText(
            TextFormat("%d", players[i].score),
            750,
            170 + i * 50,
            30,
            BLACK
        );
    }
}
//explain these codes line by line to me with its full significance
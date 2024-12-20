#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stringManip.h"

typedef struct Card
{
    char name[4];
    int value;
} Card;

// Swap two cards
void swap(Card *a, Card *b)
{
    Card temp = *a;
    *a = *b;
    *b = temp;
}

// Fisher-Yates shuffle algorithm
void shuffle(Card *array, int n)
{
    srand(time(NULL));

    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

int main(int argc, char *argv[])
{
    // Check if file name was passed as argument.
    if (argc < 2)
    {
        printf("Not enough arguments.");
        return 0;
    }

    // Attempt to open file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("File could not be opened.");
        return 0;
    }

    Card deck[52];

    char line[10];
    char *parts[2]; // Card Name + Value
    int cardCount = 0;

    // Read and parse file
    while (fgets(line, sizeof(line), fp))
    {
        // Clear new line
        line[strcspn(line, "\n")] = 0;

        if (str_split(line, ',', parts, 2) == 2)
        {
            int value = atoi(parts[1]);

            printf("Card name: %s\n", parts[0]);
            printf("Card value: %d\n", value);

            printf("\n");

            strcpy(deck[cardCount].name, parts[0]);
            deck[cardCount].value = atoi(parts[1]);

            cardCount++;
        }
    }

    fclose(fp);

    // Shuffle the deck
    shuffle(deck, cardCount);

    printf("Parse the cards...");

    for (int i = 0; i < cardCount; i++)
    {
        printf("Card %2d: %-3s (Value: %d)\n", i + 1, deck[i].name, deck[i].value);
    }

    // Core gameplay logic.
}

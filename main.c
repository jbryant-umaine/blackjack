#include <stdio.h>
#include <stdlib.h>

typedef struct Card
{
    char name[4];
    int value;
    struct Card *next;
} Card;

// Create a LinkedList of cards
Card *createCard(const char *name, int value)
{
    Card *newCard = (Card *)malloc(sizeof(Card));
    strcpy(newCard->name, name);
    newCard->value = value;
    newCard->next = NULL;
    return newCard;
}

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

    printf("Parse the cards...");
}

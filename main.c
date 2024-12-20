#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stack.h"
#include "stringManip.h"

// Swap two cards
void swap(Card *a, Card *b)
{
    Card temp = *a;
    *a = *b;
    *b = temp;
}

// Display the hand using pointer arithmetic
void displayHand(Card *hand, int size)
{
    Card *ptr = hand;
    printf("Hand: ");
    while (ptr < hand + size)
    {
        printf("%s (%d) ", ptr->name, ptr->value);
        ptr++;
    }
    printf("\n");
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

    char line[10];
    char *parts[2]; // Card Name + Value
    int cardCount = 0;

    // Create and initialize deck as a stack
    STACK deck;
    stackInit(&deck, 52);

    // Read and parse file
    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;

        if (str_split(line, ',', parts, 2) == 2)
        {
            Card newCard;
            strcpy(newCard.name, parts[0]);
            newCard.value = atoi(parts[1]);

            stackPush(&deck, newCard);
            cardCount++;
        }
    }

    fclose(fp);

    // Convert to array for shuffling
    Card tempDeck[52];
    for (int i = 0; i < cardCount; i++)
    {
        tempDeck[i] = deck.stack[i];
    }

    // Shuffle
    shuffle(tempDeck, cardCount);

    // Put shuffled cards back into stack
    stackFree(&deck);
    stackInit(&deck, 52);
    for (int i = cardCount - 1; i >= 0; i--)
    {
        stackPush(&deck, tempDeck[i]);
    }

    // Initialize hands for dealer and player
    STACK dealerHand, playerHand;
    stackInit(&dealerHand, 10);
    stackInit(&playerHand, 10);

    // Draw two cards for dealer
    printf("\nDealer's hand:\n");
    int dealerSum = 0;
    for (int i = 0; i < 2; i++)
    {
        Card drawnCard = stackPop(&deck);
        stackPush(&dealerHand, drawnCard);
        dealerSum += drawnCard.value;

        if (i == 0)
        {
            printf("Card 1: %s (Value: %d)\n", drawnCard.name, drawnCard.value);
            printf("Card 2: [Hidden]\n");
        }
    }
    printf("Dealer's visible total: %d\n", dealerHand.stack[0].value);

    // Draw two cards for player
    printf("\nPlayer's hand:\n");
    int playerSum = 0;
    for (int i = 0; i < 2; i++)
    {
        Card drawnCard = stackPop(&deck);
        stackPush(&playerHand, drawnCard);
        playerSum += drawnCard.value;
    }
    displayHand(playerHand.stack, playerHand.top + 1); // Show initial hand
    printf("Player's total: %d\n", playerSum);

    // Player's turn
    char choice[10];
    int playerStands = 0;
    printf("\nPlayer's turn:\n");

    while (playerSum < 21 && !playerStands)
    {
        printf("\nYour current hand:\n");
        displayHand(playerHand.stack, playerHand.top + 1); // Show current hand
        printf("Your total: %d\n", playerSum);
        printf("What would you like to do?\n");
        printf("1. Hit\n");
        printf("2. Stand\n");
        printf("Choice: ");

        fgets(choice, sizeof(choice), stdin);

        switch (choice[0])
        {
        case '1': // Hit
            Card drawnCard = stackPop(&deck);
            stackPush(&playerHand, drawnCard);
            playerSum += drawnCard.value;
            printf("Drew: %s (Value: %d)\n", drawnCard.name, drawnCard.value);
            break;

        case '2': // Stand
            printf("Player stands at %d\n", playerSum);
            playerStands = 1;
            break;

        default:
            printf("Invalid choice. Try again.\n");
        }
    }

    // Dealer's turn
    printf("\nDealer's turn:\n");
    printf("Dealer's hidden card was: %s (Value: %d)\n", dealerHand.stack[1].name, dealerHand.stack[1].value);
    printf("Dealer's current total: %d\n", dealerSum);

    while (dealerSum < 13)
    {
        Card drawnCard = stackPop(&deck);
        stackPush(&dealerHand, drawnCard);
        dealerSum += drawnCard.value;
        printf("Dealer hits: %s (Value: %d)\n", drawnCard.name, drawnCard.value);
        printf("Dealer's new total: %d\n", dealerSum);
    }

    // Determine winner
    printf("\nFinal Results:\n");
    printf("Dealer's final hand:\n");
    displayHand(dealerHand.stack, dealerHand.top + 1); // Show dealer's final hand
    printf("Dealer's total: %d\n\n", dealerSum);

    printf("Your final hand:\n");
    displayHand(playerHand.stack, playerHand.top + 1); // Show player's final hand
    printf("Your total: %d\n", playerSum);

    if (playerSum > 21)
        printf("Player busts! Dealer wins!\n");
    else if (dealerSum > 21)
        printf("Dealer busts! Player wins!\n");
    else if (playerSum > dealerSum)
        printf("Player wins!\n");
    else if (dealerSum > playerSum)
        printf("Dealer wins!\n");
    else
        printf("It's a tie!\n");

    // Clear all stacks
    stackFree(&deck);
    stackFree(&dealerHand);
    stackFree(&playerHand);
    return 0;
}
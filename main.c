#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "stack.h"
#include "stringManip.h"

// Swap two cards
void swap(Card *a, Card *b)
{
    Card temp = *a;
    *a = *b;
    *b = temp;
}

// Take a hand of cards and the hand size and display it using pointer arithmetic
void displayHand(Card *hand, int size)
{
    Card *ptr = hand;
    int sum = 0;
    printf("Hand: ");

    while (ptr < hand + size)
    {
        // If it's an Ace and counting it as 11 would bust, show it as 1
        if ((strcmp(ptr->name, "AD") == 0 ||
             strcmp(ptr->name, "AH") == 0 ||
             strcmp(ptr->name, "AS") == 0 ||
             strcmp(ptr->name, "AC") == 0) &&
            sum + 11 > 21)
        {
            printf("%s (1) ", ptr->name);
            sum += 1;
        }
        else
        {
            printf("%s (%d) ", ptr->name, ptr->value);
            sum += ptr->value;
        }
        ptr++;
    }
    printf("\n");
}

// Take the hand and the size of the hand and return the sum of the current hand
// Used for calculating Ace of spades.
int calculateHandValue(Card *hand, int size)
{
    int sum = 0;
    int numAces = 0;
    Card *ptr = hand;

    // count aces and sum non-aces
    while (ptr < hand + size)
    {
        if (strcmp(ptr->name, "AD") == 0 ||
            strcmp(ptr->name, "AH") == 0 ||
            strcmp(ptr->name, "AS") == 0 ||
            strcmp(ptr->name, "AC") == 0)
        {
            numAces++;
            sum += 1; // Initially count Aces as 1
        }
        else
        {
            sum += ptr->value;
        }
        ptr++;
    }

    // convert Aces from 1 to 11 if possible
    while (numAces > 0 && sum + 10 <= 21)
    {
        sum += 10; // Convert one Ace from 1 to 11
        numAces--;
    }

    return sum;
}

// Take a card array and perform the Fisher-Yates shuffle algorithm
void shuffle(Card *array, int n)
{
    srand(time(NULL));

    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

// Parses cards, runs the main game, and awaits for user commands.
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
    for (int i = 0; i < 2; i++)
    {
        Card drawnCard = stackPop(&deck);
        stackPush(&dealerHand, drawnCard);
        if (i == 0)
        {
            printf("Card 1: %s\n", drawnCard.name);
            printf("Card 2: [Hidden]\n");
        }
    }
    int dealerSum = calculateHandValue(dealerHand.stack, dealerHand.top + 1);
    printf("Dealer's visible total: %d\n", dealerHand.stack[0].value);

    // Draw two cards for player
    printf("\nPlayer's hand:\n");

    for (int i = 0; i < 2; i++)
    {
        Card drawnCard = stackPop(&deck);
        stackPush(&playerHand, drawnCard);
    }

    displayHand(playerHand.stack, playerHand.top + 1);

    int playerSum = calculateHandValue(playerHand.stack, playerHand.top + 1);

    printf("Player's total: %d\n", playerSum);

    // Player's turn
    char choice[10];
    int playerStands = 0;
    printf("\nPlayer's turn:\n");

    while (playerSum < 21 && !playerStands)
    {
        printf("\nYour current hand:\n");
        displayHand(playerHand.stack, playerHand.top + 1);
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
            playerSum = calculateHandValue(playerHand.stack, playerHand.top + 1);
            printf("Drew: %s\n", drawnCard.name);
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
    printf("Dealer's hidden card was: %s\n", dealerHand.stack[1].name);
    printf("Dealer's current total: %d\n", dealerSum);

    while (dealerSum < 13)
    {
        Card drawnCard = stackPop(&deck);
        stackPush(&dealerHand, drawnCard);
        dealerSum = calculateHandValue(dealerHand.stack, dealerHand.top + 1);
        printf("Dealer hits: %s\n", drawnCard.name);
        printf("Dealer's new total: %d\n", dealerSum);
    }

    // Determine winner
    printf("\nFinal Results:\n");
    printf("Dealer's final hand:\n");
    displayHand(dealerHand.stack, dealerHand.top + 1);
    printf("Dealer's total: %d\n\n", dealerSum);

    printf("Your final hand:\n");
    displayHand(playerHand.stack, playerHand.top + 1);
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
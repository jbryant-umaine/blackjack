#include <stdlib.h>
#include "stack.h"

// Initializes the stack of cards
void stackInit(STACK *s, int size)
{
    s->stack = (Card *)malloc(size * sizeof(Card));
    s->size = size;
    s->top = -1;
}

// Frees the stack
void stackFree(STACK *s)
{
    free(s->stack);
    s->top = -1;
    s->size = 0;
}

// Removes a card from the top of the stack
Card stackPop(STACK *s)
{
    Card emptyCard = {"", -1};
    if (s->top == -1)
    {
        return emptyCard;
    }
    return s->stack[s->top--];
}

// Pushes a card into the stack
int stackPush(STACK *s, Card card)
{
    if (s->top == s->size - 1)
    {
        return -1;
    }

    s->stack[++s->top] = card;
    return 0;
}
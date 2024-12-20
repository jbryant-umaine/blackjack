#include <stdlib.h>
#include "stack.h"

void stackInit(STACK *s, int size)
{
    s->stack = (Card *)malloc(size * sizeof(Card));
    s->size = size;
    s->top = -1;
}

void stackFree(STACK *s)
{
    free(s->stack);
    s->top = -1;
    s->size = 0;
}

Card stackPop(STACK *s)
{
    Card emptyCard = {"", -1};
    if (s->top == -1)
    {
        return emptyCard;
    }
    return s->stack[s->top--];
}

int stackPush(STACK *s, Card card)
{
    if (s->top == s->size - 1)
    {
        return -1;
    }

    s->stack[++s->top] = card;
    return 0;
}
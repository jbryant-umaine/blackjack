/**
 * stack.h
 *
 * Header file for defining methods for stack.c
 */
#ifndef STACK_H
#define STACK_H

typedef struct Card
{
    char name[4];
    int value;
} Card;

typedef struct
{
    Card *stack;
    int size;
    int top;
} STACK;

void stackInit(STACK *s, int size);
void stackFree(STACK *s);
Card stackPop(STACK *s);
int stackPush(STACK *s, Card card);

#endif
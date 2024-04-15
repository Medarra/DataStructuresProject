#ifndef INPUT
#define INPUT

#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_INPUT 50

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void getInteger(const char inputPrompt[], int* result) {
    char input[MAX_INPUT];
    bool loop = true;
    while (loop) {
        printf("\n%s >> ", inputPrompt);
        fgets(input, MAX_INPUT - 1, stdin);

        /* Check for a input buffer overflow */
        if (strchr(input, '\n') == NULL) {
            clearInputBuffer();
        }

        if (sscanf(input, "%d", result) == 1) {
            loop = false;
        }
        else {
            printf("\n--ERROR: Input was not a valid value--\n");
        }
    }
}

void getDouble(const char inputPrompt[], double* result) {
    char input[MAX_INPUT];
    bool loop = true;
    while (loop) {
        printf("\n%s >> ", inputPrompt);
        fgets(input, MAX_INPUT - 1, stdin);

        /* Check for a input buffer overflow */
        if (strchr(input, '\n') == NULL) {
            clearInputBuffer();
        }

        if ((sscanf(input, "%lf", result) == 1) && *result >= 0.00) {
            loop = false;
        }
        else {
            printf("\n--ERROR: Input was not a valid value--\n");
        }
    }
}

void getString(const char inputPrompt[], char* result) {
    char input[MAX_INPUT];
    bool loop = true;
    printf("\n%s >> ", inputPrompt);
    fgets(input, (MAX_INPUT - 1), stdin);

    /* Check for a input buffer overflow */
    if (strchr(input, '\n') == NULL) {
        clearInputBuffer();
    }

    /* Remove newline character */
    input[strlen(input) - 1] = '\0';
    sscanf(input, "%s", result);
}

#endif
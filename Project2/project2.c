/* iaed-23 - Ines Ji - ist199238 - project2 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project2.h"


int main() {
    int restart;
    lists *list = init();
    /* Executes the program until the user sends the 'q' command */
    while ((restart = handleInput(list))) {
        if (restart == RESTART) {
            list = init();
        }
    }
    deleteData(list);
    return 0;
}

/**
 * Handles the input,
 * returns 1 to stop the program if the command is 'q',
 * otherwise returns 0.
 */
int handleInput(lists *list) {
    char c;
    c = getchar();

    switch (c) {
        case 'c':
            addAndListL(list);
            return CONTINUE;
        case 'p':
            addAndListS(list);
            return CONTINUE;
        case 'l':
            addLink(list);
            return CONTINUE;
        case 'i':
            listI(list);
            return CONTINUE;
        /****Practical Test****/
        case 'f':
            commandF(list);
            return CONTINUE;
        /**********************/
        case 'r':
            removeLine(list);
            return CONTINUE;
        case 'e':
            eliminateStop(list);
            return CONTINUE;
        case 'a':
            deleteData(list);
            return RESTART;
        case 'q':
            return QUIT;
        default:
            return CONTINUE;
    }
}

/**
 * Handles the 'c' command.
 * Its function depends on the number of parameters after the command,
 * it can list all lines, create a new line 
 * or just handle a specific line.
 */
void addAndListL(lists *list) {
    int end;
    char c, *lineName;
    
    if ((c = getchar()) == EOF || c == '\n') {
        listAllLines(list);
    } else {
        lineName = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
        end = readLine(lineName);
        if (doesLineExist(list, lineName) == NULL) {
            createAndInsertLine(list, lineName);
        } else {
            listLine(list, lineName, end);
        }
        free(lineName);
    }
}

/**
 * Handles the 'p' command.
 * Its function depends on the the number of parameters after the command,
 * it can list all stops, a specific one or create a new one.
 * count = 0, equals to no parameters after command 'p'.
 * count = 1, equals 'p' <stop's name>.
 * count = 2, equals 'p' <stop's name> <longitude> <latitude>.
 */
void addAndListS(lists *list) {
    char *stopName;
    int count = 0;
    stopName = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
    count = readStop(stopName, count);
    if (count == 0) {
        listAllStops(list);
    } else if (count == 1) {
        searchStop(list, stopName);
    } else if (count == 2) {
        createAndInsertStop(list, stopName);
    }
    free(stopName);
}

/**
 * Handles the 'l' command.
 * Creates a new link, verifies the validity of its information and 
 * updates the line with their new origin/destination.
 */
void addLink(lists *list) {
    double cost, duration;
    char *lineName, *stopOrig, *stopDest;
    lineName = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
    stopOrig = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
    stopDest = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
    if (readNamesLink(list, lineName, stopOrig, stopDest) == UNSUCCESS) {
        clean();
        freeNames(lineName, stopOrig, stopDest);
        return;
    }

    scanf("%lf %lf", &cost, &duration);
    if (isCostDurationValid(cost, duration) == UNSUCCESS) {
        clean();
        freeNames(lineName, stopOrig, stopDest);
        return;
    }

    if (doesLineHasStops(list, lineName) == UNSUCCESS) {
        updateEmptyLine(list, lineName, stopOrig, stopDest);
    } else {
        updateLine(list, lineName, stopOrig, stopDest);
    }
    createLink(list, lineName, stopOrig, stopDest, cost, duration);
    freeNames(lineName, stopOrig, stopDest);
}

/**
 * Handles the 'i' command.
 * List all intersections between lines.
 * Each row lists a stop with the number of lines in which they belong 
 * and its names in alphabetical order.
 */
void listI(lists *list) {
    nodeStop *x, *head = list->headStop;
    int i, n, totalLines = numLines(list->headLine);
    char **lines, *stopName;
    for (x = head; x != NULL; x = x->next) {
        lines = (char**)safe_malloc(totalLines*sizeof(char*));
        stopName = (char*)safe_malloc((strlen(convertStop(x->value)->name)+1)
        *sizeof(char));
        strcpy(stopName, convertStop(x->value)->name);
        n = calculateLines(list, stopName, lines);
        alphabeticalOrder(lines, n);

        if (n <= 1) {
            freeArray(n, lines);
            free(stopName);
            continue;
        }
        printf("%s %d:", stopName, n);
        for (i = 0; i < n; i++) {
            printf(" %s", lines[i]);
        }
        putchar('\n');

        freeArray(n, lines);
        free(stopName);
    }
}

/**
 * Handles the command 'r'.
 * It receives a line's name and removes both the 
 * object and its links from the memory.
 */
void removeLine(lists *list) {
    nodeLine *x;
    char *lineName = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
    readLine(lineName);
    if ((x = doesLineExist(list, lineName)) == NULL) {
        printf("%s: " LINE_INVALID, lineName);
        free(lineName);
        return;
    }
    deleteLinksFromLine(list, lineName);
    deleteItems(list, list->headLine, x->value, LINE);
    free(lineName);
}

/**
 * Handles the 'e' command.
 * It eliminates the stop from the memory and 
 * the links in which it participates.
 */
void eliminateStop(lists *list) {
    int count = 0;
    nodeStop *x;
    char *stopName = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
    readStop(stopName, count);
    if ((x = isDuplicateStop(list, stopName)) == NULL) {
        printf("%s: " STOP_INVALID, stopName);
        free(stopName);
        return;
    }
    updateAfterStopDeletion(list, stopName);
    deleteItems(list, list->headStop, x->value, STOP);
    free(stopName);
}

/**
 * Handles the command 'a'.
 * Deletes all data from the memory.
 * It deletes the linked lists, its nodes and values
 * and the struct that has them saved.
 */
void deleteData(lists *list) {
    destroyLists(list);
    free(list);
}

/****Practical Test****/
void commandF(lists *list) {
    int i = 0;
    char *stopName;
    nodeLine *x;
    Line *line;
    stopName = (char*)safe_malloc(sizeof(char)*(MAX_COMMAND_SIZE - 2));
    readStop(stopName, 0);
    if (isDuplicateStop(list, stopName) == NULL) {
        printf("%s: " STOP_INVALID, stopName);
        free(stopName);
        return;
    }
    if (list->headLine == NULL) {
        return;
    }
    for (x = list->headLine; x != NULL; x = x->next) {
        line = convertLine(x->value);
        if (doesLineHasStops(list, line->name) == UNSUCCESS) {
            continue;
        }
        if (strcmp(stopName, line->destination.name) == 0) {
            if (i != 0) {
                putchar (' ');
            }
            printf("%s", line->name);
            i++;
        }
    }
    if (i != 0) {
        putchar('\n');
    }
    free(stopName);
}

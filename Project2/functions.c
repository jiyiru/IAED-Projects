/* iaed-23 - Ines Ji - ist199238 - project2 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project2.h"

/* Command c */

/**
 * List all lines with their respective origin and destination stops, 
 * their total number of stops, their duration and cost.
 */
void listAllLines(lists *list) {
    double cost, duration;
    nodeLine *x;
    nodeLink *y;
    for (x = list->headLine; x != NULL; x = x->next) {
        Line *line = convertLine(x->value);
        cost = 0, duration = 0;
        for (y = list->headLink; y != NULL; y = y->next) {
            Link *link = convertLink(y->value);
            if (strcmp(link->line.name, line->name) == 0) {
                cost += link->cost;
                duration += link->duration;
            }
        }
        /****Practical Test****/
        if (strlen(line->name) < 6) {
        /**********************/
            if (doesLineHasStops(list, line->name) == UNSUCCESS) {
                printf("%s %d %.2f %.2f\n", line->name, line->numStops, cost, 
                duration);
            } else {
                printf("%s %s %s %d %.2f %.2f\n", line->name, line->origin.name, 
                line->destination.name, line->numStops, cost, duration);
            }
        }
    }
}

/**
 * Populates the line's name with the input from stdin.
 * If the maximum length is reached before a line break/EOF/space,
 * the function will keep reading and discarding the output.
 * If the line's name is the last parameter in the input 
 * the function returns 1, otherwise it returns 0.
 */
int readLine(char *lineName) {
    char c;
    int i = 0;
    
    while ((c = getchar()) != EOF && c != '\n') {
        /*Skips the first space if it exists*/
        if ((c == ' ' || c == '\t') && i == 0) {
            continue;
        /*If it reaches a space, the string is done*/
        } else if ((c == ' ' || c == '\t') && i > 0) {
            break;
        /*Ignores the rest of the input if string is full*/
        } else if (i < MAX_COMMAND_SIZE - 2) {
            lineName[i++] = c;
        }
    }
    lineName[i] = '\0';
    /*Checks if the name is the last parameter in the input*/
    if (c == EOF || c == '\n') {
        return 1;
    }
    return 0;
}

/**
 * Creates a new line and inserts it to the list with all lines.
 */
void createAndInsertLine(lists *list, char *lineName) {
    Line *line = (Line*)safe_malloc(sizeof(Line));
    line->name = (char*)safe_malloc((strlen(lineName)+1)*sizeof(char));
    strcpy(line->name, lineName);
    line->numStops = 0;
    insertList(list, line, LINE);
}

/**
 * Handles the listing of the line's stops,
 * the order can be origin->destination or the inverse, 
 * depending on the input.
 * Receives a variable end which tells if there is
 * still input left from stdin to read.
 */
void listLine(lists *list, char *lineName, int end) {
    char *input;
    int length = (MAX_COMMAND_SIZE-2-strlen(lineName)-1);

    if (end == 1) {
        printLine(list, lineName, NORMAL);
    } else if (end == 0) {
        input = (char*)safe_malloc(sizeof(char)*(length));
        scanf("%s", input);
        if (isSortValid(input) == SUCCESS) {
            printLine(list, lineName, INVERSED);
        }
        free(input);
    }
}

/**
 * Checks if the line has any stops and if it does
 * it prints the stops in the asked order
 * (origin -> destination or destination -> origin).
 */
void printLine(lists *list, char *lineName, int inv) {
    int j, numStops = getNumStopsFromLine(list, lineName);
    char **stops;
    if (doesLineHasStops(list, lineName) == UNSUCCESS) {
        return;
    }
    stops = (char**)safe_malloc(numStops*sizeof(char*));
    buildStopsOrder(list, lineName, stops);
    if (inv == INVERSED) {
        for (j = numStops; j > 1; j--) {
            printf("%s, ", stops[j]);
        }
        printf("%s\n", stops[j]);
    } else {
        for (j = 0; j < numStops-1; j++) {
            printf("%s, ", stops[j]);
        }
        printf("%s\n", stops[j]);
    }
    freeArray(numStops, stops);
}  

/**
 * Builds an array with the stops in the order origin->destination
 * of the given line.
 */
void buildStopsOrder(lists *list, char *lineName, char **stops) {
    nodeLink *x;
    Link *link;
    int i = 0;

    for (x = list->headLink; x != NULL; x = x->next) {
        link = convertLink(x->value);
        if (strcmp(link->line.name, lineName) == 0) {
            if (i == 0) {
                stops[i] = (char*)safe_malloc((strlen(link->origin.name)+1)*
                sizeof(char));
                strcpy(stops[i++], link->origin.name);
                stops[i] = (char*)safe_malloc((strlen(link->destination.name)
                +1)*sizeof(char));
                strcpy(stops[i++], link->destination.name);
            } else if (strcmp(stops[i-1], link->origin.name) == 0) {
                stops[i] = (char*)safe_malloc((strlen(link->destination.name)
                +1)*sizeof(char));
                strcpy(stops[i++], link->destination.name);
            } else if (strcmp(stops[0], link->destination.name) == 0) {
                moveToBegin(stops, link->origin.name, i);
                i++; 
            }
        }
    }
}

/**
 * Auxiliary function for buildStopsOrder.
 * It inserts the stop's name in the beginning of the array. 
 */
void moveToBegin(char **stops, char *stopName, int i) {
    int j;
    for (j = i; j > 0; j--) {
        if (j == i) {
            stops[j] = (char*)safe_malloc((strlen(stops[j-1])+1)*sizeof(char));
        } else {
            stops[j] = (char*)safe_realloc(stops[j], 
            (strlen(stops[j-1])+1)*sizeof(char));
        }
        strcpy(stops[j], stops[j-1]);
    }
    stops[0] = (char*)safe_realloc(stops[0],(strlen(stopName)+1)*sizeof(char));
    strcpy(stops[0], stopName);
}


/* Command 'p' */

/**
 * Populates the stop's name with the input from stdin.
 * When it is reading a name without spaces, 
 * if the maximum length is reached before a line break/EOF/space,
 * the function will keep reading and discarding the output.
 * Variable count is used to control the whitespacing between the 
 * arguments.
 * When count = 1 means it had read a space before the name.
 */
int readStop(char *stopName, int count) {
    int i = 0;
    char c;

    if (count == 0) {
        if ((c = getchar()) == ' ' || c == '\t') {
            count = 1;
        }
    }
    /*Starts reading after the space*/ 
    if (count == 1) {
        if ((c = getchar()) == '"') {
            readStopQM(stopName);
        } else {
            do {
                /*Ignores the rest of the input if string is full*/
                if (i < MAX_COMMAND_SIZE - 4) {
                    stopName[i++] = c;
                }
            } while((c = getchar()) != EOF && c != '\n' && c != ' '); 
            stopName[i] = '\0';
        }
        /*Verifies if the input has ended yet*/
        if (c != EOF && c != '\n') {
            count = 2;
        }
    }
    return count;
}

/**
 * Populates the stop's name with the input from stdin.
 * Since it is reading a name limited by quotation marks, 
 * if the maximum length is reached before the quotation marks,
 * the function will keep reading and discarding the output.
 */
void readStopQM(char *stopName) {
    int i = 0;
    char c;

    while ((c = getchar()) != '"') {
        /*Ignores the rest of the input if string is full*/
        if (i < MAX_COMMAND_SIZE - 3) {
            stopName[i++] = c; 
        }
    }
    stopName[i] = '\0';
    /*To read the space/EOF/'\n' after the name*/
    c = getchar();
}

/**
 * Lists all stops with their name, latitude, longitude
 * and number of lines in which they belong.
 */
void listAllStops(lists *list) {
    nodeStop *x, *headStop;
    char **lines;
    Stop *stop;
    int totalLines, num_lines = 0;
    headStop = list->headStop;
    totalLines = numLines(list->headLine);

    for (x = headStop; x != NULL; x = x->next) {
        lines = (char**)safe_malloc(totalLines*sizeof(char*));
        stop = convertStop(x->value);
        num_lines = calculateLines(list, stop->name, lines);
        printf("%s: %16.12f %16.12f %d\n", stop->name, 
        stop->latitude, stop->longitude, num_lines);
        freeArray(num_lines, lines);
    }
}

/**
 * Calculates how many lines does the stop belong to and
 * returns its number.
 */
int calculateLines(lists *list, char *stopName, char **lines) {
    nodeLink *head = list->headLink, *x;
    Link *link;
    int j, n = 0;

    for (x = head; x != NULL; x = x->next) {
        link = convertLink(x->value);
        if (strcmp(link->origin.name, stopName) == 0 ||
        strcmp(link->destination.name, stopName) == 0) {
            /*Checks whether the line is already in the array*/
            for (j = 0; j < n; j++) {
                if (strcmp(lines[j], link->line.name) == 0) {
                    j = UNSUCCESS;
                    break;
                }
            }
            if (j == UNSUCCESS) {
                continue;
            }
            lines[n] = (char*)safe_malloc((strlen(link->line.name)+1)
            *sizeof(char));
            strcpy(lines[n++], link->line.name);
        }
    }
    return n;
}

/**
 * Searches the specific stop in the list of stops 
 * and prints its latitude and longitude.
 */
void searchStop(lists *list, char *stopName) {
    nodeStop *x;
    Stop *stop;

    x = isDuplicateStop(list, stopName);
    if (x == NULL) {
        printf("%s: " STOP_INVALID, stopName);
    } else if (x != NULL) {
        stop = convertStop(x->value);
        printf("%16.12f %16.12f\n", stop->latitude, 
        stop->longitude);
    }
}

/**
 * Creates a new stop with its longitude and latitude.
 */
void createAndInsertStop(lists *list, char *stopName) {
    Stop *stop = (Stop*)safe_malloc(sizeof(Stop));
    double longitude, latitude;

    scanf("%lf %lf", &latitude, &longitude);
    /****Practical Test****/
    if (latitude < -90 || latitude > 90) {
        printf("invalid location.\n");
        return;
    }
    if (longitude < -180 || longitude > 180) {
        printf("invalid location.\n");
        return;
    }
    /**********************/
    if (isDuplicateStop(list, stopName) != NULL) {
        printf("%s: " STOP_ALREADY_EXISTS, stopName);
        return;
    }
    stop->name = (char*)safe_malloc((strlen(stopName)+1)*sizeof(char));
    strcpy(stop->name, stopName);
    stop->longitude = longitude;
    stop->latitude = latitude;
    insertList(list, stop, STOP);
}


/* Command 'l'*/

/**
 * Frees the three given strings. 
 */
void freeNames(char *lineName, char *stopOrig, char *stopDest) {
    free(lineName);
    free(stopOrig);
    free(stopDest);
}

/**
 * Reads the input from stdin of the parameters after 'l'.
 * It reads the name of the line, the stop's origin and 
 * the stop's destination and verifies if they are valid.
 */
int readNamesLink(lists *list, char *lineName, char *stopOrig, 
char *stopDest) {
    int count = 1;
    readLine(lineName);
    if (doesLineExist(list, lineName) == NULL) {
        printf("%s: " LINE_INVALID, lineName);
        return UNSUCCESS;
    }
    /*count starts at 1 since the function readLine already skipped through
    the space before the name's stop*/
    readStop(stopOrig, count);
    readStop(stopDest, count);

    if (isDuplicateStop(list, stopOrig) == NULL) {
        printf("%s: " STOP_INVALID, stopOrig);
        return UNSUCCESS;
    }
    if (isDuplicateStop(list, stopDest) == NULL) {
        printf("%s: " STOP_INVALID, stopDest);
        return UNSUCCESS;
    }
    if (isLinkValid(list, lineName, stopOrig, stopDest) == UNSUCCESS) {
        printf(LINK_INVALID);
        return UNSUCCESS;
    }

    return SUCCESS;
}

/**
 * Updates the line with origin/destination when it doesn't have any stops.
 */
void updateEmptyLine(lists *list, char *lineName, char *stopOrig, 
char *stopDest) {
    nodeLine *x;
    nodeStop *y;
    x = doesLineExist(list, lineName);
    y = isDuplicateStop(list, stopOrig);
    convertLine(x->value)->origin = *convertStop(y->value);
    y = isDuplicateStop(list, stopDest);
    convertLine(x->value)->destination = *convertStop(y->value);
    convertLine(x->value)->numStops = 2;
}

/**
 * Updates the line (already with stops) with new origin/destination stops 
 * after a new link is added.
 */
void updateLine(lists *list, char *lineName, char *stopOrig, char *stopDest) {
    nodeLine *x;
    nodeStop *y;
    Line *line;
    x = doesLineExist(list, lineName);
    line = convertLine(x->value);
    
    /*Checks if the line will become circular after the new link is added*/
    if (strcmp(line->origin.name, stopDest) == 0 && 
    strcmp(line->destination.name, stopOrig) == 0) {
        y = isDuplicateStop(list, stopDest);
        line->destination = *convertStop(y->value);
        line->numStops++;
        return;
    }
    /*Checks if the new destination equals to the line's origin stop*/
    if (strcmp(line->origin.name, stopDest) == 0) {
        y = isDuplicateStop(list, stopOrig);
        line->origin = *convertStop(y->value);
        line->numStops++;
    }
    /*Checks if the new origin equals to the line's destination stop*/
    if (strcmp(line->destination.name, stopOrig) == 0) {
        y = isDuplicateStop(list, stopDest);
        line->destination = *convertStop(y->value);
        line->numStops++;
    }
    return;
}

/**
 * Creates a new link with its name, its origin and 
 * destination stops, its cost and its duration.
 */
void createLink(lists *list, char *lineName, char *stopOrig, char *stopDest, 
double cost, double duration) {
    Link *link = (Link*)safe_malloc(sizeof(Link));
    nodeLine *x;
    nodeStop *y;
    x = doesLineExist(list, lineName);
    link->line = *convertLine(x->value);
    y = isDuplicateStop(list, stopOrig);
    link->origin = *convertStop(y->value);
    y = isDuplicateStop(list, stopDest);
    link->destination = *convertStop(y->value);
    link->cost = cost;
    link->duration = duration;
    insertList(list, link, LINK);
}


/* Command 'i' */

/**
 * Receives an array of char arrays and orders it in alphabetical order.
 */
void alphabeticalOrder(char **lines, int n) {
    int i, j;
    char *temp;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n-1-i; j++) {
            if (strcmp(lines[j], lines[j+1]) > 0) {
                temp = (char*)safe_malloc((strlen(lines[j])+1)*sizeof(char));
                strcpy(temp, lines[j]);
                strcpy(lines[j], lines[j+1]);
                strcpy(lines[j+1], temp);
                free(temp);
            }
        }
    }
}


/* Command 'r' */

/**
 * Deletes the links in which the line's name is part of,
 * from the linked list and the object itself.
 */
void deleteLinksFromLine(lists *list, char *lineName) {
    nodeLink *x = list->headLink, *next;
    Link *link;

    while (x != NULL) {
        next = x->next;
        link = convertLink(x->value);
        if (strcmp(link->line.name, lineName) == 0) {
            deleteItems(list, list->headLink, x->value, LINK);
        }
        x = next;
    }
}


/* Command 'e' */

/**
 * Updates the rest of the memory (links and lines) in which the
 * stop is part of (after the removal of it). 
 */
void updateAfterStopDeletion(lists *list, char *stopName) {
    int i, n, numLinks, totalLines = numLines(list->headLine);
    nodeLink **links;
    char **lines = (char**)safe_malloc(sizeof(char*)*totalLines);
    n = calculateLines(list, stopName, lines);
    /*Goes through each line that the stops belongs to*/
    for (i = 0; i < n; i++) {
        numLinks = (getNumStopsFromLine(list, lines[i]) - 1);
        links = (nodeLink**)safe_malloc(sizeof(nodeLink*)*numLinks);
        buildLineLinkOrder(list, lines[i], links);
        updateLinks(list, links, stopName, numLinks);
        updateLineStops(list, lines[i]);
        free(links);
    }
    freeArray(n, lines);
}

/**
 * Builds an array of node links from a specific line in order.
 */
void buildLineLinkOrder(lists *list, char *lineName, nodeLink **links) {
    int i = 0;
    nodeLink *x;
    Link *link, *finalLink, *initialLink;
    for (x = list->headLink; x != NULL; x = x->next) {
        link = convertLink(x->value);
        if (strcmp(link->line.name, lineName) == 0) {
            if (i == 0) {
                links[i++] = x;
                continue;
            }     
            initialLink = convertLink(links[0]->value);
            finalLink = convertLink(links[i-1]->value);
            /*Adds to the end of the array*/
            if (strcmp(link->origin.name, finalLink->destination.name) == 0) {
                links[i++] = x;
            /*Adds to the beginning of the array*/
            } else if (strcmp(link->destination.name, initialLink->origin.name) 
            == 0) {
                insertBeginLink(links, x, i);
                i++;
            }
        }
    }
}

/**
 * Auxiliar function of buildLineLinkOrder.
 * It inserts the node link to the beginning of the array.
 */
void insertBeginLink(nodeLink **links, nodeLink *x, int i) {
    int j;

    links[i] = links[i-1];
    /*Moves each element to the right*/
    for (j = i-1; j > 0; j--) {
        links[j] = links[j-1];
    }
    links[0] = x;
}

/**
 * Updates the links after the removal of the given stop.
 * It deletes the link if the stop is in one of the extremities of the line,
 * otherwise deletes one link and updates the next with the
 * sum of the cost and duration of the deleted link and the updated one. 
 */
void updateLinks(lists *list, nodeLink **links, char *stopName, int numLinks) {
    int i;
    Link *link, *nextLink;
    char *name = (char*) safe_malloc(MAX_COMMAND_SIZE);
    
    for (i = 0; i < numLinks; i++) {
        link = convertLink(links[i]->value);
        strcpy(name, link->line.name);
        /*The stop is the origin of the line*/
        if (strcmp(link->origin.name, stopName) == 0) {
            deleteItems(list, list->headLink, links[i]->value, LINK);
            updateNumStopsFromLine(list, name);
        } else if (strcmp(link->destination.name, stopName) == 0) {
             /*The stop is not the destination of the line*/
            if (i != (numLinks-1)) {
                nextLink = convertLink(links[i]->next->value);
                nextLink->cost += link->cost;
                nextLink->duration += link->duration;
                nextLink->origin = link->origin;
            } 
            deleteItems(list, list->headLink, links[i]->value, LINK);
            updateNumStopsFromLine(list, name);
        }
    }
    free(name);
}

/**
 * Updates the line's stops (destination and origin) in case
 * they changed.
 */
void updateLineStops(lists *list, char *lineName) {
    int numStops;
    char **stops;
    nodeLine *x = doesLineExist(list, lineName);
    nodeStop *y;
    Line *line = convertLine(x->value);
    if (doesLineHasStops(list, lineName) == UNSUCCESS) {
        return;
    }
    numStops = getNumStopsFromLine(list, lineName);
    stops = (char**)safe_malloc(numStops*sizeof(char*));
    buildStopsOrder(list, lineName, stops);
    /*Check if the origin has changed and updated it*/
    if (strcmp(stops[0], line->origin.name) != 0) {
        y = isDuplicateStop(list, stops[0]);
        line->origin = *convertStop(y->value);
    }
    /*Check if the destination has changed and updated it*/
    if (strcmp(stops[numStops-1], line->destination.name) != 0) {
        y = isDuplicateStop(list, stops[numStops-1]);
        line->destination = *convertStop(y->value);
    }
    freeArray(numStops, stops);
}

/**
 * Updates the variable numStops of the line, substracting one stop
 * every time the function is called unless it has 0 stops.
 */
void updateNumStopsFromLine(lists *list, char *lineName) {
    nodeLine *x;
    Line *line;
    x = doesLineExist(list, lineName);
    line = convertLine(x->value);
    if (doesLineHasStops(list, lineName) == UNSUCCESS) {
        line->numStops = 0;
        return;
    }
    line->numStops--;
}


/* Verifications */

/**
 * Checks if the stop already exists in the stop linked list.
 * If it does, it returns the node that contains it.
 * otherwise, it returns NULL.
 */
nodeStop* isDuplicateStop(lists *list, char *stopName) {
    nodeStop *x;
    Stop *stop;
    if (list->headStop == NULL) {
        return NULL;
    }
    for (x = list->headStop; x != NULL; x = x->next) {
        stop = convertStop(x->value);
        if (strcmp(stop->name, stopName) == 0) {
            return x;
        }
    }
    return NULL;
}

/**
 * Checks if the line exists in the line linked list.
 * If it does, it returns the node that contains it.
 * Otherwise, it returns NULL.
 */
nodeLine* doesLineExist(lists *list, char *lineName) {
    nodeLine *x;
    Line *line;
    if (list->headLine == NULL) {
        return NULL;
    }
    for (x = list->headLine; x != NULL; x = x->next) {
        line = convertLine(x->value);
        if (strcmp(line->name, lineName) == 0) {
            return x;
        }
    }
    return NULL;
}

/**
 * Checks if the specified line has any stops.
 * If it does, it returns SUCCESS. 
 * Otherwise, it returns UNSUCCESS.
 */
int doesLineHasStops(lists *list, char *lineName) {
    nodeLink *x;
    Link *link;
    if (list->headLink == NULL) {
        return UNSUCCESS;
    }
    for (x = list->headLink; x != NULL; x = x->next) {
        link = convertLink(x->value);
        if (strcmp(link->line.name, lineName) == 0) {
            return SUCCESS;
        }
    }
    return UNSUCCESS;
}

/**
 * Checks if the link is valid.
 * If it is, it returns SUCCESS.
 * Otherwise, it returns UNSUCCESS.
 */
int isLinkValid(lists *list, char *lineName, char *stopOrig, char *stopDest) {
    nodeLine *x;
    Line *line;

    if (doesLineHasStops(list, lineName) == UNSUCCESS) {
        return SUCCESS;
    }

    x = doesLineExist(list, lineName);
    line = convertLine(x->value);
    if (strcmp(line->origin.name, stopDest) == 0) {
        return SUCCESS;
    }
    if (strcmp(line->destination.name, stopOrig) == 0) {
        return SUCCESS;
    }
    return UNSUCCESS;
}

/**
 * Checks if the input for 'inverse'
 * is valid.
 * If it is, it returns SUCCESS.
 * Otherwise, it returns UNSUCCESS.
 */
int isSortValid(char *input) {
    char *inv = INVERSE;
    int i, size = strlen(input);

    if (size < 3 || size > 7) {
        printf(INCORRECT_SORT);
        return UNSUCCESS;
    }
    for (i = 0; i < size; i++) {
        if (inv[i] != input[i]) {
            printf(INCORRECT_SORT);
            return UNSUCCESS;
        }
    }
    return SUCCESS;
}

/**
 * Checks if the cost and duration is valid.
 * If it is, it returns SUCCESS.
 * Otherwise, it returns UNSUCCESS.
 */
int isCostDurationValid(int cost, int duration) {
    if (cost < 0 || duration < 0) {
        printf(COST_OR_DURATION_INVALID);
        return UNSUCCESS;
    }
    return SUCCESS;
}

/**
 * Returns the number of stops of the specified line.
 */
int getNumStopsFromLine(lists *list, char *lineName) {
    nodeLine *x;
    Line *line;
    for (x = list->headLine; x != NULL; x = x->next) {
        line = convertLine(x->value);
        if (strcmp(line->name, lineName) == 0) {
            return line->numStops;
        }
    }
    return UNSUCCESS;
}

/**
 * Returns the number of lines that exist in the linked list
 * of lines. 
 */
int numLines(nodeLine *headLine) {
    int count = 0;
    nodeLine *x;
    for(x = headLine; x != NULL; x = x->next) {
        count++;
    }
    return count;
}


/* utils */

/**
 * Frees the array and its components.
 */
void freeArray(int length, char **array) {
    int i;
    for(i = 0; i < length; i++) {
        free(array[i]);
    }
    free(array);
}

/**
 * Allocates memory and
 * checks if the operation went correctly.
 */
void* safe_malloc(unsigned int size) {
	void* p = malloc(size);
	if (p != NULL) {
        return p;
    } 
	printf(ERR_NO_MEMORY);
	exit(EXIT_CODE_NO_MEMORY);
	return NULL;
}

/**
 * Reallocates memory and 
 * checks if the operation went correctly.
 */
void* safe_realloc(void *p, unsigned int size) {
    p = realloc(p, size);
	if (p != NULL) {
        return p;
    } 
	printf(ERR_NO_MEMORY);
	exit(EXIT_CODE_NO_MEMORY);
	return NULL;
}

/**
 * Reads until it finds a break of line.
 * Used to clean up the rest of the input if there is an error.
 */
void clean() {
    char c;
    while ((c = getchar()) != '\n') {
    }
}
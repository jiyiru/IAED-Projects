/* iaed-23 - Inês Ji - ist199238 - project1 */

#include <stdio.h>
#include <string.h>

#include "project1.h"

/*Arrays containing the lines*/
Line LineList[MAXLINES];
/*Array containing the stops*/
Stop StopList[MAXSTOPS];
/*Array containing the links*/
Link LinkList[MAXLINKS];
/*Struct containing the counters for the arrays*/
Counter counter;

int main() {
    counter.numStop = 0;
    counter.numLine = 0;
    counter.numLink = 0;

    /* Executes the program until the user sends the 'q' command */
    while (handleInput()) {
    }
    return 0;
}

/**
 * Handles the input,
 * returns 1 to stop the program if the command is 'q',
 * otherwise returns 0.
 */
int handleInput() {
    char c;
    c = getchar();
    
    switch (c) {
        case 'c':
            addAndListL();
            return 1;
        case 'p':
            addAndListS();
            return 1;
        case 'l':
            addLink();
            return 1;
        case 'i':
            listI();
            return 1;
        case 'q':
            return 0;
        default:
            return 1;
    }
}

/**
 * Handles the 'c' command.
 * Its function depends on the number of parameters after the command,
 * it can list all lines, create a new line 
 * or just handle a specific line.
 */
void addAndListL() {
    int end;
    char c, lineName[MAXNAMEL];
    
    if ((c = getchar()) == EOF || c == '\n') {
        listAllLines();
    } else {
        end = readLine(lineName);
        if (doesLineExist(lineName) == UNSUCCESS) {
            createAndInsertLine(lineName);
        } else {
            listLine(lineName, end);
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
int readLine(char lineName[]) {
    char c;
    int i = 0;
    
    while ((c = getchar()) != EOF && c != '\n') {
        /*Skips the first space if it exists*/
        if (c == ' ' && i == 0) {
            continue;
        /*If it reaches a space, the string is done*/
        } else if (c == ' ' && i > 0) {
            break;
        /*Ignores the rest of the input if string is full*/
        } else if (i < MAXNAMEL - 1) {
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
 * List all lines with their respective origin and destination stops, 
 * their total number of stops, their duration and cost.
 */
void listAllLines() {
    int i, j, num_stops;
    double cost, duration;
    
    for (i = 0; i < counter.numLine; i++) {
        cost = 0, duration = 0, num_stops = 0;
        for (j = 0; j < counter.numLink; j++) {
            /*For each line, it goes through the array of links to find 
            the ones containing the line and adds its needed values*/
            if (strcmp(LinkList[j].line.name, LineList[i].name) == 0) {
                cost += LinkList[j].cost;
                duration += LinkList[j].duration;
                num_stops++; 
            }
        }
        if (doesLineHasStops(LineList[i].name) == UNSUCCESS) {
            printf("%s %d %.2f %.2f\n", LineList[i].name, num_stops, cost, 
            duration);
        } else {
            num_stops++;
            printf("%s %s %s %d %.2f %.2f\n", LineList[i].name, 
            LineList[i].origin.name, LineList[i].destination.name, 
            num_stops, cost, duration);
        }
    }
}

/**
 * Creates a new line and inserts it to the array with all lines.
 */
void createAndInsertLine(char lineName[]) {
    Line l = {0};

    strcpy(l.name, lineName);
    LineList[counter.numLine] = l;
    counter.numLine++;
}

/**
 * Handles the listing of the line's stops,
 * the order can be origin->destination or the inverse, 
 * depending on the input.
 * Receives a variable end which tells if there is
 * still input left from stdin to read.
 */
void listLine(char lineName[], int end) {
    char input[BUFSIZ];

    if (end == 1) {
        printLine(lineName, NORMAL);
    } else if (end == 0) {
        scanf("%s", input);
        if (strlen(input) > NUM_INV) {
            printf(INCORRECT_SORT);
        } else if (strcmp(input, INV) == 0) {
            printLine(lineName, INVERSED);
        } else if (strcmp(input, INVE) == 0) {
            printLine(lineName, INVERSED);
        } else if (strcmp(input, INVER) == 0) {
            printLine(lineName, INVERSED);
        } else if (strcmp(input, INVERS) == 0) {
            printLine(lineName, INVERSED);
        } else if (strcmp(input, INVERSE) == 0) {
            printLine(lineName, INVERSED);
        } else {
            printf(INCORRECT_SORT);
            return;
        }
    }
}

/**
 * Checks if the line has any stops and if it does
 * it prints the stops in the asked order
 * (origin -> destination or destination -> origin).
 */
void printLine(char lineName[], int inv) {
    int id;
    char stop[MAXNAMES], lastStop[MAXNAMES];

    if (doesLineHasStops(lineName) == UNSUCCESS) {
        return;
    }
    /*Gets the index of the line in the array of lines*/
    id = doesLineExist(lineName);
    if (inv == INVERSED) {
        strcpy(stop, LineList[id].destination.name);
        strcpy(lastStop, LineList[id].origin.name);
    } else if (inv == NORMAL) {
        strcpy(stop, LineList[id].origin.name);
        strcpy(lastStop, LineList[id].destination.name);
    }
    printf("%s, ", stop);

    printStopIfCircular(lineName, stop, id, inv);
    printLineStops(lineName, stop, lastStop, inv);
}

/**
 * Checks if the line is circular, if it is circular 
 * it prints the second stop in the asked order.
 * This is used so the function printLineStops is able to print the stops
 * in loop until the stop == finalStop (which is not possible if 
 * startStop == finalStop from the start).
 */
void printStopIfCircular(char lineName[], char stop[], int id, int inv) {
    int i;
    char origStop[MAXNAMES], newStop[MAXNAMES];
    /*Checks if the origin of the line == destination of the line*/
    if (strcmp(LineList[id].origin.name, LineList[id].destination.name) == 0) {
        for (i = 0; i < counter.numLink; i++) {
            /*Searches in the array for the index with the line given*/
            if (strcmp(LinkList[i].line.name, lineName) == 0) {
                if (inv == INVERSED) {
                    strcpy(origStop, LinkList[i].destination.name);
                    strcpy(newStop, LinkList[i].origin.name);
                } else if (inv == NORMAL) {
                    strcpy(origStop, LinkList[i].origin.name);
                    strcpy(newStop, LinkList[i].destination.name);
                }
                /*If the origin stop in the link equals to the current stop
                then we have a new destination stop to print*/
                if (strcmp(origStop, stop) == 0) {
                    strcpy(stop, newStop);
                    printf("%s, ", stop);
                    break;
                }
            }
        }
    }
}

/**
 * Prints the line's stop in the asked order (normal or inverse) through
 * looping the array of links to find the order of the connections.
 */
void printLineStops(char lineName[], char stop[], char lastStop[], int inv) {
    int i;
    char origStop[MAXNAMES], newStop[MAXNAMES];
    /*Loops until the last printed stop is the last stop*/
    while (strcmp(stop, lastStop) != 0) {
        for (i = 0; i < counter.numLink; i++) {
            if (strcmp(LinkList[i].line.name, lineName) == 0) {
                if (inv == INVERSED) {
                    strcpy(origStop, LinkList[i].destination.name);
                    strcpy(newStop, LinkList[i].origin.name);
                } else if (inv == NORMAL) {
                    strcpy(origStop, LinkList[i].origin.name);
                    strcpy(newStop, LinkList[i].destination.name);
                }
                /*If the origin stop in the link equals to the current stop
                then we have a new destination stop to print*/
                if (strcmp(origStop, stop) == 0) {
                    strcpy(stop, newStop);
                    if (strcmp(stop, lastStop) != 0) {
                        printf("%s, ", stop);
                    } else {
                        printf("%s\n", stop);
                    }
                }
            }
        }
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
void addAndListS() {
    char stopName[MAXNAMES];
    int count = 0;
    
    count = readStop(stopName, count);
    if (count == 0) {
        listAllStops();
    } else if (count == 1) {
        searchStop(stopName);
    } else if (count == 2) {
        createAndInsertStop(stopName);
    }
}

/**
 * Populates the stop's name with the input from stdin.
 * When it is reading a name without spaces, 
 * if the maximum length is reached before a line break/EOF/space,
 * the function will keep reading and discarding the output.
 * Variable count is used to control the whitespacing between the 
 * arguments.
 * When count = 1 means it had read a space before the name.
 */
int readStop(char stopName[], int count) {
    int i = 0;
    char c;

    if (count == 0) {
        if ((c = getchar()) == ' ') {
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
                if (i < MAXNAMES - 1) {
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
void readStopQM(char stopName[]) {
    int i = 0;
    char c;

    while ((c = getchar()) != '"') {
        /*Ignores the rest of the input if string is full*/
        if (i < MAXNAMES - 1) {
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
void listAllStops() {
    char lines[MAXLINES][MAXNAMEL];
    int i, num_lines = 0;

    for (i = 0; i < counter.numStop; i++) {
        num_lines = calculateLines(StopList[i].name, lines);
        printf("%s: %16.12f %16.12f %d\n", StopList[i].name, 
        StopList[i].latitude, StopList[i].longitude, num_lines);
    }
}

/**
 * Calculates how many lines does the stop belong to and
 * returns its number.
 */
int calculateLines(char stopName[], char lines[MAXLINES][MAXNAMEL]) {
    int i, j, n = 0;

    for (i = 0; i < counter.numLink; i++) {
        /*Checks if the origin/destination stop of the link matches the 
        stop's name given*/
        if (strcmp(LinkList[i].origin.name, stopName) == 0 || 
        strcmp(LinkList[i].destination.name, stopName) == 0) {
            if (n == 0) {
                strcpy(lines[n++], LinkList[i].line.name);
                continue;
            }
            /*Checks whether the line is already in the array*/
            for (j = 0; j < n; j++) {
                if (strcmp(lines[j], LinkList[i].line.name) == 0) {
                    j = UNSUCCESS;
                    break;
                }
            }
            if (j == UNSUCCESS) {
                continue;
            }
            strcpy(lines[n++], LinkList[i].line.name);
        }
    }
    return n;
}

/**
 * Searches the specific stop in the array of stops 
 * and prints its latitude and longitude.
 */
void searchStop(char stopName[]) {
    int id;

    id = isDuplicateStop(stopName);
    if (id == UNSUCCESS) {
        printf("%s: " STOP_INVALID, stopName);
    } else if (id >= SUCCESS) {
        printf("%16.12f %16.12f\n", StopList[id].latitude, 
        StopList[id].longitude);
    }
}

/**
 * Creates a new stop with its longitude and latitude.
 */
void createAndInsertStop(char stopName[]) {
    Stop s = {0};
    double longitude, latitude;

    scanf("%lf %lf", &latitude, &longitude);
    if (isDuplicateStop(stopName) != UNSUCCESS) {
        printf("%s: " STOP_ALREADY_EXISTS, stopName);
        return;
    }
    strcpy(s.name, stopName);
    s.longitude = longitude;
    s.latitude = latitude;
    StopList[counter.numStop] = s;
    counter.numStop++;
}

/**
 * Handles the 'l' command.
 * Creates a new link, verifies the validity of its information and 
 * updates the line with their new origin/destination.
 */
void addLink() {
    double cost, duration;
    char lineName[MAXNAMEL], stopOrig[MAXNAMES], stopDest[MAXNAMES];
    
    if (readNamesLink(lineName, stopOrig, stopDest) == UNSUCCESS) {
        return;
    }

    scanf("%lf %lf", &cost, &duration);
    if (cost < 0 || duration < 0) {
        printf(COST_OR_DURATION_INVALID);
        return;
    }

    if (doesLineHasStops(lineName) == UNSUCCESS) {
        updateEmptyLine(lineName, stopOrig, stopDest);
    } else {
        updateLine(lineName, stopOrig, stopDest);
    }
    createLink(lineName, stopOrig, stopDest, cost, duration);
}

/**
 * Reads the input from stdin of the parameters after 'l'.
 * It reads the name of the line, the stop's origin and 
 * the stop's destination and verifies if they are valid.
 */
int readNamesLink(char lineName[], char stopOrig[], char stopDest[]) {
    int count = 1;

    readLine(lineName);
    if (doesLineExist(lineName) == UNSUCCESS) {
        printf("%s: " LINE_INVALID, lineName);
        return UNSUCCESS;
    }
    /*count starts at 1 since the function readLine already skipped through
    the space before the name's stop*/
    readStop(stopOrig, count);
    readStop(stopDest, count);

    if (isDuplicateStop(stopOrig) == UNSUCCESS) {
        printf("%s: " STOP_INVALID, stopOrig);
        return UNSUCCESS;
    }
    if (isDuplicateStop(stopDest) == UNSUCCESS) {
        printf("%s: " STOP_INVALID, stopDest);
        return UNSUCCESS;
    }
    if (isLinkValid(lineName, stopOrig, stopDest) == UNSUCCESS) {
        printf(LINK_INVALID);
        return UNSUCCESS;
    }

    return SUCCESS;
}

/**
 * Updates the line with origin/destination when it doesn't have any stops.
 */
void updateEmptyLine(char lineName[], char stopOrig[], char stopDest[]) {
    int i, j;

    i = doesLineExist(lineName);
    j = isDuplicateStop(stopOrig);
    LineList[i].origin = StopList[j];
    j = isDuplicateStop(stopDest);
    LineList[i].destination = StopList[j];
}

/**
 * Updates the line (already with stops) with new origin/destination stops 
 * after a new link is added.
 */
void updateLine(char lineName[], char stopOrig[], char stopDest[]) {
    int i, j;
    i = doesLineExist(lineName);
    /*Checks if the line is already circular, thus any link added is invalid*/
    if (strcmp(LineList[i].origin.name, LineList[i].destination.name) == 0) {
        printf(LINK_INVALID);
        return;
    }
    /*Checks if the line will become circular after the new link is added*/
    if (strcmp(LineList[i].origin.name, stopDest) == 0 && 
    strcmp(LineList[i].destination.name, stopOrig) == 0) {
        j = isDuplicateStop(stopDest);
        LineList[i].destination = StopList[j];
        return;
    }
    /*Checks if the new destination equals to the line's origin stop*/
    if (strcmp(LineList[i].origin.name, stopDest) == 0) {
        j = isDuplicateStop(stopOrig);
        LineList[i].origin = StopList[j];
    }
    /*Checks if the new origin equals to the line's destination stop*/
    if (strcmp(LineList[i].destination.name, stopOrig) == 0) {
        j = isDuplicateStop(stopDest);
        LineList[i].destination = StopList[j];
    }
}

/**
 * Creates a new link with its name, its origin and 
 * destination stops, its cost and its duration.
 */
void createLink(char lineName[], char stopOrig[], char stopDest[], double cost, 
double duration) {
    Link l = {0};
    int i;

    i = doesLineExist(lineName);
    l.line = LineList[i];
    i = isDuplicateStop(stopOrig);
    l.origin = StopList[i];
    i = isDuplicateStop(stopDest);
    l.destination = StopList[i];
    l.cost = cost;
    l.duration = duration;
    LinkList[counter.numLink] = l;
    counter.numLink++;
}

/**
 * Handles the 'i' command.
 * List all intersections between lines.
 * Each row lists a stop with the number of lines in which they belong 
 * and its names in alphabetical order.
 */
void listI() {
    int i, k, n;
    char lines[MAXLINES][MAXNAMEL], stop[MAXNAMES];

    for (k = 0; k < counter.numStop; k++) {
        strcpy(stop, StopList[k].name);
        n = calculateLines(stop, lines);
        /*Order the lines in alphabetical order*/
        alphabeticalOrder(lines, n);
        /*If the stop only belongs to one line or less, it doesn't print*/
        if (n <= 1) {
            continue;
        }
        printf("%s %d:", stop, n);
        for (i = 0; i < n; i++) {
            printf(" %s", lines[i]);
            strcpy(lines[i], "\0");
        }
        putchar('\n');
    }
}

/**
 * Receives an array of char arrays and orders it in alphabetical order.
 */
void alphabeticalOrder(char lines[MAXLINES][MAXNAMEL], int n) {
    int i, j;
    char temp[MAXNAMEL];

    for (i = 0; i < n; i++) {
        for (j = 0; j < n-1-i; j++) {
            if (strcmp(lines[j], lines[j+1]) > 0) {
                strcpy(temp, lines[j]);
                strcpy(lines[j], lines[j+1]);
                strcpy(lines[j+1], temp);
            }
        }
    }
}

/**
 * Checks if the line already exists through the lines array.
 * Returns the index of the line in the array,
 * otherwise if it doesn't exist, it returns UNSUCCESS (-1).
 */
int doesLineExist(char lineName[]) {
    int i;

    for (i = 0; i < counter.numLine; i++) {
        if (strcmp(LineList[i].name, lineName) == 0) {
            return i;
        }
    }
    return UNSUCCESS;
}

/**
 * Checks if the line has any stops through the link array,
 * if yes, it returns its first index in the array,
 * otherwise, it returns UNSUCCESS (-1).
 */
int doesLineHasStops(char lineName[]) {
    int i;

    for (i = 0; i < counter.numLink; i++) {
        if (strcmp(LinkList[i].line.name, lineName) == 0) {
            return i;
        }
    }
    return UNSUCCESS;
}

/**
 * Checks if the link is valid.
 * If the new destination equals to the line's origin
 * or the new origin equals to the line's destination,
 * it returns SUCCESS (0),
 * otherwise, it returns UNSUCCESS (-1).
 */
int isLinkValid(char lineName[], char stopOrig[], char stopDest[]) {
    int i;

    if (doesLineHasStops(lineName) == UNSUCCESS) {
        return SUCCESS;
    }
    i = doesLineExist(lineName);
    if (strcmp(LineList[i].origin.name, stopDest) == 0) {
        return SUCCESS;
    }
    if (strcmp(LineList[i].destination.name, stopOrig) == 0) {
        return SUCCESS;
    }
    return UNSUCCESS;
}

/**
 * Checks if the stop already exists through the stop array.
 * If it exists, it returns the index of the list,
 * otherwise, it returns UNSUCCESS (-1).
 */
int isDuplicateStop(char stopName[]) {
    int i;

    for (i = 0; i < counter.numStop; i++) {
        if (strcmp(StopList[i].name, stopName) == 0) {
            return i;
        }
    }
    return UNSUCCESS;
}

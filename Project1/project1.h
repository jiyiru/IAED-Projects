#ifndef MAIN_H
#define MAIN_H

/*Errors*/
#define STOP_INVALID "no such stop.\n"
#define STOP_ALREADY_EXISTS "stop already exists.\n"
#define LINE_INVALID "no such line.\n"
#define LINK_INVALID "link cannot be associated with bus line.\n"
#define COST_OR_DURATION_INVALID "negative cost or duration.\n"
#define INCORRECT_SORT "incorrect sort option.\n"

/*To use to check if the order is inverse*/
#define INV "inv"
#define INVE "inve"
#define INVER "inver"
#define INVERS "invers"
#define INVERSE "inverso"

/*Constant to show the unsuccess*/
#define UNSUCCESS -1
/*Constant to show the success*/
#define SUCCESS 0

/*Constant for the number of letters in the word 'inverso' */
#define NUM_INV 7

/*Constants to define the order to print the stops of a line*/
/*Normal order*/
#define NORMAL 0
/*Inverse order*/
#define INVERSED 1

/*Constants to limit the arrays*/
/*Max length of line's name + 1 for the null terminator '\0'*/
#define MAXNAMEL 21
/*Max length of stop's name + 1 for the null terminator '\0'*/
#define MAXNAMES 51
/*Max number of lines*/
#define MAXLINES 200
/*Max number of stops*/
#define MAXSTOPS 10000
/*Max number of links*/
#define MAXLINKS 30000

typedef struct {
    int numLine, numStop, numLink; /*Counter for arrays*/
} Counter;

typedef struct {
    char name[MAXNAMES];
    double latitude;
    double longitude;
} Stop;

typedef struct {
    char name[MAXNAMEL];
    Stop origin;
    Stop destination;
} Line;

typedef struct {
    Line line;
    Stop origin;
    Stop destination;
    double cost;
    double duration;
} Link;

int handleInput();

/*Lines*/

void addAndListL();
int readLine(char lineName[]);
void listAllLines();
void createAndInsertLine(char lineName[]);
void listLine(char lineName[], int end);
void printLine(char lineName[], int inv);
void printStopIfCircular(char lineName[], char stop[], int id, int inv);
void printLineStops(char lineName[], char stop[], char lastStop[], int inv);


/*Stops*/

void addAndListS();
int readStop(char stopName[], int count);
void readStopQM(char stopName[]);
void listAllStops();
int calculateLines(char stopName[], char lines[MAXLINES][MAXNAMEL]);
void searchStop(char stopName[]);
void createAndInsertStop(char stopName[]);


/*Links*/

void addLink();
int readNamesLink(char lineName[], char stopOrig[], char stopDest[]);
void updateEmptyLine(char lineName[], char stopOrig[], char stopDest[]);
void updateLine(char lineName[], char stopOrig[], char stopDest[]);
void createLink(char lineName[], char stopOrig[], char stopDest[], double cost, 
double duration);


/*Intersections*/

void listI();

/*Auxiliary*/

void alphabeticalOrder(char lines[MAXLINES][MAXNAMEL], int n);

/*Verifications*/

int doesLineExist(char lineName[]);
int doesLineHasStops(char lineName[]);
int isLinkValid(char lineName[], char stopOrig[], char stopDest[]);
int isDuplicateStop(char stopName[]);

#endif
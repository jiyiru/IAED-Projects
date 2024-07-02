#ifndef MAIN_H
#define MAIN_H

/*Errors*/
#define STOP_INVALID "no such stop.\n"
#define STOP_ALREADY_EXISTS "stop already exists.\n"
#define LINE_INVALID "no such line.\n"
#define LINK_INVALID "link cannot be associated with bus line.\n"
#define COST_OR_DURATION_INVALID "negative cost or duration.\n"
#define INCORRECT_SORT "incorrect sort option.\n"
#define ERR_NO_MEMORY "No memory.\n"

#define STOP "stop"
#define LINK "link"
#define LINE "line"

/*To use to check if the order is inverse*/
#define INVERSE "inverso"

/*Max command size*/
#define MAX_COMMAND_SIZE 65535

/*Variables for the program state*/
#define RESTART 2
#define CONTINUE 1
#define QUIT 0

/*Constant to show the unsuccess*/
#define UNSUCCESS -1
/*Constant to show the success*/
#define SUCCESS 0

/*Constants to define the order to print the stops of a line*/
/*Normal order*/
#define NORMAL 0
/*Inverse order*/
#define INVERSED 1

#define EXIT_CODE_NO_MEMORY -1

typedef struct {
    char *name;
    double latitude;
    double longitude;
} Stop;

typedef struct {
    char *name;
    Stop origin;
    Stop destination;
    int numStops;
} Line;

typedef struct {
    Line line;
    Stop origin;
    Stop destination;
    double cost;
    double duration;
} Link;

struct node {
    void* value;
    struct node *next;
};

/* Types of nodes */

typedef struct node nodeStop;
typedef struct node nodeLine;
typedef struct node nodeLink;

/*Struct with all lists*/
typedef struct {
    nodeStop *headStop;
    nodeLine *headLine;
    nodeLink *headLink;
} lists;

int handleInput(lists *list);

/*Command 'c'*/

void addAndListL(lists *list);
void listAllLines(lists *list);
int readLine(char *lineName);
void createAndInsertLine(lists *list, char *lineName);
void listLine(lists *list, char *lineName, int end);
void printLine(lists *list, char *lineName, int inv);
void buildStopsOrder(lists *list, char *lineName, char **stops);
void moveToBegin(char **stops, char *stopName, int i);

/*Command 'p'*/

void addAndListS(lists *list);
int readStop(char *stopName, int count);
void readStopQM(char *stopName);
void listAllStops(lists *list);
int calculateLines(lists *list, char *stopName, char **lines);
void searchStop(lists *list, char *stopName);
void createAndInsertStop(lists *list, char *stopName);

/*Command 'l'*/

void addLink(lists *list);
void freeNames(char *lineName, char *stopOrig, char *stopDest);
int readNamesLink(lists *list, char *lineName, char *stopOrig, 
char *stopDest);
void updateEmptyLine(lists *list, char *lineName, char *stopOrig, 
char *stopDest);
void updateLine(lists *list, char *lineName, char *stopOrig, char *stopDest);
void createLink(lists *list, char *lineName, char *stopOrig, char *stopDest, 
double cost, double duration);

/*Command 'i'*/

void listI(lists *list);
void alphabeticalOrder(char **lines, int n);

/****Practical Test****/
/*Command 'f'*/

void commandF(lists *list);
/**********************/

/*Command 'r'*/

void removeLine(lists *list);
void deleteLinksFromLine(lists *list, char *lineName);

/*Command 'e'*/

void eliminateStop(lists *list);
void updateAfterStopDeletion(lists *list, char *stopName);
void buildLineLinkOrder(lists *list, char *lineName, nodeLink **links);
void insertBeginLink(nodeLink **links, nodeLink *x, int i);
void updateLinks(lists *list, nodeLink **links, char *stopName, 
int numLinks);
void updateLineStops(lists *list, char *lineName);
void updateNumStopsFromLine(lists *list, char *lineName);

/*Command 'a'*/

void deleteData(lists *list);

/*Conversions*/

Link* convertLink(void *value);
Line* convertLine(void *value);
Stop* convertStop(void *value);

/*Functions to insert new data into memory*/

lists* init();
void initHead(lists *list, char *type, struct node *next);
void insertList(lists *list, void *value, char *type);
void insertIntoList(lists *list, struct node *head, struct node *newNode,
char *type);
nodeLine* newLine(void *line);
nodeStop* newStop(void *stop);
nodeLink* newLink(void *link);

/*Functions to delete data from memory*/

void destroyLists(lists *list);
void destroyList(struct node *head, char *type);
void deleteItems(lists *list, struct node *head, void *value, char *type);
void destroyStops(void *stop);
void destroyLines(void *line);
void destroyLinks(void *link);

/* Verifications and gets */

nodeStop* isDuplicateStop(lists *list, char *stopName);
nodeLine* doesLineExist(lists *list, char *lineName);
int doesLineHasStops(lists *list, char *lineName);
int isLinkValid(lists *list, char *lineName, char *stopOrig, char *stopDest);
int isSortValid(char *input);
int isCostDurationValid(int cost, int duration);
int getNumStopsFromLine(lists *list, char *lineName);
int numLines(nodeLine *headLine);

/*utils*/

void freeArray(int length, char **array);
void* safe_malloc(unsigned int size);
void* safe_realloc(void *p, unsigned int size);
void clean();

#endif
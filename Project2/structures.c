/* iaed-23 - Ines Ji - ist199238 - project2 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "project2.h"


/* Conversions */

/**
 * Converts the void value in a Line struct.
 */
Line* convertLine(void *value) {
    Line* l = (Line*) value;
    return l;
}

/**
 * Converts the void value in a Link struct.
 */
Link* convertLink(void *value) {
    Link* l = (Link*) value;
    return l;
}

/**
 * Converts the void value in a Stop struct.
 */
Stop* convertStop(void *value) {
    Stop* s = (Stop*) value;
    return s;
}


/*Functions to insert new data into memory*/

/**
 * Handles the start of the memory.
 * It allocates memory for the struct with the linked lists
 * and sets their head node as NULL.
 */
lists* init() {
    lists* list = (lists*)safe_malloc(sizeof(lists));
    list->headLine = NULL;
    list->headLink = NULL;
    list->headStop = NULL;
    return list;
}

/**
 * Initiates the head of the linked list with the specified node given.
 * The head updated depends of the type given: LINE, STOP, LINK.
 */
void initHead(lists *list, char *type, struct node *next) {
    if (strcmp(type, LINE) == 0) {
        list->headLine = next;
    } else if (strcmp(type, STOP) == 0) {
        list->headStop = next;
    } else if (strcmp(type, LINK) == 0) {
        list->headLink = next;
    }
}

/**
 * Creates the node with the given value and adds it to the
 * list.
 * The list to be added is specified by the type:
 * LINE, STOP, LINK. 
 */
void insertList(lists *list, void* value, char* type) {
    if (strcmp(type, LINE) == 0) {
        insertIntoList(list, list->headLine, newLine(value), type);
    } else if (strcmp(type, STOP) == 0) {
        insertIntoList(list, list->headStop, newStop(value), type);
    } else if (strcmp(type, LINK) == 0) {
        insertIntoList(list, list->headLink, newLink(value), type);
    }
}

/**
 * Inserts the given node in the end of the the linked list.
 * The list is specified by the type:
 * LINE, STOP, LINK. 
 */
void insertIntoList(lists *list, struct node *head, struct node *newNode,
char *type) {
    struct node *x;
    if (head == NULL) {
        initHead(list, type, newNode);
        return;
    }
    for (x = head; x->next != NULL; x = x->next);
    x->next = newNode;
}

/**
 * Creates a new nodeStop with the given stop.
 */
nodeStop* newStop(void *stop) {
    nodeStop* newNodeStop = (nodeStop*)safe_malloc(sizeof(nodeStop));
    newNodeStop->value = stop;
    newNodeStop->next = NULL;
    return newNodeStop;
}

/**
 * Creates a new node line with the given line.
 */
nodeLine* newLine(void *line) {
    nodeLine* newNodeLine = (nodeLine*)safe_malloc(sizeof(nodeLine));
    newNodeLine->value = line;
    newNodeLine->next = NULL;
    return newNodeLine;
}

/**
 * Creates a new link with the given link.
 */
nodeLink* newLink(void *link) {
    nodeLink* newNodeLink = (nodeLink*)safe_malloc(sizeof(nodeLink));
    newNodeLink->value = link;
    newNodeLink->next = NULL;
    return newNodeLink;
}


/*Functions to delete data from memory*/

/**
 * Deletes the 3 linked lists of links, lines, stops 
 * from the memory. 
 */
void destroyLists(lists *list) {
    destroyList(list->headLine, LINE);
    destroyList(list->headStop, STOP);
    destroyList(list->headLink, LINK);
}

/**
 * Deletes a specified linked list from the memory.
 * The specification is made through the variable type:
 * LINK, NODE, STOP.
 */
void destroyList(struct node *head, char *type) {
    struct node *next = head;
    if (head == NULL) {
        return;
    }
    while(next != NULL) {
        struct node *aux = next;
        next = aux->next;
        if (strcmp(type, STOP) == 0) {
            destroyStops(aux->value);
        } else if (strcmp(type, LINE) == 0) {
            destroyLines(aux->value);
        } else if (strcmp(type, LINK) == 0) {
            destroyLinks(aux->value);
        }
        free(aux);
    }
}

/**
 * Deletes a specified node from the linked list.
 * The linked list chosen is specified by the char type: LINE, STOP, LINK.
 * It deletes both the node and the value.
 */
void deleteItems(lists *list, struct node *head, void *value, char *type) {
    struct node *x, *prev;
    for (x = head, prev = NULL; x != NULL; prev = x, x = x->next) {
        if (x->value == value) {
            if (x != head) {
                prev->next = x->next;
            } else {
                head = x->next;
                initHead(list, type, head);
            }
            if (strcmp(type, LINE) == 0) {
                destroyLines(x->value);
            } else if (strcmp(type, STOP) == 0) {
                destroyStops(x->value);
            } else if (strcmp(type, LINK) == 0) {
                destroyLinks(x->value);
            }
            free(x);
            return;
        }
    }
}

/**
 * Deletes the specified stop from the memory.
 */
void destroyStops(void *stop) {
    Stop* s = (Stop*) stop;
    free(s->name);
    free(s);
}

/**
 * Deletes the specified line from the memory.
 */
void destroyLines(void *line) {
    Line* l = (Line*) line;
    free(l->name);
    free(l);
}

/**
 * Deletes the specified link from the memory.
 */
void destroyLinks(void *link) {
    Link* l = (Link*) link;
    free(l);
}

#include<stdio.h>
#include <stdlib.h>

typedef struct _node
{
    struct _node* prev;
    int data;
    struct _node* next;
}NODE;

/*
 NULL
 0x01
 -----------------------
|       |       |       |
| NULL  | Value | NULL  |
 -----------------------
0x01                        0x25
 -----------------------     -----------------------
|       |       |       |   |       |       |       |
|  NULL | Value1| 0x25  |   |  NULL | Value2| NULL  |
 -----------------------     -----------------------

*/
/**
 * Boundary Conditions
 * 1.When value has to go in a first node
 * 2.When value has to go in Nth node
 * Algorithm Steps:
 * 1.Create a temp NODE ptr and insert the value and NULL in next and prev
 * 2.If incoming queue is NULL insert the created node in head node
 * 3.If incoming node is not null, traverse next node till node->next becomes
 *   NULL
 * 4.Insert the created node in node->next.
 */
void insertQ(NODE *&userQueue, int value)
{
    NODE* temp, *traverse;
    temp = (NODE*)malloc(sizeof(NODE));
    temp->next = NULL;
    temp->data = value;
    temp->prev = NULL;
    traverse = userQueue;

    if(traverse == NULL)
    {
        userQueue = temp; //Create the head node
    }
    else
    {
        while(traverse->next != NULL)
        {
            traverse = traverse->next;
        }
        traverse->next = temp;
    }

}

/*
 NULL
 0x01
 -----------------------
|       |       |       |
| NULL  | Value | NULL  |
 -----------------------
0x01                        0x25
 -----------------------     -----------------------
|       |       |       |   |       |       |       |
|  NULL | Value1| 0x25  |   |  0x01 | Value2| NULL  |
 -----------------------     -----------------------

*/
/// Boundary Conditions
/// 1.When Queue has only one node
/// 2.When Queue has N nodes
void displayQ(NODE* inQueue)
{
    if(inQueue == NULL)
        printf("Empty Queue\n");
    while(inQueue != NULL)
    {
        printf("%d ",inQueue->data);
        inQueue = inQueue->next;
    }
}

/*
 NULL
 0x01
 -----------------------
|       |       |       |
| NULL  | Value | NULL  |
 -----------------------
0x01                        0x25
 -----------------------     -----------------------
|       |       |       |   |       |       |       |
|  NULL | Value1| 0x25  |   |  0x01 | Value2| NULL  |
 -----------------------     -----------------------

*/
/**
 * @brief insertDoubleQ
 *  Boundary Conditions:
 *  1. Head Node
 *  2. Nth Node
 *  Algorithm:
 *  1. Create a *temp and *traverse node.
 *  2. Fill the user data in temp node and set next & prev as NULL
 *  3. Point traverse node to userQueue
 *  4. Check the userQueue.
 *  5. If userQueue is NULL, point the userQueue to temp node
 *  6. If userQueue has data, traverse to the next node till
 *     traverse->next becomes NULL
 *  7. Update the temp->prev with traverse
 *  8. Point traverse->next to temp
 * @param userNode
 * @param data
 */
void insertDoubleQ(NODE* &userNode, int data)
{
    NODE* temp, *traverse;
    temp = (NODE*)malloc(sizeof(NODE));
    temp->prev = NULL;
    temp->data = data;
    temp->next = NULL;
    traverse = userNode;
    if(traverse == NULL)
    {
        userNode = temp;
    }
    else
    {
        while(traverse->next != NULL)
        {
            traverse = traverse->next;
        }
        temp->prev = traverse;
        traverse->next = temp;
    }
}


/**
 * @brief displayDoubleQ
 *  Display values in doubleQ by pressing N/P keys
 *  Algorithm:
 *  1.Get the key value
 *  2.If key is n/N,then check userQueue->next != NULL and traverse to next node
 *  3.If key is p/P,then check userQueue->prev != NULL and traverse to prev node
 * @param userQueue
 */
/*
 NULL
 0x01
 -----------------------
|       |       |       |
| NULL  | Value | NULL  |
 -----------------------
0x01                        0x25
 -----------------------     -----------------------
|       |       |       |   |       |       |       |
|  NULL | Value1| 0x25  |   |  0x01 | Value2| NULL  |
 -----------------------     -----------------------

*/
int displayDoubleQ(NODE* userQueue)
{
    char key;
    if(userQueue == NULL)
    {
        printf("UserQueue is empty!\n");
        return 0;
    }
    printf("Press Q/q to quit displaying...\n");
    printf("!!! Key P/p for previous value\n");
    printf("!!! Key N/n for next value\n");
    do
    {
        scanf("%c", &key);
        //printf("%d ",key);

        if(key == 110 || key == 78) // n/N key
        {
            if(userQueue->prev == NULL)
            {
                printf("You are in head NODE\n");
                printf("--> %d\n", userQueue->data);
                userQueue = userQueue->next;
                continue;
            }
            if(userQueue->next == NULL)
            {
                printf("You are in TAIL node\n");
                printf("--> %d\n", userQueue->data);
                continue;
            }
            printf("--> %d\n", userQueue->data);
            userQueue = userQueue->next;
        }
        else if(key == 112 || key == 80) // p/P Key
        {

            if(userQueue->next == NULL)
            {
                printf("You are in tail NODE\n");
                 printf("<-- %d\n", userQueue->data);
                userQueue = userQueue->prev;
                //printf("<-- %d\n", userQueue->data);
                continue;
            }
            if(userQueue->prev == NULL)
            {
                printf("You are in HEAD node\n");
                printf("<-- %d\n", userQueue->data);
                continue;
            }
            printf("<-- %d\n", userQueue->data);
            userQueue = userQueue->prev;
            //printf("<-- %d\n", userQueue->data);
        }

    }while(key != 113 && key != 81);

    //Key Value : q =113 q = 81 n = 110 N = 78 p = 112 P =80
}

int main()
{
    NODE *queue, *doubleQueue;
    queue = doubleQueue = NULL;
    insertQ(queue, 5);
    insertQ(queue, 10);
    insertQ(queue, 15);
    insertQ(queue, 20);
    insertQ(queue, 25);
    printf("Values in the Single Queue: ");
    displayQ(queue);
    printf("\n");

    insertDoubleQ(doubleQueue, 1);
    insertDoubleQ(doubleQueue, 2);
    insertDoubleQ(doubleQueue, 3);
    insertDoubleQ(doubleQueue, 4);
    insertDoubleQ(doubleQueue, 5);
    printf("Values in the Double Queue: ");
    displayDoubleQ(doubleQueue);
    printf("\n");
}

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    double *results;
    int len;
}Data;

//a)
typedef struct tagQueueElement{
    Data data;
    struct tagQueueElement *next;
}QueueElement;

typedef struct tagQueue{
    QueueElement *head;
    QueueElement *tail;
}Queue;

//b)
void free_queue(Queue *pqueue){
    QueueElement *last=pqueue->head,*ptr;
    for (ptr=last; ptr; ptr=last){
        last=ptr->next;
        free(ptr);
    }
}

//c)
void push_copy(Queue *pqueue, const Data *pdata){
    QueueElement *new_tail=malloc(sizeof(QueueElement));
    new_tail->data=*pdata;
    pqueue->tail->next=new_tail;
    pqueue->tail=new_tail;
    new_tail->next=NULL;
}

//d)
int peek(const Queue *pqueue, Data *pdata){
    if (pqueue->head==NULL) return 0;
    pdata=&(pqueue->head->data);
    return 1;
}

//e)
int pop(Queue *pqueue, Data *pdata){
    if (pqueue->head==NULL) return -1;
    QueueElement *old_head=pqueue->head;
    pqueue->head=pqueue->head->next;
    pdata=&(old_head->data);
    free(old_head);
    if (pqueue->head==NULL) return 0;
    return 1;
}

//f)
int get_total_count(const Queue *pqueue){
    int total=0;
    for (QueueElement *ptr=pqueue->head; ptr; ptr=ptr->next){
        total+=ptr->data.len;
    }
    return total;
}

int main()
{
    printf("Hello world!\n");
    return 0;
}

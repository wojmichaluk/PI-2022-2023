#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char *name;
    char *surname;
    char pesel[12];
}Person;

//a)
typedef struct{
    Person *persons;
    int size;
    int capacity;
}PersonList;

void init_pl(PersonList *p_pl){
    p_pl->size=0;
    p_pl->capacity=4;
    Person *ptr=(Person *)malloc(sizeof(Person)*p_pl->capacity);
    if (!ptr) exit(1);
    p_pl->persons=ptr;
}

//b)
void free_pl(PersonList *p_pl){
    for (int i=0; i<p_pl->size; i++){
        free(p_pl->persons[i].name);
        free(p_pl->persons[i].surname);
    }
    free(p_pl->persons);
}

//c)
void fill_person_data(Person *p_p,const char *pesel, const char *name, const char *surname){
    p_p->name=strdup(name);
    p_p->surname=strdup(surname);
    strcpy(p_p->pesel,pesel);
}

//d)
int person_cmp(Person *fir, Person *sec){
    return strcmp(fir->pesel,sec->pesel);
}

int add(PersonList *p_pl,const char *pesel, const char *name, const char *surname){
    int flag=0,i;
    Person example;
    example.pesel=pesel;
    for (i=0; i<p_pl->size; i++){
        if (!person_cmp(&example,&(p_pl->persons[i]))){
            flag=1;
            break;
        }
    }
    if (flag){
        free(p_pl->persons[i].name);
        free(p_pl->persons[i].surname);
        p_pl->persons[i].name=strdup(name);
        p_pl->persons[i].surname=strdup(surname);
        return 1;
    }else{
        if (p_pl->size==p_pl->capacity){
            p_pl->capacity*=2;
            p_pl->persons=realloc(p_pl->persons,sizeof(Person)*p_pl->capacity);
        }
        p_pl->persons[p_pl->size].name=name;
        p_pl->persons[p_pl->size].surname=surname;
        p_pl->persons[p_pl->size++].pesel=pesel;
        return 0;
    }
}

//e)
Person *find(const PersonList *p_pl, const char *pesel){
    int flag=0,i;
    Person example;
    example.pesel=pesel;
    for (i=0; i<p_pl->size; i++){
        if (!person_cmp(&example,&(p_pl->persons[i]))){
            flag=1;
            break;
        }
    }
    if (flag){
        return &(p_pl->persons[i]);
    }else{
        return NULL;
    }
}

//f)
void merge(PersonList *p_target, const PersonList *p_src){
    for (int i=0; i<p_src->size; i++){
        add(p_target,p_src->persons[i].name,p_src->persons[i].surname,p_src->persons[i].pesel);
    }
}

int main()
{
    printf("Hello world!\n");
    return 0;
}

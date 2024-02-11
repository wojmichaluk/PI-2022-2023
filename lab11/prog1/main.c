#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1
#define FILE_OPEN_ERROR          -2
#define LIST_ERROR               -3
#define TEST 0              // 1 dla testowania, 0 dla automatycznej oceny

typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} ListElement;

typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*,const void*);

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    int         size;
    ConstDataFp dumpData;
    DataFp      freeData;
    CompareDataFp compareData;
    DataFp      modifyData;
} List;

////////////////////////////////////////////////

// Funkcje pomocnicze
void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	printf("malloc error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}
char *safe_strdup(char *str) {
	char *ptr = strdup(str);
	if (ptr) return ptr;
	printf("safe_strdup error\n");
	exit(MEMORY_ALLOCATION_ERROR);
}

////////////////////////////////////////////////////////////////
// Funkcje uniwersalne (ogolnego przeznaczenia) obs³ugi listy

// Inicjuje listê
void init_List(List *list, ConstDataFp dump, DataFp free, CompareDataFp cmp, DataFp modify) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->dumpData = dump;
    list->freeData = free;
    list->compareData = cmp;
    list->modifyData = modify;
}

// Wypisuje dane elemntów listy
void dumpList(const List* list) {
    if(list->dumpData == NULL) return;
    ListElement *i;
    for(i=list->head; i!=0; i=i->next) list->dumpData(i->data);
}

// Zwalnia pamiêæ elementów listy i danych
void freeList(List *list) {
    ListElement *current = 0;
    current = list->head;
    while(current) {
        ListElement *todelete = current;
        current = current->next;
        if(list->freeData) list->freeData(todelete->data); // zwolnienie pamiêci struktury data
        free(todelete);              // zwolnienie pamiêci elementu
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Uniwersalne funkcje obs³ugi elementów listy

// Dodaje element na pocz¹tku listy
void pushFront(List *list, void *data) {
    ListElement *element=(ListElement *)safe_malloc(sizeof(ListElement));
    element->data=data;
    element->next=list->head;
    list->head=element;
    if(!list->tail)list->tail=list->head;
    list->size++;
}

// Dodaje element na koñcu listy
void pushBack(List *list, void *data) {
    ListElement *element=(ListElement *)safe_malloc(sizeof(ListElement));
    element->data=data;
    element->next=NULL;
    if(list->tail)list->tail->next=element;
    list->tail=element;
    if(!list->head)list->head=list->tail;
    list->size++;
}

// Usuwa pierwszy element listy
void popFront(List *list) {
    if(list->head==NULL) return;
    ListElement *toDel=list->head;
    list->head=list->head->next;
    free(toDel);
    if(!list->head) list->tail=NULL;
    list->size--;
}

// Odwraca kolejnoœæ elementów listy
void reverse(List *list) {
    if (list->size<2) return;
    ListElement *iter=list->head,*tmp=iter->next,*ptr;
    while(tmp){
        ptr=tmp->next;
        tmp->next=iter;
        iter=tmp;
        tmp=ptr;
    }
    list->tail=list->head;
    list->head=iter;
    list->tail->next=NULL;
}

// Funkcje pomocnicze dla list uporzadkowanych
// (porz¹dek wyznacza funkcja wskazana polem compareData)

// Zwraca element w posortowanej liœcie, a je¿eli nie ma, to element poprzedzaj¹cy (nie ma, to NULL)
ListElement* findInsertionPoint(const List *list, ListElement *element) {
    ListElement *insertionPoint=NULL;
    ListElement *current;
    for(current=list->head; current; current=current->next){
        if(list->compareData(current->data,element->data)<=0)
            insertionPoint=current;
        else break;
    }
    return insertionPoint;
}


// Drugi parametr przekazuje do funkcji adres przydzielonej dla "nowych" danych pamiêci.
// Je¿eli takie dane s¹ ju¿ w elemencie listy, to dane tego elementu s¹ modyfikowane
// funkcj¹ wskazan¹ w polu modifyData, a pamiêæ "nowych" danych jest zwalniana.
// Je¿eli w liœcie takiego elementu nie ma, to nowy element jest tworzony
// i dopisany do listy zachowuj¹c uporz¹dkowanie listy.
void insertInOrder(List *list, void *a) {
    ListElement *example=(ListElement *)safe_malloc(sizeof(ListElement));
    example->data=a;
    ListElement *ptr=findInsertionPoint(list,example);
    if (ptr==NULL) pushFront(list,example->data);
    else if (list->compareData(ptr->data,example->data)==0){
        if (list->modifyData) list->modifyData(ptr->data);
        list->freeData(example->data);
        free(example);
    }
    else{
        example->next=ptr->next;
        ptr->next=example;
    }
    list->size++;
}


/////////////////// Funkcje do zadania 11.1.2

// Dodaje element na koñcu listy bez korzystania z pola tail
void pushBack_v0(List *list, void *data) {
    ListElement *element=(ListElement *)safe_malloc(sizeof(ListElement));
    element->data=data;
    element->next=NULL;
    if (!list->head) list->head=list->tail=element;
    else{
        ListElement *ptr;
        for (ptr=list->head; ptr->next; ptr=ptr->next);
        ptr->next=element;
    }
    list->tail=element;
    list->size++;
}


// Funkcje warstwy obs³ugi danych wskazywanych w polach struktury List


//////  Dla zadania 11.1.1 i 11.1.2
// 1. Dana jest liczb¹ typu int

typedef int DataInt;

void dump_int(const void *d) {
    printf("%d ",*(const int *)d);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    return *(const int *)a-*(const int *)b;
}

// Przydziela pamiêæ i zapisuje w niej dan¹ o wartoœci v
void *create_data_int(int v) {
    void *ptr=safe_malloc(sizeof(int));
    memcpy(ptr,(void *)&v,sizeof(int));
    return ptr;
}

//////  Dla zadania 11.1.3 i 11.1.4
// 2. Dana jest zapisana w  strukturze typu DataWord

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    const DataWord *fir=(const DataWord *)d;
    printf("%s ",fir->word);
}

void free_word(void *d) {
    DataWord *ptr=(DataWord *)d;
    free(ptr->word);
    free(ptr);
}

int cmp_word_alphabet(const void *a, const void *b) {
    const DataWord *fir=(const DataWord *)a;
    const DataWord *sec=(const DataWord *)b;
    return strcasecmp(fir->word,sec->word);
}

int cmp_word_counter(const void *a, const void *b) {
    const DataWord *f=(const DataWord *)a;
    const DataWord *s=(const DataWord *)b;
    return f->counter-s->counter;
}

void modify_word(void *a) {
    DataWord *ptr=(DataWord *)a;
    ptr->counter+=1;
}

// Wypisz dane elementów spe³niaj¹cych warunek równoœci sprawdzany funkcj¹
// wskazywan¹ w polu compareData nag³ówka listy
void dumpList_word_if(List *plist, int n) {
    DataWord cmp;
    cmp.counter=n;
    for (ListElement *ptr=plist->head; ptr; ptr=ptr->next){
        if (plist->compareData(ptr->data,&cmp)==0){
            char *p=((DataWord *)ptr->data)->word;
            for(int i=0; p[i]; i++) p[i]=tolower(p[i]);
            plist->dumpData(ptr->data);
        }
    }
}

// Przydziela pamiêæ dla ³añcucha string i struktury typu DataWord.
// Do przydzielonej pamiêci wpisuje odpowiednie dane.
// Zwraca adres struktury.
void *create_data_word(char *string, int counter) {
    DataWord *ptr=(DataWord *)safe_malloc(sizeof(DataWord));
    ptr->word=safe_strdup(string);
    ptr->counter=counter;
    return (void *)ptr;
}

//////////////////////////////////////////////////

/////////////////// Funkcje pomocnicze dla zadan 11.1.3 i 11.1.4

// Wyrazy ze strumienia wejœciowego zapisuje w elementach listy
// w kolejnoœci okreœlonej ostatnim parametrem funkcji (cmp)
// - adresem funkcji porównuj¹cej typu CompareDataFp. Adres zerowy oznacza,
// ¿e kolejnoœæ elementów listy jest zgodna z kolejnoœci¹
void stream_to_list(List *list, FILE *stream, CompareDataFp cmp) {
    DataWord *data;
    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-;";
    if(cmp) list->compareData = cmp;
    while(fgets(buf,BUFFER_SIZE,stream)) {
        p = strtok(buf, delimits);
        while(p) {
            data = create_data_word(p, 1);
            if(cmp) insertInOrder(list,data);
            else    pushBack(list, data);
            p = strtok(NULL,delimits);
        }
    }
}

void list_test(List *plist, int n) {
	char op[2];
	int v;
	ListElement element, *pelement;
	DataInt data;
	for (int i = 0; i < n; ++i) {
        if(TEST) printf("Wpisz kod polecenia ");
		scanf("%s", op);
		switch (op[0]) {
			case 'f': // dodaj na pocz¹tku listy
                if(TEST) printf("Wpisz wartoœæ ");
				scanf("%d", &v);
				pushFront(plist,create_data_int(v));
				break;
			case 'b': // dodaj na koñcu listy
                if(TEST) printf("Wpisz wartoœæ ");
				scanf("%d", &v);
				pushBack(plist,create_data_int(v));
				break;
			case 'd': // usuñ pierwszy element
			    popFront(plist);
				break;
            case 'r': // odwróæ kolejnoœæ
                reverse(plist);
                break;
			case 'i': // dodaj wg porz¹dku rosn¹cego (int) lub alfabetycznego (char),
			          // je¿eli element ju¿ istnieje, to akcja zale¿na od typu danych
                if(TEST) printf("Wpisz wartoœæ ");
				scanf("%d",&v);
				insertInOrder(plist,create_data_int(v));
                break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
}

int main(void) {
    int to_do, n, size;
    char buf[257];
    clock_t t_start, t_end;
    double t_total;
    FILE *stream;
    List list;
    ListElement *head;

	if(TEST) printf("Wpisz nr zadania ");
	scanf ("%d", &to_do);
	switch (to_do) {
case 1:
	    if(TEST) printf("Wpisz liczbê poleceñ ");
        scanf("%d",&n);
        head = NULL;
        init_List(&list, dump_int, free_int, cmp_int, NULL);
//        list_test(&list, n, int (cmp)(const void*, const void*))
        list_test(&list, n);
        dumpList(&list);
        freeList(&list);
        break;
case 2:
        if(TEST) printf("Wpisz liczbê elementów ");
        scanf("%d",&size);
        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack_v0(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back bez tail.  Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushFront(&list,create_data_int(i));
        reverse(&list);
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Front + revers. Czas = %g s.\n",size,t_total);
        freeList(&list);

        init_List(&list, dump_int, free_int, cmp_int, NULL);
        t_start = clock();
        for(int i=0; i<size; ++i) pushBack(&list,create_data_int(i));
        t_end = clock();
        t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
        printf("n = %d. Back z tail.    Czas = %g s.\n",size,t_total);
        freeList(&list);
        break;
case 3:      // wypisz wszystkie s³owa wczytane do elementów listy
        init_List(&list,dump_word,free_word,NULL,NULL);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, NULL);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_1.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, NULL);
            fclose(stream);
        }
        dumpList(&list);
        freeList(&list);
        break;
case 4:      // wypisz w porz¹dku alfabetycznym s³owa o zadanej krotnoœci
        if(TEST) printf("Wpisz krotnoœæ s³owa ");
        scanf("%d",&n);
        init_List(&list,dump_word,free_word,NULL,modify_word);
        if(TEST) {
            stream = stdin;
            printf("Wpisz linie tekstu\n");
            stream_to_list(&list, stream, cmp_word_alphabet);
            rewind(stream);
        }
        else {
            stream = fopen("Tekst_C.txt","r");
            if(stream == NULL) exit(FILE_OPEN_ERROR);
            stream_to_list(&list, stream, cmp_word_alphabet);
            fclose(stream);
        }
        list.compareData = cmp_word_counter;
        dumpList_word_if(&list,n); printf("\n");
        freeList(&list);
        break;
default:
		printf("NOTHING TO DO FOR %d\n", to_do);
	}
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->compare_data=compare_data;
    p_table->create_data=create_data;
    p_table->dump_data=dump_data;
    p_table->free_data=free_data;
    p_table->hash_function=hash_function;
    p_table->modify_data=modify_data;
    p_table->ht=(ht_element *)malloc(size*sizeof(ht_element));
    for (int i=0; i<size; i++){
        p_table->ht[i].next=NULL;
    }
    p_table->no_elements=0;
    p_table->size=size;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
    for (const ht_element *ptr=p_table->ht[n].next; ptr; ptr=ptr->next)
        p_table->dump_data(ptr->data);
    printf("\n");
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
    if (free_data) free_data(to_delete->data);
    free(to_delete);
}

void free_all(ht_element *ht, size_t size, DataFp free_data){
    for (int i=0; i<size; i++){
        ht_element *to_del=ht[i].next;
        for (ht_element *ptr=to_del; ptr; ptr=to_del){
            to_del=ptr->next;
            free_element(free_data,ptr);
        }
    }
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
    free_all(p_table->ht,p_table->size,p_table->free_data);
    free(p_table->ht);
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data) {
    size_t n=p_table->hash_function(data,p_table->size);
    ht_element *ptr,*last=&p_table->ht[n];
    for (ptr=p_table->ht[n].next; ptr; ptr=ptr->next){
        if (p_table->compare_data(data,ptr->data)==0)
            return last;
        last=ptr;
    }
    return NULL;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    size_t n=p_table->hash_function(*data,p_table->size);
    ht_element *ptr;
    for (ptr=p_table->ht[n].next; ptr; ptr=ptr->next)
        if (p_table->compare_data(*data,ptr->data)==0)
            return ptr;
    return NULL;
}

void rehash(hash_table *p_table);

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    ht_element *to_mod=get_element(p_table,data);
    if (to_mod!=NULL){
        if (p_table->modify_data!=NULL) p_table->modify_data(&to_mod->data);
        return;
    }
    size_t n=p_table->hash_function(*data,p_table->size);
    ht_element *old=p_table->ht[n].next;
    ht_element *new_el=(ht_element *)malloc(sizeof(ht_element));
    new_el->data=*data;
    p_table->ht[n].next=new_el;
    new_el->next=old;
    p_table->no_elements+=1;
    if (p_table->no_elements>MAX_RATE*p_table->size){
        rehash(p_table);
    }
}

void rehash(hash_table *p_table) {
    size_t old_size=p_table->size;
    p_table->size*=2;
    ht_element *old_table=p_table->ht;
    p_table->ht=(ht_element *)malloc(p_table->size*sizeof(ht_element));
    for (int i=0; i<p_table->size; i++){
        p_table->ht[i].next=NULL;
    }
    p_table->no_elements=0;
    for (int i=0; i<old_size; i++){
        for (ht_element *ptr=old_table[i].next; ptr; ptr=ptr->next){
            insert_element(p_table,&(ptr->data));
        }
    }
    free(old_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    ht_element *to_del=get_element(p_table,&data);
    if (to_del==NULL) return;
    ht_element *prev=find_previous(p_table,data);
    prev->next=to_del->next;
    free_element(p_table->free_data,to_del);
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ",data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data-b.int_data;
}

data_union create_int(void* value) {
    data_union du;
    scanf("%d",&du.int_data);
    if (value) value=&du;
    return du;
}

// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ",data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return (int)a.char_data-(int)b.char_data;
}

data_union create_char(void* value) {
    data_union du;
    scanf(" %c",&du.char_data);
    if (value) value=&du;
    return du;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord *ptr=(DataWord *)(data.ptr_data);
    printf("%s %d\n",ptr->word,ptr->counter);
}

void free_word(data_union data) {
    free(((DataWord *)data.ptr_data)->word);
    free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
    DataWord *fir=(DataWord *)(a.ptr_data);
    DataWord *sec=(DataWord *)(b.ptr_data);
    return strcmp(fir->word,sec->word);
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)(data.ptr_data);
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data) {
    DataWord *ptr=(DataWord *)(data->ptr_data);
    ptr->counter+=1;
}

data_union create_data_word(void *value) {
    char *word=strdup((char *)value);
    data_union du;
    du.ptr_data=(DataWord *)malloc(sizeof(DataWord));
    DataWord *ptr=(DataWord *)du.ptr_data;
    ptr->word=word;
    ptr->counter=1;
    return du;
}

void lower(char *word){
    int n=strlen(word);
    for (int i=0; i<n; i++){
        word[i]=tolower(word[i]);
    }
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    data_union data;
    char *p,buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-;";
    while(fgets(buf,BUFFER_SIZE,stream)) {
        p=strtok(buf,delimits);
        while(p){
            lower(p);
            data=p_table->create_data(p);
            insert_element(p_table, &data);
            p=strtok(NULL,delimits);
        }
    }
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
//		p_table->create_data(&data);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;
	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	free_table(&table);

	return 0;
}

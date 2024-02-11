#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    const int *ptr=A+row*cols+col;
    return *ptr;
}

void set(int cols, int row, int col, int *A, int value) {
    int *ptr=A+row*cols+col;
    *ptr=value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    int suma;
    for (int i=0; i<rowsA; i++){
        for (int j=0; j<colsB; j++){
            suma=0;
            for (int k=0; k<colsA; k++){
                suma+=get(colsA,i,k,A)*get(colsB,k,j,B);
            }
            set(colsB,i,j,AB,suma);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            scanf("%d",t+i*cols+j);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            printf("%d  ",*(t+i*cols+j));
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    int lines=0;
    //size_t len=32;
    char line[BUF_SIZE];
    char *to_copy;
    //getline(&line,&len,stdin);
    fgets(line,BUF_SIZE,stdin);
    while (strlen(line)-1 && !feof(stdin) && !ferror(stdin)){
        to_copy=(char *)malloc(strlen(line)+1);
        strcpy(to_copy,line);
        array[lines++]=to_copy;
        //getline(&line,&len,stdin);
        fgets(line,BUF_SIZE,stdin);
    }
    //array[lines]=0;
    return lines;
}

void write_char_line(char *array[], int n) {
    printf("%s\n",array[n]);
}

void delete_lines(char *array[]) {
    for (char **ptr=array; *ptr; ptr++){
        free(*ptr);
    }
}

int read_int_lines_cont(int *ptr_array[]) {
    int lines=0;
    size_t len=32;
    int *ptr=ptr_array[0];
    ptr_array[0]=ptr;
    char *line, *word;
    char space[]=" ";
    getline(&line,&len,stdin);
    //fgets(line,BUF_SIZE,stdin);
    while (strlen(line)-1 && !feof(stdin) && !ferror(stdin)){
        word=strtok(line,space);
        while (word!=NULL){
            *ptr=atoi(word);
            ptr++;
            word=strtok(NULL,space);
        }
        ptr_array[++lines]=ptr;
        getline(&line,&len,stdin);
        //fgets(line,BUF_SIZE,stdin);
    }
    //ptr_array[lines]=0;
    return lines;
}

void write_int_line_cont(int *ptr_array[], int n) {
    for (int *ptr=ptr_array[n]; ptr<ptr_array[n+1]; ptr++){
        printf("%d ",*ptr);
    }
    printf("\n");
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int lines=0;
    //size_t len=32;
    int tab[TAB_SIZE];
    int cnt,suma;
    char line[BUF_SIZE];
    char *word;
    char space[]=" ";
    //getline(&line,&len,stdin);
    fgets(line,BUF_SIZE,stdin);
    while (strlen(line)-1 && !feof(stdin) && !ferror(stdin)){
        cnt=suma=0;
        word=strtok(line,space);
        while (word!=NULL){
            tab[cnt++]=atoi(word);
            suma+=tab[cnt-1];
            word=strtok(NULL,space);
        }
        lines_array[lines].values=(int *)malloc(sizeof(int)*cnt);
        lines_array[lines].len=cnt;
        lines_array[lines].average=suma/cnt;
        for (int i=0; i<cnt; i++){
            *(lines_array[lines].values+i)=tab[i];
        }
        lines++;
        //getline(&line,&len,stdin);
        fgets(line,BUF_SIZE,stdin);
    }
    return lines;
}

void write_int_line(line_type lines_array[], int n) {
    for (int i=0; i<lines_array[n].len; i++){
        printf("%d ",*(lines_array[n].values+i));
    }
    printf("\n%.2f\n",lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) {
    for (int i=0; i<line_count; i++){
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type *fir=(line_type *)a;
    line_type *sec=(line_type *)b;
    double diff=fir->average-sec->average;
    if (diff<0) return -1;
    else if (diff>0) return 1;
    else return 0;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array,line_count,sizeof(line_type),cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for (int i=0; i<n_triplets; i++){
        scanf("%d %d %d",&triplet_array[i].r,&triplet_array[i].c,&triplet_array[i].v);
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *T1=(triplet *)t1;
    triplet *T2=(triplet *)t2;
    int f=T1->r-T2->r;
    if (f!=0) return f;
    return T1->c-T2->c;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array,n_triplets,sizeof(triplet),cmp_triplets);
    for (int i=0; i<rows+1; i++)
        R[i]=0;
    for (int i=0; i<n_triplets; i++){
        for (int j=triplet_array[i].r+1; j<rows+1; j++)
            R[j]+=1;
        V[i]=triplet_array[i].v;
        C[i]=triplet_array[i].c;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    int suma;
    for (int i=0; i<rows; i++){
        suma=0;
        for (int j=R[i]; j<R[i+1]; j++){
            suma+=V[j]*x[C[j]];
        }
        y[i]=suma;
    }
}

void read_vector(int *v, int n) {
    for (int i=0; i<n; i++){
        scanf("%d",v+i);
    }
}

void write_vector(int *v, int n) {
    for (int i=0; i<n; i++){
        printf("%d ",v[i]);
    }
    printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

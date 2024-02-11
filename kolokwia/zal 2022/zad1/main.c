#include <stdio.h>
#include <stdlib.h>

typedef struct _matrix{
    int rows;
    int cols;
    double **data;
}Matrix;

//mam funkcje int create_matrix()
//a)
int get(Matrix *pm, int row, int col, double *pvalue){
    if (pm==NULL || row<0 || row>=pm->rows || col>=pm->cols || col<0)
        return 0;
    *pvalue=pm->data[row][col];
    return 1;
}

int set(Matrix *pm, int row, int col, double value){
    if (pm==NULL || row<0 || row>=pm->rows || col>=pm->cols || col<0)
        return 0;
    pm->data[row][col]=value;
    return 1;
}

//b)
void create_identity_matrix(Matrix *pm, int size){
    if (size<1) return;
    if (!create_matrix(pm,size,size)) exit(1);
    for (int i=0; i<size; i++){
        pm->data[i][i]=1.0;
    }
}

//c)
double dot_prod_vect(const double *v1, const double *v2, int size){
    double dot_prod=0.;
    for (int i=0; i<size; i++){
        dot_prod+=v1[i]*v2[i];
    }
    return dot_prod;
}

//d)
void mul_rect(double *result,const Matrix *pm, const double *vect){
    for (int i=0; i<pm->rows; i++){
        result[i]=dot_prod_vect(pm->data[i],vect,pm->cols);
    }
}

//e)
void max_element_in_rows(double *maxdata, const Matrix *pm){
    for (int i=0; i<pm->rows; i++){
        maxdata[i]=pm->data[i][0];
        for (int j=1; j<pm->cols; j++){
            maxdata[i]=pm->data[i][j]>maxdata[i] ? pm->data[i][j] : maxdata[i];
        }
    }
}

int main()
{
    printf("Hello world!\n");
    return 0;
}

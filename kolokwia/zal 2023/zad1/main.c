#include <stdio.h>
#include <stdlib.h>

typedef struct _matrix{
    int rows;
    int cols;
    double **data;
}Matrix;

//a)
int create_matrix(Matrix *pmatrix, int rows, int cols){
    glob_columns=cols;
    if (rows<1 || cols<1) return 0;
    pmatrix->cols=cols;
    pmatrix->rows=rows;
    pmatrix->data=(double **)malloc((sizeof(double *)*rows));
    for (int i=0; i<rows; i++){
        pmatrix->data[i]=(double *)malloc(sizeof(double)*cols);
    }
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            pmatrix->data[i][j]=0.;
        }
    }
    return 1;
}

//b)
Matrix random_matrix(int rows, int cols){
    Matrix *pmatrix=(matrix *)malloc(sizeof(Matrix));
    if(create_matrix(pmatrix,rows,cols)){
        srand(0);
        for (int i=0; i<rows; i++){
            for (int j=0; j<cols; j++){
                pmatrix->data[i][j]=(double)rand()/RAND_MAX;
            }
        }
    }
    return *pmatrix;
}

//c)
void inplace_mul_scalar(Matrix *pm, double v){
    for (int i=0; i<pm->rows; i++){
        for (int j=0; j<pm->cols; j++){
            pmatrix->data[i][j]*=v;
        }
    }
}

//d)
Matrix copy_matrix(const Matrix *psrc){
    Matrix *pm=(Matrix *)malloc(sizeof(Matrix));
    pm->cols=psrc->cols;
    pm->rows=psrc->rows;
    pm->data=(double **)malloc((sizeof(double *)*rows));
    for (int i=0; i<rows; i++){
        pmatrix->data[i]=(double *)malloc(sizeof(double)*cols);
    }
    for (int i=0; i<pm->rows; i++){
        for (int j=0; j<pm->cols; j++){
            pm->data[i][j]=psrc->data[i][j];
        }
    }
    return *pm;
}

//e)
Matrix mul_by_scalar(const Matrix *pm, double v){
    Matrix *pmatrix=(Matrix *)malloc(sizeof(Matrix));
    *pmatrix=copy_matrix(pm);
    inplace_mul_scalar(pmatrix,v);
    return *pmatrix;
}

//f)
int compar(const void *a, const void *b){
    double *fir=(double *)a;
    double *sec=(double *)b;
    double sum1=0., sum2=0.;
    for (int i=0; i<glob_columns; i++){
        sum1+=fir[i];
        sum2+=sec[i];
    }
    if (sum1<sum2) return -1;
    else if (sum1>sum2) return 1;
    return 0;
}

static glob_columns;
qsort(M->data,M->rows,sizeof(double *), compar);

int main()
{
    printf("Hello world!\n");
    return 0;
}

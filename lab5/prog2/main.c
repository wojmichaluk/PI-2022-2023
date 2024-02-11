#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 0    // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n) {
  for(size_t i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(const double x[], size_t n) {
  for(size_t i=0; i<n; ++i) printf("%.4f ",x[i]);
  printf("\n");
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
  }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
  for(size_t i=0; i<m; ++i) {
    for(size_t j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
    printf("\n");
  }
}

//zmodyfikowana funkcja z zadania 1.
double gauss_simplified(double A[][SIZE], double b[], size_t n, double eps) {
    double det=1.0,el,prop;
    for (size_t i=0; i<n; i++){
        el=A[i][i];
        if (fabs(el)<eps) return 0.0;
        det*=el;
        for (size_t j=i+1; j<n; j++){
            prop=A[j][i]/el;
            A[j][i]=0.0;
            for (size_t k=i+1; k<n; k++)
                A[j][k]-=prop*A[i][k];
            b[j]-=prop*b[i];
        }
    }
    return det;
}

void set_perm(const double A[][SIZE], int perm[], size_t n){
    int temp;
    for (size_t i=0; i<n; i++)
        for (size_t j=0; j<n-i-1; j++)
            if (fabs(A[perm[j]][perm[j]])>fabs(A[perm[j+1]][perm[j+1]])){
                temp=perm[j];
                perm[j]=perm[j+1];
                perm[j+1]=temp;
            }
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    int temp,perm[SIZE];
    for (size_t i=0; i<n; i++) perm[i]=i;
    set_perm(A,perm,n);
    double B[SIZE][SIZE];
    double c[SIZE];
    temp=0;
    for (size_t i=0; i<n; i++){
        for (size_t j=0; j<n; j++)
            B[i][j]=A[perm[i]][j];
        c[i]=b[perm[i]];
        temp+=abs(perm[i]-i);
    }
    double suma=0.0,det=pow(-1,temp/2)*gauss_simplified(B,c,n,eps);
    if (det==0.0) return det;
    for (int i=n-1; i>=0; i--){
        for (size_t j=0; j<n-1-i; j++)
            suma-=B[i][n-j-1]*x[n-j-1];
        suma+=c[i];
        x[i]=suma/B[i][i];
        suma=0.0;
    }
    return det;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    int temp,perm[SIZE];
    for (size_t i=0; i<n; i++) perm[i]=i;
    set_perm(A,perm,n);
    double C[SIZE][SIZE];
    double D[SIZE][SIZE];
    for (size_t i=0; i<n; i++){
        for (size_t j=0; j<n; j++)
            B[i][j]=0.0;
        B[i][i]=1.0;
    }
    temp=0;
    for (size_t i=0; i<n; i++){
        for (size_t j=0; j<n; j++){
            C[i][j]=A[perm[i]][j];
            D[i][j]=B[perm[i]][j];
        }
        temp+=abs(perm[i]-i);
    }
    double det=pow(-1,temp/2)*1.0,el1,el2,prop;
    for (size_t i=0; i<n; i++){
        el1=C[i][i];
        el2=D[i][i];
        if (fabs(el1)<eps) return 0.0;
        det*=el1;
        for (size_t j=i+1; j<n; j++){
            prop=C[j][i]/el1;
            C[j][i]=0.0;
            for (size_t k=i+1; k<n; k++)
                C[j][k]-=prop*C[i][k];
            for (size_t k=0; k<n; k++)
                D[j][k]-=prop*D[i][k];
        }
    }
    for (size_t i=0; i<n; i++){
        prop=C[i][i];
        for (size_t j=0; j<n; j++){
            C[i][j]/=prop;
            D[i][j]/=prop;
        }
    }
    det*=prop/fabs(prop);
    for (int i=n-1; i>=0; i--){
        el1=C[i][i];
        el2=D[i][i];
        for (int j=i-1; j>=0; j--){
            prop=C[j][i]/el1;
            C[j][i]=0.0;
            for (int k=i-1; k>=0; k--)
                C[j][k]-=prop*C[i][k];
            for (int k=n-1; k>=0; k--)
                D[j][k]-=prop*D[i][k];
        }
    }
    for (size_t i=0; i<n; i++)
        for (size_t j=0; j<n; j++)
            B[i][j]=D[i][j];
    return det;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
    int to_do;
    size_t m,n,p,q;
    if(TEST) printf("Wpisz nr zadania ");
    scanf ("%d", &to_do);
    switch (to_do) {
    case 1:
      if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
      scanf("%d", &n);
      if(TEST) printf("Wpisz macierz A (wierszami): ");
      read_mat(A,n,n);
      if(TEST) printf("Wpisz wektor b: ");
      read_vec(b,n);
      det = gauss(A, b, x, n, eps);
      printf("%.4f\n",det);
      if(det) print_vec(x,n);
      break;
    case 2:
      if(TEST) printf("Wpisz rozmiar macierzy n = ");
      scanf("%d",&n);
      if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
      read_mat(A,n,n);
      det = matrix_inv(A,B,n,eps);
      printf("%.4f\n",det);
      if(det) print_mat(B,n,n);
      break;
    default:
      printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}

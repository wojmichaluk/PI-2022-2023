#include <stdio.h>

int main() {
    int a,b,liczba;
    a=0;
    b=1;
    printf("Podaj liczbe wyrazow ciagu do wypisania: ");
    scanf("%d",&liczba);
    if (liczba<=0){
        printf("Nieprawidlowe dane. Koncze dzialanie programu.");
        return 1;
    }
    printf("Wypisuje %d poczatkowych wyrazow:\n",liczba);
    printf("%d",a);
    int c=a+b;
    for (int i=1; i<liczba; i++){
        a=b;
        b=c;
        c=a+b;
        printf(" %d",a);
    }
    return 0;
}

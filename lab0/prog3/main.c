#include <stdio.h>

int main() {
    float liczba;
    float eps;
    printf("Podaj liczbe, z ktorej oblicze pierwiastek kwadratowy oraz dokladnosc: ");
    scanf("%f",&liczba);
    scanf("%f",&eps);
    if (liczba<0){
        printf("Wpisana liczba jest ujemna. Koncze dzialanie programu.");
        return 1;
    }
    float x0=1.0;
    float x1=(x0+liczba/x0)/2.0;
    float diff=x1-x0;
    if (diff<0) diff=-diff;
    while (diff>eps){
        x0=x1;
        x1=(x0+liczba/x0)/2.0;
        diff=x1-x0;
        if (diff<0) diff=-diff;
    }
    printf("Pierwiastek z liczby %f wynosi %f",liczba,x1);
}

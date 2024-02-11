#include <stdio.h>

int main() {
    int a,b;
    printf("Podaj dwie liczby calkowite. Oblicze ich NWD: ");
    scanf("%d",&a);
    scanf("%d",&b);
    int temp;
    while (b!=0){
        temp=b;
        b=a%b;
        a=temp;
    }
    printf("NWD tych liczb wynosi %d",a);
    return 0;
}

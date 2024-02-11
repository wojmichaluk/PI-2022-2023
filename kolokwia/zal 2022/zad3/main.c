#include <stdio.h>
#include <stdlib.h>

//a)
typedef struct car_specific{
    int max_persons;
    float engine_power;
}car_specific;

typedef struct bus_specific{
    int seats;
    int stands;
}bus_specific;

typedef struct truck_specific{
    float engine_vol;
    float axis_pressure;
}truck_specific;

typedef union specific{
    car_specific car;
    bus_specific bus;
    truck_specific truck;
}specific;

struct Vehicle{
    char *owner;
    int next_service[3];
    enum type {car,bus,truck};
    enum drive {electric,hybrid,combustion};
    specific un;
};

//b)
void new_owner(struct Vehicle *pvehicle){
    char *str;
    fgets(str,1024,stdin);
    char *old_owner=pvehicle->owner;
    pvehicle->owner=strdup(str);
    free(old_owner);
}

//c)
int is_delayed(int *serv, int *date){
    if (serv[2]>date[2]) return 0;
    else if (serv[2]<date[2]) return 1;
    else{
        if (serv[1]>date[1]) return 0;
        else if (serv[1]<date[1]) return 1;
        else{
            if (serv[0]>=date[0]) return 0;
            else return 1;
        }
    }
}

int delayed(struct Vehicle *pvehicle, int size, int *base_date, char ***pdelayed_owners){
    int delayed=0;
    for (int i=0; i<size; i++){
        if (is_delayed(pvehicle[i].next_service,base_date)){
            pdelayed_owners[delayed++]=pvehicle[i].owner;
        }
    }
    if (!delayed) pdelayed_owners=NULL;
    return delayed;
}

//d)
void print_pojazd(struct Vehicle *pvehicle){
    printf("%s\n%d.%d.%d\n%d\n%d\n",pvehicle->owner,pvehicle->next_service[0],pvehicle->next_service[1],pvehicle->next_service[2],pvehicle->type,pvehicle->drive);
    switch(pvehicle->type){
    case 0:
        printf("%d\n%f\n",pvehicle->un.car.max_persons,pvehicle->un.car.engine_power);
        break;
    case 1:
        printf("%d\n%d\n",pvehicle->un.bus.seats,pvehicle->un.bus.stands);
        break;
    case 2:
        printf("%f\n%f\n",pvehicle->un.truck.engine_vol,pvehicle->un.truck.axis_pressure);
        break;
    default: break;
    }
}

int main()
{
    printf("Hello world!\n");
    return 0;
}

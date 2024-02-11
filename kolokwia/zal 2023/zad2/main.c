#include <stdio.h>
#include <stdlib.h>

enum Vehicle_type {truck, car, v_ndef};
typedef struct{
    float load;
    short axes;
    int trailer_mass;
} Truck_data;

//a)
typedef struct{
    char vehicle_ID[15];
    int places : 3;
    int driving_wheels : 2;
    int towbar : 1;
} Car_data;

typedef union {
    Car_data cs;
    Truck_data ts;
} vehicle_union;

typedef struct {int d; int m; int y;} Date;
typedef struct{
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    vehicle_union veh_union;
}Vehicle;

//b)
int is_late(const Date a, const Date b){
    if (a.y<b.y) return 1;
    else if (a.y>b.y) return 0;
    else{
        if (a.m<b.m) return 1;
        else if (a.m>b.m) return 0;
        else{
            if (a.d<b.d) return 1;
            else return 0;
        }
    }
}

int delayed(const Vehicle *tv, int size, const Date *base_date, char ***pdelayed_owners){
    int delays=0;
    char **delayed_owners=(char **)malloc(sizeof(char *)*size);
    for (int i=0; i<size; i++){
        if is_late(tv[i].valid_date,*base_date){
            delayed_owners[delays++]=tv[i].owner;
        }
    }
    if (!delays){
        pdelayed_owners=0;
    }else{
        pdelayed_owners=&delayed_owners;
    }
    return delays;
}

//c)
void print_owners(const char **t_owners, size_t size){
    for (int i=0; i<size; i++){
        printf("%s\n",t_owners[i]);
    }
}

//d)
Vehicle *create_vehicle(void){
    Vehicle *veh=(Vehicle *)malloc(sizeof(Vehicle));
    veh->owner=0;
    veh->valid_date.d=1;
    veh->valid_date.m=1;
    veh->valid_date.y=1970;
    veh->v_type=v_ndef;
    return veh;
}

//e)
void print_v_data(const Vehicle *v){
    switch(v->v_type){
    case 0:
        printf("Truck: Za³adunek: %f, osie: %d, masa przyczepy: %d\n",v->veh_union.ts.load,v->veh_union.ts.axes,v->veh_union.ts.trailer_mass);
        break;
    case 1:
        printf("Car: ID: %s, miejsc: %d, naped na: %d kola, ",v->veh_union.cs.vehicle_ID,v->veh_union.cs.places,v->veh_union.cs.driving_wheels);
        if (v->veh_union.cs.towbar){
            printf("hak: TAK\n");
        }else{
            printf("hak: NIE\n");
        }
        break;
    case 2:
    default:
        break;
    }
}

//f)
int cmp_date(Date d1, Date d2){
    if (a.y<b.y) return -1;
    else if (a.y>b.y) return 1;
    else{
        if (a.m<b.m) return -1;
        else if (a.m>b.m) return 1;
        else{
            if (a.d<b.d) return -1;
            else if (a.d>b.d) return 1;
            else return 0;
        }
    }
}

//g)
//Vehicle Vtab[SIZE];
int cmp_dates(Vehicle *fir, Vehicle *sec){
    return cmp_date(fir->valid_date, sec->valid_date);
}

int main()
{
    printf("Hello world!\n");
    return 0;
}

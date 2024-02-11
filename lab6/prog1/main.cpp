#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

int read_relation(pair*);

void print_int_array(int*, int);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair*, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair*, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair*, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair*, int);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(pair*, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair*, int);

int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);
int get_domain(pair*, int, int*);

// Case 3:

int composition (pair*, int, pair*, int, pair*);

//Comparator for pair
int cmp_pair(const void*,const void*);

int insert_int (int *, int, int);

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    int i,check=1;
    for (i=0; i<n; i++){
        if (tab[i].first==new_pair.first && tab[i].second==new_pair.second){
            check=0;
            break;
        }
    }
    if (!check) return n;
    tab[n]=new_pair;
    return n+1;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int number,fir,sec,curr=0;
    pair temp;
    scanf("%d",&number);
    for (int i=0; i<number; i++){
        scanf("%d %d",&temp.first,&temp.second);
        curr=add_relation(relation,curr,temp);
    }
    return curr;
}

void print_int_array(int *array, int n) {
    printf("%d\n",n);
    for (int i=0; i<n; i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}

int is_reflexive(pair* rel, int n){
    int i,j,elem,check;
    for (i=0; i<n; i++){
        elem=rel[i].first;
        check=0;
        for (j=0; j<n; j++){
            if (rel[j].second==elem){
                check=1;
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;
}

int is_irreflexive(pair* rel, int n){
    return 1-is_reflexive(rel,n);
}

int is_symmetric(pair* rel, int n){
    int i,j,fir,sec,check;
    for (i=0; i<n; i++){
        fir=rel[i].first;
        sec=rel[i].second;
        check=0;
        for (j=0; j<n; j++){
            if (rel[j].first==sec && rel[j].second==fir){
                check=1;
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;
}

int is_antisymmetric(pair* rel, int n){
    int i,j,fir,sec,check;
    for (i=0; i<n; i++){
        fir=rel[i].first;
        sec=rel[i].second;
        check=1;
        for (j=0; j<n; j++){
            if (rel[j].first==sec && rel[j].second==fir){
                if (fir==sec) check=1;
                else check=0;
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;
}

int is_asymmetric(pair* rel, int n){
    int check1=is_antisymmetric(rel,n);
    int check2=is_irreflexive(rel,n);
    return check1*check2;
}

int is_transitive(pair* rel, int n){
    int i,j,k,fir,sec,third,check;
    for (i=0; i<n; i++){
        fir=rel[i].first;
        sec=rel[i].second;
        check=1;
        for (j=0; j<n; j++){
            if (rel[j].first==sec){
                third=rel[j].second;
                check=0;
                for (k=0; k<n; k++){
                    if (rel[k].first==fir && rel[k].second==third){
                        check=1;
                        break;
                    }
                }
                break;
            }
        }
        if (!check) return 0;
    }
    return 1;
}

int is_partial_order(pair* rel, int n){
    return is_reflexive(rel,n)*is_antisymmetric(rel,n)*is_transitive(rel,n);
}

int is_total_order(pair* rel, int n){
    return is_partial_order(rel,n)*is_connected(rel,n);
}

int is_connected(pair* rel, int n){
    int i,j,k,check,elem1,elem2,tmp_domain[n];
    int cnt=get_domain(rel,n,tmp_domain);
    for (i=0; i<cnt; i++){
        for (j=0; j<cnt; j++){
            elem1=tmp_domain[i];
            elem2=tmp_domain[j];
            check=0;
            for (k=0; k<n; k++){
                if ((elem1==rel[k].first && elem2==rel[k].second) || (elem1==rel[k].second && elem2==rel[k].first)){
                    check=1;
                    break;
                }
            }
            if (!check)
                return 0;
        }
    }
    return 1;
}



int find_max_elements(pair* rel, int n, int* maks){
    int i,j,elem,check,cnt=0;
    for (i=0; i<n; i++){
        elem=rel[i].second;
        check=1;
        for (j=0; j<n; j++){
            if (rel[j].first==elem){
                if (rel[j].second!=elem){
                    check=0;
                    break;
                }
            }
        }
        if (check){
            if (insert_int(maks,cnt,elem))
                maks[cnt++]=elem;
        }
    }
    qsort(maks,cnt,sizeof(int),cmp_pair);
    return cnt;
}

int find_min_elements(pair* rel, int n, int* mins){
    int i,j,elem,check,cnt=0;
    for (i=0; i<n; i++){
        elem=rel[i].first;
        check=1;
        for (j=0; j<n; j++){
            if (rel[j].second==elem){
                if (rel[j].first!=elem){
                    check=0;
                    break;
                }
            }
        }
        if (check){
            if (insert_int(mins,cnt,elem))
                mins[cnt++]=elem;
        }
    }
    qsort(mins,cnt,sizeof(int),cmp_pair);
    return cnt;
}

int get_domain(pair* rel, int n, int* dom){
    int elem1,elem2,i,cnt=0;
    for (i=0; i<n; i++){
        elem1=rel[i].first;
        elem2=rel[i].second;
        if (insert_int(dom,cnt,elem1))
            dom[cnt++]=elem1;
        if (!(elem1==elem2))
            if (insert_int(dom,cnt,elem2))
                dom[cnt++]=elem2;
    }
    qsort(dom,cnt,sizeof(int),cmp_pair);
    return cnt;
}

int composition (pair* rel1, int n1, pair* rel2, int n2, pair* comp){
    int i,j,el,comp_size=0;
    for (i=0; i<n1; i++){
        el=rel1[i].second;
        for (j=0; j<n2; j++){
            if (rel2[j].first==el){
                pair new_pair;
                new_pair.first=rel1[i].first;
                new_pair.second=rel2[j].second;
                comp_size=add_relation(comp,comp_size,new_pair);
            }
        }
    }
    return comp_size;
}

int cmp_pair(const void* p1,const void* p2){
    return *(int*)p1-*(int*)p2;
}

int insert_int (int *tab, int n, int new_element) {
    int check=1;
    for (int i=0; i<n; i++){
        if (tab[i]==new_element){
            check=0;
            break;
        }
    }
    return check;
}

int main(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d",&to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            //{
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size));
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int no_max_elements = find_max_elements(relation, size, max_elements);
            int no_min_elements = find_min_elements(relation, size, min_elements);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            //}
            break;
        case 3:
            size_2 = read_relation(relation_2);
            printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

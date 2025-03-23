#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

const int qmax = 1000000;
int *q;

int q1 = 0, q2 = 0;

void InQ(int x){
    q[q1] = x; q1++;
    if (q1 == qmax)
        q1 = 0;
}

int OutQ(){
    int t = q[q2]; q2++;
    if (q2 == qmax)
        q2 = 0;
    return t;
}

bool QEmpty(){
    return q1 == q2;
}

int a[6000][6000]; /// agency matrix

int ga[6000][6000], b[1000000], c[1000000], nb, nc, x[1000000], y[1000000], d[1000000], nd;
int d1[6000], dbest[6000];

char s1[10000000], s2[10000000], s3[10000000], s4[10000000];

void Convert(char s[10000000]){
    int xx = 0; nb = 0;
    for (int i = 0; i < strlen(s); i++)
        if ('0' <= s[i] && s[i] <= '9')
            xx = xx * 10 + s[i] - '0';
        else{
            b[nb] = xx;
            nb++;
            xx = 0;
        }
    b[nb] = xx;
    nb++;
}

int main(){
    q = (int*)malloc(qmax * sizeof(int));
    FILE *fin = fopen("graf4.csrrg", "rt");
    int n, nv;
    fscanf(fin, "%d", &n);
    fscanf(fin, "%s", &s1);
    fscanf(fin, "%s", &s2);
    fscanf(fin, "%s", &s3);
    fscanf(fin, "%s", &s4);
    fclose(fin);
    Convert(s2);
    nv = b[nb - 1];
    for (int i = nb - 1; i > 0; i--)
        b[i] -= b[i - 1];
    nc = nb;
    for (int i = 0; i < nb; i++)
        c[i] = b[i];
    Convert(s1);
    int k = 0;
    for (int i = 1; i <= n; i++){
        for (int j = 0; j < c[i]; j++)
            ga[i][b[k++]] = 1;
    }
   
    Convert(s4);
    nd = nb;
    for (int i = 0; i < nb; i++)
        d[i] = b[i];
    Convert(s3);
    k = 0; int h = 0, first;
    for (int i = 0; i < nb; i++){
        if (i == d[h]){
            first = b[i];
            h++;
        } else{
            x[k] = first;
            y[k] = b[i];
            k++;
        }
    }
    
    for (int i = 0; i < k; i++)
        a[x[i]][y[i]] = a[y[i]][x[i]] = 1;


    int parts = 1;
    scanf("%d", &parts);
    if(parts > nv){
        printf("Wrong input");
        return 0;
    }
    
    
    for (int i = 0; i < nv; i++)
        d1[i] = -1;

    srand(time(0));
    for (int i = 0; i < parts; i++){
        int x;
        do{
            x = rand() % nv;
        } while (d1[x] != -1);
        d1[x] = i; InQ(x);
    }
    while (!QEmpty()){
        int x = OutQ();
        for (int i = 0; i < nv; i++)
            if (a[x][i] == 1 && d1[i] == -1){
                d1[i] = d1[x];
                InQ(i);
            }
    }
    int kparts[parts];
    for (int i = 0; i < parts; i++)
        kparts[i] = 0;
    for (int i = 0; i < nv; i++)
        kparts[d1[i]]++;

    int bestmin = kparts[0], bestmax = kparts[0];
    for (int i = 1; i < parts; i++){
        if (kparts[i] < bestmin)
            bestmin = kparts[i];
        if (kparts[i] > bestmax)
            bestmax = kparts[i];
    }
    for (int i = 0; i < nv; i++)
        dbest[i] = d1[i];
        
    for (int j = 0; j < 100000; j++){
        for (int i = 0; i < nv; i++)
            d1[i] = -1;
        for (int i = 0; i < parts; i++){
            int x;
            do{
                x = rand() % nv;
            } while (d1[x] != -1);
            d1[x] = i; InQ(x);
        }
        while (!QEmpty()){
            int x = OutQ();
            for (int i = 0; i < nv; i++)
                if (a[x][i] == 1 && d1[i] == -1){
                    d1[i] = d1[x];
                    InQ(i);
                }
        }

       
        int kparts[parts];
        for (int i = 0; i < parts; i++)
            kparts[i] = 0;
        for (int i = 0; i < nv; i++)
            kparts[d1[i]]++;
        int localmin = kparts[0], localmax = kparts[0];
        for (int i = 1; i < parts; i++){
            if (kparts[i] < localmin)
                localmin = kparts[i];
            if (kparts[i] > localmax)
                localmax = kparts[i];
        }
        if (localmax - localmin < bestmax - bestmin){
            bestmax = localmax; bestmin = localmin;
            for (int i = 0; i < nv; i++)
                dbest[i] = d1[i];
        }
    }
    printf("Number of parts: %d\n", parts);

    for(int i = 0; i < parts; i++){
        int pom = 0;
        printf("Part %d: ", i + 1);
        for(int j = 0; j < nv; j++)
            if(dbest[j] == i){
                printf("%d ", j);
                pom++;
            }
        printf("\nPart %d contains %d elements\n", i+1, pom);
    }

    
    free(q);
    return 0;
}

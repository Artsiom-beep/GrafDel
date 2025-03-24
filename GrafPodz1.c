#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_QSIZE 1000000
#define MAX_SIZE 6000

int *queue;
int front = 0, rear = 0;

void enqueue(int x) {
    queue[rear++] = x;
    if (rear == MAX_QSIZE) rear = 0;
}

int dequeue() {
    int val = queue[front++];
    if (front == MAX_QSIZE) front = 0;
    return val;
}

bool isQueueEmpty() {
    return front == rear;
}

int adjacency[MAX_SIZE][MAX_SIZE];
int ga[MAX_SIZE][MAX_SIZE];

int b[MAX_QSIZE], c[MAX_QSIZE], x[MAX_QSIZE], y[MAX_QSIZE], d[MAX_QSIZE];
int d1[MAX_SIZE], bestPartition[MAX_SIZE];
int nb;

char s1[10000000], s2[10000000], s3[10000000], s4[10000000];

void convertStringToNumbers(char s[]) {
    int number = 0; nb = 0;
    char *token = strtok(s, "; \n");
    while(token != NULL) {
        b[nb++] = atoi(token);
        token = strtok(NULL, "; \n");
    }
}

int main() {
    queue = (int*)malloc(MAX_QSIZE * sizeof(int));

    char filename[100];
    printf("Enter filename: ");
    scanf("%s", filename);

    FILE *fin = fopen(filename, "rt");
    if (!fin) {
        fprintf(stderr, "Error opening file.\n");
        free(queue);
        return 1;
    }
    int iterations;
    printf("Enter number of iterations(for lower grafs more iterations & for bigger grafs less iterations & at least 100): ");
    scanf("%d", &iterations);

    int n, numVertices;
    printf("Enter number of parts: ");
    fscanf(fin, "%d\n", &n);
    fgets(s1, sizeof(s1), fin);
    fgets(s2, sizeof(s2), fin);
    fgets(s3, sizeof(s3), fin);
    fgets(s4, sizeof(s4), fin);
    fclose(fin);

    convertStringToNumbers(s2);
    numVertices = b[nb - 1];
    for (int i = nb - 1; i > 0; i--) b[i] -= b[i - 1];
    memcpy(c, b, sizeof(int) * nb);

    convertStringToNumbers(s1);
    int idx = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < c[i]; j++)
            ga[i][b[idx++]] = 1;

    convertStringToNumbers(s4);
    memcpy(d, b, sizeof(int) * nb);

    convertStringToNumbers(s3);
    idx = 0;
    for (int i = 0, h = 0, first; i < nb; i++) {
        if (i == d[h]) first = b[i], h++;
        else x[idx] = first, y[idx++] = b[i];
    }

    for (int i = 0; i < idx; i++)
        adjacency[x[i]][y[i]] = adjacency[y[i]][x[i]] = 1;

    int parts;
    printf("Enter number of parts: ");
    scanf("%d", &parts);
    if (parts > numVertices) {
        printf("Invalid input\n");
        free(queue);
        return 1;
    }

    srand(time(0));

    int bestDiff = numVertices;
    
    for (int iter = 0; iter < iterations; iter++) {
        memset(d1, -1, sizeof(int) * numVertices);
        front = rear = 0;

        for (int i = 0; i < parts; i++) {
            int vertex;
            do { vertex = rand() % numVertices; }
            while (d1[vertex] != -1);
            d1[vertex] = i;
            enqueue(vertex);
        }

        while (!isQueueEmpty()) {
            int v = dequeue();
            for (int i = 0; i < numVertices; i++) {
                if (adjacency[v][i] && d1[i] == -1) {
                    d1[i] = d1[v];
                    enqueue(i);
                }
            }
        }

        int counts[parts];
        memset(counts, 0, sizeof(counts));
        for (int i = 0; i < numVertices; i++) counts[d1[i]]++;

        int localMin = counts[0], localMax = counts[0];
        for (int i = 1; i < parts; i++) {
            if (counts[i] < localMin) localMin = counts[i];
            if (counts[i] > localMax) localMax = counts[i];
        }

        if (localMax - localMin < bestDiff) {
            bestDiff = localMax - localMin;
            memcpy(bestPartition, d1, sizeof(int) * numVertices);
        }
    }

    FILE *fout = fopen("output.txt", "wt");
    if (!fout) {
        fprintf(stderr, "Error creating output file.\n");
        free(queue);
        return 1;
    } 

    printf("Number of parts: %d\n", parts);
    fprintf(fout, "Number of parts: %d\n", parts);

    for (int i = 0; i < parts; i++) {
        int pom = 0;
        printf("Part %d: ", i + 1);
        fprintf(fout, "Part %d: ", i + 1);
        for (int j = 0; j < numVertices; j++) {
            if (bestPartition[j] == i) {
                printf("%d ", j);
                fprintf(fout, "%d ", j);
                pom++;
            }
        }
        printf("\n");
        fprintf(fout, "\n");
        printf("Part %d: consists of %d \n", i + 1, pom);
    }

    fclose(fout);


    {
        FILE *fbout = fopen("output.bin", "wb");
        if (!fbout) {
            fprintf(stderr, "Error creating binary output file.\n");
            free(queue);
            return 1;
        }
        fwrite(&parts, sizeof(parts), 1, fbout);
        fwrite(bestPartition, sizeof(int), numVertices, fbout);
        fclose(fbout);
    }

    printf("Results saved to output.txt\n");
    free(queue);
    return 0;
}

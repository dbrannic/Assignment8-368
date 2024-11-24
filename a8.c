#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 100

struct gnode {
    int label;
    int weight;
    struct gnode* next;
};

struct tnode {
    int label;
    int distance;
    int predecessor;
};

struct gnode* graph[SIZE];
int heap_index[SIZE];

void dequeue(struct tnode* arr, int n) {
    struct tnode temp = arr[n];
    arr[n] = arr[0];
    arr[0] = temp;
    n--;
    int i = 0, j;
    while ((j = 2 * i + 1) <= n) {
        if (j < n && arr[j].distance > arr[j + 1].distance)
            j = j + 1;
        if (temp.distance <= arr[j].distance) break;
        else {
            arr[i] = arr[j];
            heap_index[arr[j].label] = i;
            i = j;
        }
    }
    arr[i] = temp;
    heap_index[temp.label] = i;
}

void update(struct tnode* arr, int index) {
    struct tnode temp = arr[index];
    int i = index, parent;
    while (i > 0 && temp.distance < arr[(parent = (i - 1) / 2)].distance) {
        arr[i] = arr[parent];
        heap_index[arr[parent].label] = i;
        i = parent;
    }
    arr[i] = temp;
    heap_index[temp.label] = i;
}

void dijkstra(int source) {
    struct tnode arr[SIZE];
    int n = SIZE;

    for (int i = 0; i < SIZE; i++) {
        arr[i].label = i;
        arr[i].distance = INT_MAX;
        arr[i].predecessor = -1;
        heap_index[i] = i;
    }

    arr[0].distance = 0;
    arr[0].label = source;
    arr[source].label = 0;
    heap_index[0] = source;
    heap_index[source] = 0;

    while (n != 0) {
        dequeue(arr, n - 1);
        n--;
        int u = arr[n].label;
        struct gnode* v = graph[u];

        while (v != NULL) {
            if (heap_index[v->label] < n && 
                arr[heap_index[v->label]].distance >
                arr[heap_index[u]].distance + v->weight) {
                arr[heap_index[v->label]].distance =
                    arr[heap_index[u]].distance + v->weight;
                arr[heap_index[v->label]].predecessor = u;
                update(arr, heap_index[v->label]);
            }
            v = v->next;
        }
    }

    printf("Vertex\tDistance\tPath\n");
    for (int i = 0; i < SIZE; i++) {
        if (arr[i].distance == INT_MAX) {
            printf("%d\tINF\t-\n", i);
        } else {
            printf("%d\t%d\t", i, arr[i].distance);
            int path_node = i;
            while (path_node != -1) {
                printf("%d ", path_node);
                path_node = arr[heap_index[path_node]].predecessor;
            }
            printf("\n");
        }
    }
}

void add_edge(int src, int dest, int weight) {
    struct gnode* new_node = (struct gnode*)malloc(sizeof(struct gnode));
    new_node->label = dest;
    new_node->weight = weight;
    new_node->next = graph[src];
    graph[src] = new_node;
}

int main() {
    for (int i = 0; i < SIZE; i++) {
        graph[i] = NULL;
    }

    add_edge(0, 1, 4);
    add_edge(0, 2, 1);
    add_edge(2, 1, 2);
    add_edge(1, 3, 1);
    add_edge(2, 3, 5);

    dijkstra(0);

    for (int i = 0; i < SIZE; i++) {
        struct gnode* current = graph[i];
        while (current != NULL) {
            struct gnode* temp = current;
            current = current->next;
            free(temp);
        }
    }

    return 0;
}

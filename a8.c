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

void swap(struct tnode* arr, int i, int j) {
    struct tnode temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    heap_index[arr[i].label] = i;
    heap_index[arr[j].label] = j;
}

void heapify_down(struct tnode* arr, int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].distance < arr[smallest].distance)
        smallest = left;
    if (right < n && arr[right].distance < arr[smallest].distance)
        smallest = right;

    if (smallest != i) {
        swap(arr, i, smallest);
        heapify_down(arr, n, smallest);
    }
}

void heapify_up(struct tnode* arr, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (arr[i].distance >= arr[parent].distance)
            break;
        swap(arr, i, parent);
        i = parent;
    }
}

void dequeue(struct tnode* arr, int* n) {
    swap(arr, 0, *n - 1);
    (*n)--;
    heapify_down(arr, *n, 0);
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
    swap(arr, 0, source);

    while (n > 0) {
        int u = arr[0].label;
        dequeue(arr, &n);
        struct gnode* v = graph[u];
        while (v != NULL) {
            int v_index = heap_index[v->label];
            if (v_index < n &&
                arr[v_index].distance > arr[heap_index[u]].distance + v->weight) {
                arr[v_index].distance = arr[heap_index[u]].distance + v->weight;
                arr[v_index].predecessor = u;
                heapify_up(arr, v_index);
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

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 100 // Adjust as needed for the size of your graph

// Node structure for adjacency list
struct gnode {
    int label;
    int weight;
    struct gnode* next;
};

// Node structure for the heap
struct tnode {
    int label;
    int distance;
    int predecessor;
};

// Graph and heap structures
struct gnode* graph[SIZE];
int heap_index[SIZE];

// Dequeue function for heap (min-heap)
void dequeue(struct tnode* arr, int n) {
    struct tnode temp = arr[n]; // Exchange the root and the last node
    arr[n] = arr[0];
    arr[0] = temp;
    n--; // Decrease the heap size
    int i = 0, j;
    while ((j = 2 * i + 1) <= n) { // Left child exists?
        if (j < n && arr[j].distance > arr[j + 1].distance) // Pick the smaller child
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

// Update function for upward heapify
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

// Dijkstra's algorithm
void dijkstra(int source) {
    struct tnode arr[SIZE];
    int n = SIZE;

    // Initialize all distances to infinity and predecessors to -1
    for (int i = 0; i < SIZE; i++) {
        arr[i].label = i;
        arr[i].distance = INT_MAX;
        arr[i].predecessor = -1;
        heap_index[i] = i;
    }

    // Set the source distance to 0
    arr[0].distance = 0;
    arr[0].label = source;
    arr[source].label = 0;
    heap_index[0] = source;
    heap_index[source] = 0;

    // Dijkstra's main loop
    while (n != 0) {
        dequeue(arr, n - 1); // Remove the node with the smallest distance
        n--;
        int u = arr[n].label;
        struct gnode* v = graph[u];

        // Explore all neighbors of the current node
        while (v != NULL) {
            if (heap_index[v->label] < n && // Ensure the neighbor is still in the heap
                arr[heap_index[v->label]].distance >
                arr[heap_index[u]].distance + v->weight) {
                arr[heap_index[v->label]].distance =
                    arr[heap_index[u]].distance + v->weight;
                arr[heap_index[v->label]].predecessor = u;
                update(arr, heap_index[v->label]); // Upward heapify
            }
            v = v->next;
        }
    }

    // Print the shortest distances and paths
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

// Helper function to add an edge to the graph
void add_edge(int src, int dest, int weight) {
    struct gnode* new_node = (struct gnode*)malloc(sizeof(struct gnode));
    new_node->label = dest;
    new_node->weight = weight;
    new_node->next = graph[src];
    graph[src] = new_node;
}

int main() {
    // Initialize the graph adjacency list
    for (int i = 0; i < SIZE; i++) {
        graph[i] = NULL;
    }

    // Example: Adding edges
    add_edge(0, 1, 4);
    add_edge(0, 2, 1);
    add_edge(2, 1, 2);
    add_edge(1, 3, 1);
    add_edge(2, 3, 5);

    // Run Dijkstra's algorithm from source vertex 0
    dijkstra(0);

    // Free allocated memory
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

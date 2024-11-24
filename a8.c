#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_VERTICES 1000
#define INF INT_MAX

typedef struct {
    int target;
    int *weights;
} Edge;

typedef struct {
    int vertices;
    int period;
    int *edge_count;
    Edge **adj_list;
} Graph;

Graph *create_graph(int vertices, int period) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->vertices = vertices;
    graph->period = period;
    graph->edge_count = (int *)calloc(vertices, sizeof(int));
    graph->adj_list = (Edge **)malloc(vertices * sizeof(Edge *));

    for (int i = 0; i < vertices; i++) {
        graph->adj_list[i] = NULL;
    }
    return graph;
}

void add_edge(Graph *graph, int src, int target, int *weights) {
    int count = graph->edge_count[src];
    graph->adj_list[src] = (Edge *)realloc(graph->adj_list[src], (count + 1) * sizeof(Edge));
    graph->adj_list[src][count].target = target;
    graph->adj_list[src][count].weights = weights;
    graph->edge_count[src]++;
}

int min_distance(int *dist, bool *visited, int vertices) {
    int min = INF, min_index = -1;
    for (int i = 0; i < vertices; i++) {
        if (!visited[i] && dist[i] < min) {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

void print_path(int *parent, int target) {
    if (parent[target] == -1) {
        printf("%d", target);
        return;
    }
    print_path(parent, parent[target]);
    printf(" %d", target);
}

void shortest_path(Graph *graph, int start, int end) {
    int vertices = graph->vertices;
    int period = graph->period;

    int *dist = (int *)malloc(vertices * sizeof(int));
    bool *visited = (bool *)malloc(vertices * sizeof(bool));
    int *parent = (int *)malloc(vertices * sizeof(int));

    if (!dist || !visited || !parent) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < vertices; i++) {
        dist[i] = INF;
        visited[i] = false;
        parent[i] = -1;
    }

    dist[start] = 0;

    for (int step = 0; step < vertices - 1; step++) {
        int u = min_distance(dist, visited, vertices);
        if (u == -1) break;
        visited[u] = true;

        for (int i = 0; i < graph->edge_count[u]; i++) {
            Edge edge = graph->adj_list[u][i];
            int v = edge.target;
            int weight = edge.weights[step % period];

            if (!visited[v] && dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }

    if (dist[end] == INF) {
        printf("No path\n");
    } else {
        print_path(parent, end);
        printf("\n");
    }

    free(dist);
    free(visited);
    free(parent);
}


void free_graph(Graph *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->edge_count[i]; j++) {
            free(graph->adj_list[i][j].weights);
        }
        free(graph->adj_list[i]);
    }
    free(graph->adj_list);
    free(graph->edge_count);
    free(graph);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <graph file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int V, N;
    fscanf(file, "%d %d", &V, &N);
    Graph *graph = create_graph(V, N);

    int src, target;
    while (fscanf(file, "%d %d", &src, &target) == 2) {
        int *weights = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            fscanf(file, "%d", &weights[i]);
        }
        add_edge(graph, src, target, weights);
    }

    fclose(file);

    char query[100];
    while (fgets(query, sizeof(query), stdin)) {
        int start, end;
        if (sscanf(query, "%d %d", &start, &end) == 2) {
            shortest_path(graph, start, end);
        }
    }

    free_graph(graph);
    return 0;
}

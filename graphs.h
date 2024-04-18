#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#define MAX_NODES 100
#define MAX_STR_SIZE 100
typedef struct Edge {
    int dest;
    int cost;
} Edge;

typedef struct Node {
    char str[20];
    struct Node* next;
    Edge* edges;
    int num_edges;
    int cost;
} Node;

typedef struct DEdge {
    int dest;
    int cost;
    int value;
    float score;
} DEdge;

typedef struct DNode {
    char str[20];
    struct DNode* next;
    DEdge* edges;
    int num_edges;
    int cost;
    int value;
} DNode;

typedef struct Graph {
    Node* adj[MAX_NODES];
    Node *aux;
    int num_nodes;
    DNode* dadj[MAX_NODES];
    int comoara;
} Graph;

typedef struct QueueNode {
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* head;
    QueueNode* tail;
} Queue;

void init_graph(Graph* g);
int find_index(Graph* g, char* str);
void DFS(Graph* g, int i, int* visited);
void add_edges_from_file(char* filename, Graph* g);
void add_edge(Graph* g, char* u, char* v, int cost);
void print_graph(Graph* g);
int count_components(Graph* g);
void dfs(Graph* g, int u, int* visited);
void min_cost_per_component(Graph* g);
void dfs_min_cost(Graph* g, int u, int* visited, int* min_cost);
int compare_edges(const void* a, const void* b);
int find_src(Graph* g, int dest, int cost);
int find_parent(int* parent, int node);
Queue* create_queue();
void enqueue(Queue* q, int data);
int dequeue(Queue* q);
int is_queue_empty(Queue* q);
Graph* create_subgraph(Graph* g, int* nodes_in_component, int num_nodes_in_component);
int kruskal(Graph* g);
void kruskal_on_components(Graph* g, int* msts, int* num_components);
void swap(int *x, int *y);
void read_graph(char* filename, Graph* g);
//void add_edge_directed(Graph* g, char* u, char* v, int cost) ;
float get_score(Graph* g, char* u, char* v);
void print_graph_directed(Graph* g);
void add_node_value(Graph* g, char* str, int value);
int find_index_directed(Graph* g, char* str);
void add_edge_directed(Graph* g, char* u, char* v, int cost, int value, float score);
void calculate_scores(Graph* g);
void printdijkstra(Graph* g, int start, int end,char *filename);
//void dijkstra(Graph* g, int start, int* dist, int* previous);
void dijkstra(Graph* g, int start, float* score, int* previous);
void printPathWithScores(Graph* g, int start, int end, int* dist, int* previous);
void printPath(Graph* g, int start, int end, int*previous, char *filename);
DEdge* findEdge(DNode* node, int dest);
void free_graph(Graph* g);
void free_graph_directed(Graph** g);
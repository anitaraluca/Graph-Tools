#include "graphs.h"

void init_graph(Graph* g) { //intialize empty graph
    g->num_nodes = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        g->adj[i] = NULL;
    }
   // g->aux = NULL;
}

int find_index(Graph* g, char* str) {// find the index of a node represented with adjacency list
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->adj[i]->str, str) == 0) {
            return i;
        }
    }
    return -1;
}

void add_edge(Graph* g, char* u, char* v, int cost) {//add edges to the graph
    int index_u = find_index(g, u);
    if (index_u == -1) {
        index_u = g->num_nodes++;
        Node* p = (Node*)malloc(sizeof(Node));
        strcpy(p->str, u);
        p->edges = NULL;
        p->num_edges = 0;
        g->adj[index_u] = p;
    }

    int index_v = find_index(g, v);
    if (index_v == -1) {
        index_v = g->num_nodes++;
        Node* q = (Node*)malloc(sizeof(Node));
        strcpy(q->str, v);
        q->edges = NULL;
        q->num_edges = 0;
        g->adj[index_v] = q;
    }

    Node* node_u = g->adj[index_u];//add edge to the graph
    Edge* edge_uv = (Edge*)malloc(sizeof(Edge));
    edge_uv->dest = index_v;
    edge_uv->cost = cost;
    node_u->edges = (Edge*)realloc(node_u->edges, (node_u->num_edges + 1) * sizeof(Edge));
    node_u->edges[node_u->num_edges] = *edge_uv;
    node_u->num_edges++;

    Node* node_v = g->adj[index_v];
    Edge* edge_vu = (Edge*)malloc(sizeof(Edge));
    edge_vu->dest = index_u;
    edge_vu->cost = cost;
    node_v->edges = (Edge*)realloc(node_v->edges, (node_v->num_edges + 1) * sizeof(Edge));
    node_v->edges[node_v->num_edges] = *edge_vu;
    node_v->num_edges++;
}

void add_edges_from_file(char* filename, Graph* g) {
    FILE* fp = fopen(filename, "r");
    int N,M;
    char u[20], v[20];
    int cost;
    fscanf(fp, "%d %d", &N,&M);
    while (fscanf(fp, "%s %s %d", u, v, &cost) != EOF) {//while i can read from the file c add edges to the graph
        add_edge(g, u, v, cost);
    }
    fclose(fp);
}

void dfs(Graph* g, int u, int* visited) {//function use to move through the graph
    visited[u] = 1;
    Node* node_u = g->adj[u];
    for (int i = 0; i < node_u->num_edges; i++) {
        int v = node_u->edges[i].dest;
        if (!visited[v]) {
            dfs(g, v, visited);
        }
    }
}

int count_components(Graph* g) {// count the connected components of the graph
    int num_components = 0;
    int* visited = (int*) calloc(g->num_nodes, sizeof(int));
    for (int i = 0; i < g->num_nodes; i++) {
        if (!visited[i]) {
            num_components++;
            dfs(g, i, visited);
        }
    }
    free(visited);
    return num_components;
}

void dfs_min_cost(Graph* g, int u, int* visited, int* min_cost) {
    visited[u] = 1;
    Node* node_u = g->adj[u];
    for (int i = 0; i < node_u->num_edges; i++) {
        Edge edge = node_u->edges[i];
        int v = edge.dest;
        if (!visited[v]) {
            if (edge.cost < *min_cost) {
                *min_cost = edge.cost;
            }
            dfs_min_cost(g, v, visited, min_cost);
        }
    }
}

void min_cost_per_component(Graph* g) {
    int num_components = 0;
    int* visited = (int*)calloc(g->num_nodes, sizeof(int));
    for (int i = 0; i < g->num_nodes; i++) {
        if (!visited[i]) {
            num_components++;
            printf("%d:", num_components);
            int min_cost = INT_MAX;
            dfs_min_cost(g, i, visited, &min_cost);
            printf("%d\n", min_cost);
        }
    }
    free(visited);
}

int compare_edges(const void* a, const void* b) {//compare two edges by cost 
    Edge* edge_a = (Edge*)a;
    Edge* edge_b = (Edge*)b;
    if (edge_a->cost < edge_b->cost) {
        return -1;
    } else if (edge_a->cost > edge_b->cost) {
        return 1;
    } else {
        return 0;
    }
}

int find_src(Graph* g, int dest, int cost) {//the function returns the index of the source node where there exists an edge with the specified cost
    for (int i = 0; i < g->num_nodes; i++) {
        Node* curr_node = g->adj[i];
        for (int j = 0; j < curr_node->num_edges; j++) {
            Edge* curr_edge = &curr_node->edges[j];
            if (curr_edge->dest == dest && curr_edge->cost == cost) {
                return i;
            }
        }
    }
    return -1;
}

Queue* create_queue() {//create the queue
    Queue* q = (Queue*) malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void enqueue(Queue* q, int data) {//add new element to the end of the queue
    QueueNode* new_node = (QueueNode*) malloc(sizeof(QueueNode));
    new_node->data = data;
    new_node->next = NULL;
    if (q->head == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
}

int dequeue(Queue* q) {//removes and the first element of the queue
    int data = q->head->data;
    QueueNode* temp = q->head;
    q->head = q->head->next;
    free(temp);
    if (q->head == NULL) {
        q->tail = NULL;
    }
    return data;
}

int is_queue_empty(Queue* q) {//verifies if the queue has elements
    return q->head == NULL;
}

//the function forms a subgraph from curremt graph
Graph* create_subgraph(Graph* g, int* nodes_in_component, int num_nodes_in_component) {
    Graph* subgraph = (Graph*)malloc(sizeof(Graph));
    subgraph->num_nodes = num_nodes_in_component;
    subgraph->aux = g->aux;

    // Initialize nodes in subgraph
    for (int i = 0; i < num_nodes_in_component; i++) {
        int node_id = nodes_in_component[i];
        Node* curr_node = g->adj[node_id];
        Node* new_node = (Node*)malloc(sizeof(Node));
        strcpy(new_node->str, curr_node->str);
        new_node->num_edges = 0;
        new_node->cost = curr_node->cost;
        new_node->next = NULL;
        new_node->edges = NULL;
        subgraph->adj[i] = new_node;
    }

    // Create edges in subgraph
    for (int i = 0; i < num_nodes_in_component; i++) {
        int node_id = nodes_in_component[i];
        Node* curr_node = g->adj[node_id];
        Node* subgraph_node = subgraph->adj[i];
        for (int j = 0; j < curr_node->num_edges; j++) {
            Edge* curr_edge = &curr_node->edges[j];
            int dest_id = curr_edge->dest;
            for (int k = 0; k < num_nodes_in_component; k++) {
                if (nodes_in_component[k] == dest_id) {
                    // Found edge in component, add to subgraph
                    Edge* new_edge = (Edge*)malloc(sizeof(Edge));
                    new_edge->dest = k;
                    new_edge->cost = curr_edge->cost;
                    subgraph_node->num_edges++;
                    subgraph_node->edges = (Edge*)realloc(subgraph_node->edges, subgraph_node->num_edges * sizeof(Edge));
                    subgraph_node->edges[subgraph_node->num_edges - 1] = *new_edge;
                    break;
                }
            }
        }
    }

    return subgraph;
}

int kruskal(Graph* g) {
    // Step 1: Sort all edges in non-decreasing order of cost
    int num_edges = 0;
    Edge* edges = (Edge*)malloc(g->num_nodes * g->num_nodes * sizeof(Edge));
    for (int i = 0; i < g->num_nodes; i++) {
        Node* curr_node = g->adj[i];
        for (int j = 0; j < curr_node->num_edges; j++) {
            Edge* curr_edge = &curr_node->edges[j];
            if (curr_node->cost < curr_edge->cost) {
                edges[num_edges].dest = curr_edge->dest;
                edges[num_edges].cost = curr_edge->cost;
                num_edges++;
            }
        }
    }
    qsort(edges, num_edges, sizeof(Edge), compare_edges);

    // Step 2: Initialize a disjoint set for all nodes in the graph
    int* parent = (int*)malloc(g->num_nodes * sizeof(int));
    for (int i = 0; i < g->num_nodes; i++) {
        parent[i] = i;
    }

    // Step 3: Add edges to the minimum spanning tree until all nodes are in the same set
    int num_edges_added = 0;
    int total_cost = 0;
    for (int i = 0; i < num_edges; i++) {
        int dest = edges[i].dest;
        int src = find_src(g, dest, edges[i].cost);
        int src_parent = src;
        while (src_parent != parent[src_parent]) {
            src_parent = parent[src_parent];
        }
        int dest_parent = dest;
        while (dest_parent != parent[dest_parent]) {
            dest_parent = parent[dest_parent];
        }
        if (src_parent != dest_parent) {
            total_cost += edges[i].cost;
            parent[src_parent] = dest_parent;
            num_edges_added++;
            if (num_edges_added == g->num_nodes - 1) {
                break;
            }
        }
    }
    return total_cost;
    // Step 4: Free memory
    free(edges);
    free(parent);
}
void kruskal_on_components(Graph* g, int* msts, int* num_components) {
    int* visited = (int*)calloc(g->num_nodes, sizeof(int));
    int* parent = (int*)malloc(g->num_nodes * sizeof(int));
    for (int i = 0; i < g->num_nodes; i++) {
        if (!visited[i]) {
            int num_nodes_in_component = 0; 
            int* nodes_in_component = (int*)malloc(g->num_nodes * sizeof(int));
            Queue* q = create_queue();
            enqueue(q, i);
            visited[i] = 1;
            while (!is_queue_empty(q)) {
                int curr_node = dequeue(q);
                nodes_in_component[num_nodes_in_component] = curr_node;
                num_nodes_in_component++;
                Node* curr_adj_list = g->adj[curr_node];
                for (int j = 0; j < curr_adj_list->num_edges; j++) {
                    int dest_node = curr_adj_list->edges[j].dest;
                    if (!visited[dest_node]) {
                        visited[dest_node] = 1;
                        enqueue(q, dest_node);
                    }
                }
            }
            Graph* subgraph = create_subgraph(g, nodes_in_component, num_nodes_in_component);
            msts[*num_components] = kruskal(subgraph);
            (*num_components)++;
            free(nodes_in_component);
        }
    }
    for(int i = 0;i < count_components(g)-1;i++){
       for(int j = 0;j < count_components(g)-i-1;j++){
        if(msts[j]> msts[j+1]){
            swap(&msts[j], &msts[j+1]);
        }
       }
    }
    FILE *out = fopen("tema3.out", "w");
    fprintf(out,"%d\n", count_components(g) );
    for(int i = 0;i < count_components(g);i++){
        fprintf(out,"%d\n", msts[i]);
    }
    free(parent);
    free(visited);
}
void swap(int *x, int *y){
    int aux = *x;
    *x = *y;
    *y = aux;
}


int find_index_directed(Graph* g, char* str) {
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->dadj[i]->str, str) == 0) {
            return i;
        }
    }
    return -1;
}

void add_edge_directed(Graph* g, char* u, char* v, int cost, int value, float score) {
    int index_u = find_index_directed(g, u);
    if (index_u == -1) {
        index_u = g->num_nodes++;
        DNode* p = (DNode*)malloc(sizeof(DNode));
        strcpy(p->str, u);
        p->edges = NULL;
        p->num_edges = 0;
        p->value = value;
        g->dadj[index_u] = p;
    }

    int index_v = find_index_directed(g, v);
    if (index_v == -1) {
        index_v = g->num_nodes++;
        DNode* q = (DNode*)malloc(sizeof(DNode));
        strcpy(q->str, v);
        q->edges = NULL;
        q->num_edges = 0;
        q->value = value;
        g->dadj[index_v] = q;
    }

    DNode* node_u = g->dadj[index_u];
    DEdge* edge_uv = (DEdge*)malloc(sizeof(DEdge));
    edge_uv->dest = index_v;
    edge_uv->cost = cost;
    edge_uv->value = value;
     if (value != 0) {
        edge_uv->score = (float)(cost / value); // calculam scorul
    } else {
        edge_uv->score = 0.00;
    }
    //edge_uv->score = (float)cost / value; // calculam scorul
    node_u->edges = (DEdge*)realloc(node_u->edges, (node_u->num_edges + 1) * sizeof(DEdge));
    node_u->edges[node_u->num_edges] = *edge_uv;
    node_u->num_edges++;
}

void calculate_scores(Graph* g) {
    for (int i = 0; i < g->num_nodes; i++) {
        DNode* node = g->dadj[i];
        for (int j = 0; j < node->num_edges; j++) {
            DEdge* edge = &(node->edges[j]);
            edge->score = (float)edge->cost/g->dadj[edge->dest]->value;
        }
    }
}

void read_graph(char* filename, Graph* g) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    int N, M;
    char u[20], v[20];
    int cost, value, comoara;
    fscanf(fp, "%d %d", &N, &M);

    for (int i = 0; i < M; i++) {
        fscanf(fp, "%s %s %d", u, v, &cost);
        add_edge_directed(g, u, v, cost, 0, 0.00);  // Setăm scorul inițial la infinit
    }

    for (int i = 0; i < N; i++) {
        fscanf(fp, "%s %d", u, &value);
        int index_u = find_index_directed(g, u);
        if (index_u != -1) {
            DNode* node_u = g->dadj[index_u];
            node_u->value = value;
        }
    }

    calculate_scores(g);

    fscanf(fp, "%d", &comoara);
    g->comoara = comoara;
    fclose(fp);
}


void dijkstra(Graph* g, int start, float* score, int* previous) {
    int n = g->num_nodes;
    int* visited = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        score[i] = 100000.00;
        visited[i] = 0;
        previous[i] = -1;
    }

    score[start] = 0.0;

    for (int i = 0; i < n; i++) {
        float min_score = 100000.00;
        int u = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j] && score[j] < min_score) {
                min_score = score[j];
                u = j;
            }
        }

        if (u == -1) {
            break;
        }

        visited[u] = 1;

        for (int j = 0; j < g->dadj[u]->num_edges; j++) {
            DEdge* edge = &(g->dadj[u]->edges[j]);
            int v = edge->dest;
            float edge_score = edge->score;
            float alt = score[u] + edge_score;

            if (alt < score[v]) {
                score[v] = alt;
                previous[v] = u;
            }
        }
    }

    free(visited);
}

void printPath(Graph* g, int start, int end, int* previous, char *filename) {
    FILE *f = fopen(filename, "w");
    if (start == end) {
        printf("%s ", g->dadj[start]->str);
        return;
    }
    int* path = (int*)malloc(g->num_nodes * sizeof(int));
    int pathLength = 0;
    int current = end;
    int min_value = INT_MAX;
    while (current != -1) {
        path[pathLength++] = current;
        current = previous[current];
    }

    for (int i = pathLength - 1; i >= 0; i--) {
        fprintf(f,"%s ", g->dadj[path[i]]->str);
    }
    for (int i = pathLength - 1; i >= 0; i--) {
        int node_index = path[i];
         if(min_value > g->dadj[node_index]->value &&   g->dadj[node_index]->value != 1){
                min_value = g->dadj[node_index]->value;
         }
    }
    int sum_cost = 0;
    for (int i = pathLength - 1; i >= 0; i--) {
        int node_index = path[i];
        
        if (i > 0) {
            int next_node_index = path[i - 1];
            DEdge* edge = findEdge(g->dadj[node_index], next_node_index);
           sum_cost += edge->cost; 
        }
    }
    fprintf(f,"\n");
    fprintf(f,"%d\n", sum_cost);
    fprintf(f,"%d\n", min_value);
    fprintf(f,"%d", g->comoara/min_value);
    fclose(f);
    free(path);
}

DEdge* findEdge(DNode* node, int dest) {
    for (int i = 0; i < node->num_edges; i++) {
        DEdge* edge = &(node->edges[i]);
        if (edge->dest == dest) {
            return edge; // Return the reference of the edge 
        }
    }
    return NULL;
}

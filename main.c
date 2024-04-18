#include "graphs.h"

int getNodeByName(Graph* g, const char* name) {
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->dadj[i]->str, name) == 0) {
            return i;
        }
    }
    return -1;  // Numele nodului nu a fost găsit în graf
}

int main(int argc, char *argv[]) {
      Graph g;
      init_graph(&g);
      FILE *out = fopen("tema3.out", "w");
    if(atoi(argv[1]) == 1){
        add_edges_from_file("tema3.in", &g);
        fprintf(out,"%d\n", count_components(& g) );
        int num_components = 0;
        int* msts = (int*)malloc(g.num_nodes * sizeof(int));
        kruskal_on_components(&g,msts, &num_components);
    }
    else 
    if (atoi(argv[1]) == 2) {
        read_graph("tema3.in", &g);
        int end_node = getNodeByName(&g, "Insula");
        int start_node = getNodeByName(&g, "Corabie");

        float* scores_dus = (float*)malloc(g.num_nodes * sizeof(float));
        int* previous_dus = (int*)malloc(g.num_nodes * sizeof(int));
        dijkstra(&g, start_node, scores_dus, previous_dus);
        if (scores_dus[end_node] < 100000.00) {
            float* scores_intors = (float*)malloc(g.num_nodes * sizeof(float));
            int* previous_intors = (int*)malloc(g.num_nodes * sizeof(int));
            dijkstra(&g, end_node, scores_intors, previous_intors);
            if (scores_intors[start_node] < 100000.00) {
                printPath(&g, end_node, start_node, previous_intors, "tema3.out");
            } else {
                fprintf(out,"Echipajul nu poate transporta comoara inapoi la corabie");
            }

            free(scores_intors);
            free(previous_intors);
        } else {
            fprintf(out,"Echipajul nu poate ajunge la insula");
        }

        free(scores_dus);
        free(previous_dus);
    }
    return 0;
}
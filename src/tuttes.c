#include "tuttes.h"

#define EPSILON 1e-3




/**
 * @brief Główna funkcja implementująca algorytm Tutte'a (Barycentric Method).
 * @param graph Wskaźnik na strukturę grafu.
 * @param max_iterations Maksymalna liczba iteracji algorytmu.
 * @return EXIT_SUCCESS lub kod błędu.
 */
int tuttes_algorithm(Graph *graph, int max_iterations) {
    // 1.Stworzenie listy sąsiedstwa

    //Inicjalizacja potrzebnych narzędzi i allokacja
    int n = graph->num_nodes;
    int idx = 0;
    int dfs_res[n];
    uint **adj_list = (uint **)malloc(n * sizeof(uint *));
    int *deg = (int *)malloc(n * sizeof(int));
    Vector new_pos [n];
    for(int i = 0; i < n; i++)
        new_pos[i] = graph->nodes[i].position;
    for (int i = 0; i < n; i++){
        adj_list[i] = (uint *)malloc(n * sizeof(uint));
    }

    

    double max_change;
    double difference = 0.08 * (graph->width + graph->height) / 2.0;


    // Sprawdzenie działania allocacji
    if (adj_list == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }
    if (deg == NULL) {
        fprintf(stderr, "BŁĄD: Nie udało się zaalokować pamięci.\n");
        return ERR_MEMORY_ALLOC;
    }

    // Budowanie listy sąsiedstwa
    int result_adj_list = build_adj_list(graph, adj_list, deg);
    if (result_adj_list != 0) {
        fprintf(stderr, "BŁĄD: Nie udało się zbudować listy sąsiedztwa.\n");
        return ERR_ADJ_LIST;
    }
    // Wypisywanie listy sąsiedstwa
    // print_adj_list(graph, adj_list, deg);

    // 2.Znaleznienie dowolnego cykłu wierzchołków grafu
    find_outer_face(graph, adj_list, deg, dfs_res, &idx);

    // Wypisywanie cykłu
    // print_outer_face(dfs_res, idx);


    //3.======================Część obliczeniowa======================
    // Przygotowanie tablicy wierzchołków poligonu zewnętrznego (fixed)
    int is_fixed[n];
    for (int i = 0; i < n; i++) {
        is_fixed[i] = 0;
    }
    for (int i = 0; i < idx; i++) {
        is_fixed[dfs_res[i]] = 1;
    }
    // Ustawienie środka i rozmieszczenie punktów zewnętrznych na okręgu
    Vector center = get_center(graph);
    printf("Srodek:(%f, %f)\n", center.x, center.y);
    print_nodes_pos(graph, is_fixed);
    place_on_circle(dfs_res, graph, idx, center);
    print_nodes_pos(graph, is_fixed);

    int t = 0;
    max_change = 10.0;
    //// 4. Główna pętla obliczeniowa
    while(t < max_iterations && max_change > MINIMUM_CHANGE) {
        
        t++; 
        max_change = 0.0;
        
        //4.1.Liczymy idealne pozycje za pomocą algorytmu Tutte
        for(int i = 0; i < graph->num_nodes; i++) {
            tutte_iteration(graph,i, is_fixed, adj_list,new_pos, deg, center);

          
        }

        //4.2.Itercja po każdej parze nowych koordynat dla wierzchołków, jeśli one mają tę samą pozycję, to rozsuwamy je
                for (int a = 0;a<n; a++ ){
                    for(int b =a+1;b<n;b++){
                        if(have_same_pos(new_pos[a], new_pos[b]) && is_fixed[a] !=1 && is_fixed[b]!=1){
                            offset(new_pos, a,b, difference);
                        }
                    }
                }
        //4.3.Sprawdzenie warunku pętli while oraz zapis policzonych pozycji
        for(int i = 0; i < graph->num_nodes; i++) {
            if(is_fixed[i] != 1) {
                double change = distance(new_pos[i],graph->nodes[i].position);
                if (change >max_change){
                    max_change = change;
                }

                //Zapis końcowych pozycji do Graph
                graph->nodes[i].position.x = new_pos[i].x;
                graph->nodes[i].position.y = new_pos[i].y;

            }}
                

                
                
            }


        
        print_nodes_pos(graph, is_fixed);
   
        
    
    //4.4Zwolnienie zaalokowanej pamięci
    free_adj_list(graph, adj_list);
    free_deg(deg);


    return EXIT_SUCCESS;
}

/**
 * @brief Oblicza nową pozycję pojedynczego wierzchołka w oparciu o pozycje jego sąsiadów.
 * @param graph Wskaźnik na strukturę grafu.
 * @param iteration Indeks aktualnie przetwarzanego wierzchołka.
 * @param is_fixed Tablica określająca, czy wierzchołek jest unieruchomiony.
 * @param adj_list Lista sąsiedztwa grafu.
 * @param new_pos Tablica wynikowa dla nowych pozycji.
 * @param deg Tablica stopni wierzchołków.
 * @param center Środek obszaru rysowania (używany dla wierzchołków stopnia 1).
 * @return EXIT_SUCCESS w przypadku powodzenia.
 */

int tutte_iteration(Graph *graph,int iteration, int is_fixed[], uint** adj_list, Vector new_pos[], int deg[], Vector center){
    double sum_x = 0.0;
    double sum_y = 0.0;
    if(is_fixed[iteration] == 1) {
        new_pos[iteration] = graph->nodes[iteration].position;
   
        return EXIT_SUCCESS;
    }
        
        if(deg[iteration]==1){
             int neighbor = adj_list[iteration][0];
             //Vektor w kietrunku przeciwnym od środku
                    Vector oposite = substract_vectors(graph->nodes[neighbor].position, center);
                    
                    //Dzielimy przez dystancję między centrem a wierzchołkiem żeby otrzymać vektor kirunkowy z długością = 1
                    double len = distance(graph->nodes[neighbor].position, center);
                    if (len != 0) {
                        oposite.x /=len;
                        oposite.y /=len;
                    }
                    if (len < EPSILON) return EXIT_SUCCESS;

                    //I mnożymy przez ustawioną outer_rad żeby wierzchołek okazał się na odłegłości outer_rad od sąsiada
                    double outer_rad = 10; // nie może być < margin
                    oposite = mult_by_num(oposite, outer_rad);

                    new_pos[iteration].x = graph->nodes[neighbor].position.x + oposite.x;
                    new_pos[iteration].y = graph->nodes[neighbor].position.y + oposite.y;
        }else{
            for(int j = 0; j <deg[iteration]; j++) {
                    int neighbor = adj_list[iteration][j];
                    sum_x += graph->nodes[neighbor].position.x;
                    sum_y += graph->nodes[neighbor].position.y;
                
                        
                        
                        
                    }
                    if (deg[iteration] == 0) return EXIT_SUCCESS;
                    new_pos[iteration].x = sum_x/deg[iteration];
                    new_pos[iteration].y = sum_y/deg[iteration];
                }
            
                
                   
        
    

    return EXIT_SUCCESS;
}
/**
 * @brief Sprawdza, czy dwa punkty znajdują się w tym samym miejscu z określoną dokładnością.
 * @param a Pierwszy wektor pozycji.
 * @param b Drugi wektor pozycji.
 * @return 1 jeśli pozycje są identyczne (w granicach błędu), 0 w przeciwnym razie.
 */
int have_same_pos(Vector a, Vector b){
    return (fabs(a.x - b.x) < EPSILON && fabs(a.y - b.y) < EPSILON );

}

/**
 * @brief Rozsuwa dwa wierzchołki o zadany dystans w losowym kierunku.
 * @param new_pos Tablica z pozycjami wierzchołków.
 * @param iteration Indeks pierwszego wierzchołka.
 * @param jiteration Indeks drugiego wierzchołka.
 * @param difference Dystans, o jaki punkty mają zostać rozunięte.
 */
void offset(Vector new_pos[], int iteration,int jiteration, double difference){
    double angle = ((double)rand() / RAND_MAX) * 2.0 * M_PI;

    double dx = cos(angle) * difference / 2.0;
    double dy = sin(angle) * difference / 2.0;

    new_pos[iteration].x += dx;
    new_pos[iteration].y += dy;

    new_pos[jiteration].x -= dx;
    new_pos[jiteration].y -= dy;
}




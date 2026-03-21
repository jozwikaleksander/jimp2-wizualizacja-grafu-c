/* Kody błędów krytycznych - Program przerywa działanie */
#define EXIT_SUCCESS             0  /* Wykonanie poprawne */
#define ERR_MISSING_VALUE        1  /* Opcja -X wymaga podania wartości */
#define ERR_FILE_OPEN            2  /* Nie udało się otworzyć pliku */
#define ERR_GRAPH_LOAD           3  /* Nie udało się wczytać grafu */
#define ERR_GRAPH_NOT_CONNECTED  4  /* Podany graf nie jest spójny */
#define ERR_GRAPH_NOT_PLANAR     5  /* Podany graf nie jest planarny */
#define ERR_SAVE_TEXT_FAILED     6  /* Nie udało się zapisać do pliku tekstowego */
#define ERR_SAVE_BINARY_FAILED   7  /* Nie udało się zapisać do pliku binarnego */
#define ERR_MEMORY_ALLOC         8  /* Nie udało się zaalokować pamięci */
#define ERR_ADJ_LIST             9  /* Nie udało się zbudować listy sąsiedztwa */
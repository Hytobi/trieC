//Auteur : Plouvin Patrice 20170363

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define MAX 100
#define CSV_PATH "data/res.csv"
#define DAT_PATH "data/resultats.dat"
#define SORT                                              \
    {                                                     \
        BUBBLE, INSERTION, SELECTION, FUSION, QUICK, HEAP \
    }

typedef enum Sort
{
    BUBBLE,
    INSERTION,
    SELECTION,
    FUSION,
    QUICK,
    HEAP
} Sort; // missing
typedef struct Stat
{
    Sort sort;
    int arr_size;
    int comparaison;
    int permutation;
} Stat;

// Initialise la statistique du tri
Stat *init_stat(Sort sort, int arr_size)
{
    Stat *stat = calloc(1, sizeof(Stat));
    stat->sort = sort;
    stat->arr_size = arr_size;
    stat->comparaison = 0;
    stat->permutation = 0;
    return stat;
}

// on change la seed pour avoir des nombres aléatoires différents
void init_random()
{
    srand(time(NULL));
}

// Génère un tableau de taille arr_size avec des nombres aléatoires
int *generat_random(int size)
{
    int *tab = malloc(size * sizeof(int));
    int i;
    for (i = 0; i < size; i++)
        tab[i] = random() % 100;
    return tab;
}

// 1si le tableau est trié, 0 sinon
int tab_sort(int *tab, int size)
{
    int i;
    for (i = 0; i < size - 1; i++)
    {
        if (tab[i] >= tab[i + 1])
            return 0;
    }
    return 1;
}

// Echange deux elements d'un tableau
void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

// Les tries
// 1. Bubble sort
Stat *bubble_sort(int *tab, int size)
{
    Stat *stat = init_stat(BUBBLE, size);
    int i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size - 1; j++)
        {
            stat->comparaison++;
            if (tab[j] > tab[j + 1])
            {
                swap(&tab[j], &tab[j + 1]);
                stat->permutation += 3;
            }
        }
    }
    return stat;
}
// 2. Insertion sort
Stat *insertion_sort(int *tab, int size)
{
    Stat *stat = init_stat(INSERTION, size);
    int i, j;
    for (i = 1; i < size; i++)
    {
        j = i;
        stat->comparaison++;
        while (j > 0 && tab[j - 1] > tab[j])
        {
            swap(&tab[j - 1], &tab[j]);
            stat->permutation += 3;
            j--;
        }
    }
    return stat;
}
// 3. Selection sort
Stat *selection_sort(int *tab, int size)
{
    Stat *stat = init_stat(SELECTION, size);
    int i, j, mini;
    for (i = 0; i < size; i++)
    {
        mini = i;
        for (j = i + 1; j < size; j++)
        {
            stat->comparaison++;
            if (tab[mini] > tab[j])
                mini = j;
        }
        if (i != mini)
        {
            swap(&tab[mini], &tab[i]);
            stat->permutation += 3;
        }
    }
    return stat;
}
// 4. Fusion sort
void fusion(int *tab, int *tab1, int size1, int *tab2, int size2, Stat *stat)
{
    int i = 0, j = 0, k = 0;
    while (i < size1 && j < size2)
    {
        stat->comparaison++;
        if (tab1[i] < tab2[j])
            tab[k++] = tab1[i++];
        else
            tab[k++] = tab2[j++];
    }
    while (i < size1)
        tab[k++] = tab1[i++];
    while (j < size2)
        tab[k++] = tab2[j++];
}

int *fusion_sort(int *tab, int size, Stat *stat)
{
    if (size == 1)
        return tab;
    int size1 = size / 2;
    int size2 = size - size1;
    int *tab1 = malloc(size1 * sizeof(int));
    int *tab2 = malloc(size2 * sizeof(int));
    int i;
    for (i = 0; i < size1; i++)
        tab1[i] = tab[i];
    for (i = 0; i < size2; i++)
        tab2[i] = tab[size1 + i];
    tab1 = fusion_sort(tab1, size1, stat);
    tab2 = fusion_sort(tab2, size2, stat);
    fusion(tab, tab1, size1, tab2, size2, stat);
    return tab;
}

Stat *fusion_sort_stat(int *tab, int size)
{
    Stat *stat = init_stat(FUSION, size);
    fusion_sort(tab, size, stat);
    return stat;
}
// 5. Quick sort
void quick_sort(int *tab, int start, int end, Stat *stat)
{
    if (start >= end)
        return;
    int pivot = tab[end];
    int i = start - 1;
    int j;
    for (j = start; j < end; j++)
    {
        if (tab[j] < pivot)
        {
            i++;
            stat->permutation += 3;
            swap(&tab[i], &tab[j]);
        }
    }
    stat->permutation += 3;
    swap(&tab[i + 1], &tab[end]);
    quick_sort(tab, start, i, stat);
    quick_sort(tab, i + 2, end, stat);
}

Stat *quick_sort_stat(int *tab, int size)
{
    Stat *stat = init_stat(QUICK, size);
    quick_sort(tab, 0, size - 1, stat);
    return stat;
}

// 6. Heap sort
// transforme un tableau en tas
void heapify(int *tab, int size, int i, Stat *stat)
{
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    stat->comparaison += 2;
    if (left < size && tab[left] > tab[max])
        max = left;
    if (right < size && tab[right] > tab[max])
        max = right;
    if (max != i)
    {
        stat->permutation += 3;
        swap(&tab[i], &tab[max]);
        heapify(tab, size, max, stat);
    }
}

void build_heap(int *tab, int size, Stat *stat)
{
    int i;
    for (i = size / 2 - 1; i >= 0; i--)
        heapify(tab, size, i, stat);
}

Stat *heap_sort(int *tab, int size)
{
    Stat *stat = init_stat(HEAP, size);
    build_heap(tab, size, stat);
    int i;
    for (i = size - 1; i >= 0; i--)
    {
        stat->permutation += 3;
        swap(&tab[0], &tab[i]);
        heapify(tab, i, 0, stat);
    }
    return stat;
}

// Copie le tableau tab dans un nouveau tableau
void copytab(int *tab, int *dest, int size)
{
    int i;
    for (i = 0; i < size; i++)
        dest[i] = tab[i];
}

// Les demande a l'utilisateur
int scan_n_array()
{
    int n;
    printf("Entrez le nombre de tableau à trier : ");
    scanf("%d", &n);
    return n;
}

int scan_min_size()
{
    int size;
    printf("Entrez la taille minimum des tableaux à trier : ");
    scanf("%d", &size);
    return size;
}
int scan_n_size()
{
    int nsize;
    printf("Entrez la le nombre d'iteration : ");
    scanf("%d", &nsize);
    return nsize;
}

// proceder au trie et recuperer les statistiques
Stat *get_sort_stat(Sort sort, int *tab, int size)
{
    Stat *(*sort_f)(int *, int); // pointeur de fonction
    switch (sort)
    {
    case (BUBBLE):
        sort_f = &bubble_sort;
        break;
    case (INSERTION):
        sort_f = insertion_sort;
        break;
    case (SELECTION):
        sort_f = &selection_sort;
        break;

    case (FUSION):
        sort_f = &fusion_sort_stat;
        break;
    case (QUICK):
        sort_f = &quick_sort_stat;
        break;
    case (HEAP):
        sort_f = &heap_sort;
        break;
    default:
        printf("Erreur de saisie\n"); // si le sort n'est pas reconnu
        return NULL;
    }
    Stat *stat = sort_f(tab, size); // appel de la fonction
    return stat;
}

// Pour l'afficheage on a besoin des noms des tries
char *get_stat_sort_name(Sort sort)
{
    switch (sort)
    {
    case (BUBBLE):
        return "Bubble";
        break;
    case (INSERTION):
        return "Insertion";
        break;
    case (SELECTION):
        return "Selection";
        break;
    case (FUSION):
        return "Fusion";
        break;
    case (QUICK):
        return "Quick";
        break;
    case (HEAP):
        return "Heap";
        break;
    default:
        printf("Erreur de saisie\n");
        return NULL;
    }
}

// Reecrire les données dans un fichier csv ou dat
// Pour la premiere ligne on ecrit les noms des tries
void write_stats_headers(Stat ***stat_tab, int nb_cols, FILE *stream, int padding, char *seps)
{
    fprintf(stream, "%*s%s", padding, "Size", seps);
    for (int i = 0; i < nb_cols; i++)
    {
        char *sort_name = get_stat_sort_name(stat_tab[0][i]->sort);
        fprintf(stream, "%*s", padding, sort_name);
        if (i < nb_cols - 1)
            fprintf(stream, "%s", seps);
    }
    fprintf(stream, "\n");
}
// Pour les autres lignes on ecrit les statistiques
void write_stats_values(Stat ***stat_array, int nb_rows, int nb_cols, FILE *stream, int padding, char *seps)
{
    for (int i = 0; i < nb_rows; i++)
    {
        int array_size = stat_array[i][0]->arr_size;
        fprintf(stream, "%*d%s", padding, array_size, seps);
        for (int j = 0; j < nb_cols; j++)
        {
            int value = stat_array[i][j]->comparaison + stat_array[i][j]->permutation;
            fprintf(stream, "%*u", padding, value);
            if (j < nb_cols - 1)
                fprintf(stream, "%s", seps);
        }
        fprintf(stream, "\n");
    }
}
// Ecriture dans un fichier
void write_stats_tab(Stat ***stat_array, int nb_rows, int nb_cols, FILE *stream, int padding, char *seps)
{
    write_stats_headers(stat_array, nb_cols, stream, padding, seps);
    write_stats_values(stat_array, nb_rows, nb_cols, stream, padding, seps);
}
// Ecriture dans le terminale
void print_stats(Stat ***stat_array, int nb_rows, int nb_cols)
{
    write_stats_tab(stat_array, nb_rows, nb_cols, stdout, 15, "");
}
// Ecriture dans un fichier csv
void export_stats_to_csv(Stat ***stat_array, int nb_rows, int nb_cols, char *csv_path)
{
    FILE *csv_stream = fopen(csv_path, "w");
    write_stats_tab(stat_array, nb_rows, nb_cols, csv_stream, 10, ",");
    fclose(csv_stream);
}
// Ecriture dans un fichier dat
void export_stats_to_dat(Stat ***stat_array, int nb_rows, int nb_cols, char *dat_path)
{
    FILE *dat_stream = fopen(dat_path, "w");
    write_stats_tab(stat_array, nb_rows, nb_cols, dat_stream, 10, "\t\t");
    fclose(dat_stream);
}

// Fonction principale
int main()
{
    // Initialisation des variables
    int i, j, k, size;
    init_random();

    // Demande a l'utilisateur
    int n_arrays = scan_n_array();
    int min_size = scan_min_size();
    int n_sizes = scan_n_size();

    // On recupère les tries et le nombre de tries
    Sort sort_array[] = SORT;
    int nb_sort = sizeof(sort_array) / sizeof(sort_array[0]);

    // On a un tableau de pointeur de tableau de pointeur de Stat
    Stat ***stat_array = calloc(n_arrays * n_sizes, sizeof(Stat **));
    for (i = 0; i < n_arrays * n_sizes; i++)
    {
        stat_array[i] = calloc(nb_sort, sizeof(Stat *));
    }

    // Pour chaque tableau
    for (i = 0; i < n_sizes; i++)
    {
        size = min_size * (i + 1);                // On calcule la taille du tableau a trier
        printf("Testing for size %d...\n", size); // taille du tableau que l'on va trier
        // pour chaque iteration de tableau
        for (j = 0; j < n_arrays; j++)
        {
            int *tab = generat_random(size);        // tableau à trier
            int *inter = calloc(size, sizeof(int)); // tableau intermediaire
            for (k = 0; k < nb_sort; k++)           // pour chaque trie
            {
                Sort sort = sort_array[k];                     // type de tri
                copytab(tab, inter, size);                     // copie du tableau à trier, on tri le meme tab avec chaque trie
                Stat *stat = get_sort_stat(sort, inter, size); // statistiques du tri
                // assert(tab_sort(inter, size));                 // on verifie que le tableau est bien trié
                stat_array[(i * n_arrays) + j][k] = stat; // on stocke les stats dans le tableau de stats
            }
            free(inter); // on libere le tableau intermediaire
            free(tab);   // on libere le tableau à trier
        }
    }

    // On affiche les statistiques dans le fichier qui va bien
    int nb_rows = n_sizes * n_arrays;
    int nb_cols = nb_sort;

    print_stats(stat_array, nb_rows, nb_cols);                   // Dans le terminale
    export_stats_to_csv(stat_array, nb_rows, nb_cols, CSV_PATH); // Dans un fichier csv
    export_stats_to_dat(stat_array, nb_rows, nb_cols, DAT_PATH); // Dans un fichier dat

    return EXIT_SUCCESS;
}

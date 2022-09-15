#include "../includes/airlines.h"

/*
 * FONCTIONS UTLISÉES POUR REMPLIR LES SD
 */

// initialise une structure Airline_node

void init_airline(Airline_node **A) {
  (*A)->delay_moy = 0;
  (*A)->nb_fl = 0;
  (*A)->left = NULL;
  (*A)->right = NULL;
}

// créer le noeud d'airline à partir d'une ligne de fichier .csv

void read_airline_from_line(Airline_node **A, char *line) {
  Airline_node *new;
  new = malloc(sizeof(Airline_node));
  init_airline(&new);
  char *token = strtok(line, ",");
  strcpy(new->IATA_CODE, token);
  token = strtok(NULL, ",");
  strcpy(new->name, token);
  *A = new;
}

// ajout d'une Airline dans un arbre

void add_airline_abr(Airline_node **A, Airline_node *new) {
  if (*A == NULL) {
    *A = new;
    return;
  }
  if (strcmp((*A)->IATA_CODE, (new)->IATA_CODE) < 0)
    add_airline_abr(&((*A)->right), new);
  else
    add_airline_abr(&((*A)->left), new);
}

// rentre toutes les airlines d'un fichier dans un ABR

void fill_airline_tree(FILE *fp, Airline_node **A) {

  char *line = NULL;
  size_t len_line;
  Airline_node *new;
  new = NULL;
  getline(&line, &len_line, fp); // ignorer la légende

  while (((getline(&line, &len_line, fp)) != -1)) {
    read_airline_from_line(&new, line);
    add_airline_abr(A, new);
  }
  free(line);
}

// charche un noeud d'airline dans un ABR d'airline
Airline_node *search_airline_ABR(Airline_node *A, char *code) {
  int test = strcmp(A->IATA_CODE, code);
  if (!test)
    return A;

  if (test > 0)
    return search_airline_ABR(A->left, code);
  else
    return search_airline_ABR(A->right, code);
}

// initialise les cases d'un tableau stockant des pointeurs vers
// les airlines les plus en retard, a NULL
void init_tab_airl(Airline_node *tab[]) {
  for (int i = 0; i < 5; i++) {
    tab[i] = NULL;
  }
}

// recherche l'airline d'un tableau avec le plus petit retard
void search_min_tab(Airline_node *tab[], int *i) {
  int to_add = 0;
  int j = 0;
  while ((!to_add) && (j < 5)) {
    if (tab[j] == NULL) {
      *i = j;
      to_add = 1;
    } else if (tab[j]->delay_moy < tab[*i]->delay_moy)
      *i = j;
    j++;
  }
}

// parcours l'Arbre d'Airline et remplace les cases du tableau des
// airlines avec le plus gros retards moyen, si l'airline trouvé à un
// retard moyen supérieur aux airlines présent dans le tableau
void recup_most_del_airl(Airline_node *tab[],
                         Airline_node *Airl_tree) {
  int i = 0;
  int to_add = 0;
  if (Airl_tree == NULL)
    return;
  while ((!to_add) && (i < 5)) {
    if (tab[i] == NULL)
      to_add = 1;
    else if (tab[i]->delay_moy < Airl_tree->delay_moy) {
      search_min_tab(tab, &i);
      to_add = 1;
    } else
      i++;
  }
  if (i < 5)
    tab[i] = Airl_tree;
  recup_most_del_airl(tab, Airl_tree->left);
  recup_most_del_airl(tab, Airl_tree->right);
  return;
}

/*
 * FONCTIONS UTILISÉS POUR LES REQUETES
 */

// utilisé pour SHOW-AIRLINES
// parcours l'arbre de airl_infos d'un certain aéroport et verifie si
// la case departs de chaque noeuc est à 1, imprime cet airline dans
// ce cas
void print_airl_if_dep(Airline_node *Airl, airl_infos *A) {
  if (A == NULL)
    return;

  print_airl_if_dep(Airl, A->left);
  if (A->departs)
    print_airline(search_airline_ABR(Airl, A->IATA_CODE));
  print_airl_if_dep(Airl, A->right);
}


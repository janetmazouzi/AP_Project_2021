#include "../includes/airports.h"

// Initialise la table de hachage contenant les Airports

void init_hash_airp(Airport **H) {
  for (int i = 0; i < SIZE_HASH_AIRPORT; i++)
    H[i] = NULL;
}

/* Va chercher un airport dans la table de hachage en prenant en
 * compte les eventuelles collisions */

void collisions_airp(Airport ***pH, int index, char *code) {
  *pH = &((*pH)[index]);
  while (strcmp((**pH)->IATA_CODE, code) != 0)
    *pH = &((**pH)->suiv);
  return;
}

/* lire un Airport a partir d'une line de texte */
void read_airport_from_line(Airport **A, char *line) {
  Airport *new;
  new = malloc(sizeof(Airport));
  char *token = strtok(line, ",");
  int compt = 0;
  while (token != NULL) {
    if (compt == 0)
      strcpy((new)->IATA_CODE, token);
    if (compt == 1)
      strcpy((new)->name, token);
    if (compt == 2)
      strcpy((new)->city, token);
    if (compt == 3)
      strcpy((new)->state, token);
    if (compt == 4)
      strcpy((new)->country, token);
    if (compt == 5)
      strcpy((new)->latitude, token);
    if (compt == 6)
      strcpy((new)->longitude, token);
    compt++;
    token = strtok(NULL, ",");
  }
  new->suiv = NULL;
  new->infos_on_airl_at_airp = NULL;
  *A = new;
}

/*ajoute un Aéroport dans sa case de la table de hachage, si la
  case est déja occupée, on ajoute cet aéroport de façon
  alphabetique*/

void ajout_alph_airp(Airport **pA, Airport *new) {

  if ((*pA == NULL) || (strcmp((*pA)->IATA_CODE, new->IATA_CODE))) {
    new->suiv = *pA;
    *pA = new;
  }

  else {
    if (strcmp((*pA)->IATA_CODE, new->IATA_CODE) > 0)
      ajout_alph_airp(&(*pA)->suiv, new);
  }
}

/* recupère les Airports du fichier pour les mettre dans la table
 * hachage*/

void creation_hash_airports(FILE *fp, Airport **hash) {

  int nb = 0;
  int index;
  char *line = NULL;
  size_t len_line;
  Airport *new;
  new = NULL;

  getline(&line, &len_line, fp); // ignorer la légende

  while (((getline(&line, &len_line, fp)) != -1)) {
    read_airport_from_line(&new, line);
    index = get_hashed_index(new->IATA_CODE, SIZE_HASH_AIRPORT);
    ajout_alph_airp(&hash[index], new);
    nb++;
  }
  free(line);
}

// recherche d'un noeud airl_infos dans un ABR d'airl_infos
airl_infos *search_airl_infos(airl_infos *A, char *code) {
  if (A == NULL)
    return A;
  int test = strcmp(A->IATA_CODE, code);
  if (!test)
    return A;

  if (test > 0)
    return search_airl_infos(A->left, code);
  else
    return search_airl_infos(A->right, code);
}

// initialise le tableau des airlines les plus en retard pour
// un aéroport
void init_tab_airl_inf(airl_infos *tab[]) {
  for (int i = 0; i < 3; i++) {
    tab[i] = NULL;
  }
}

// recherche la case du tableau de airl_infos avec le retard moyen le
// plus bas
void search_min_tab_airl_inf(airl_infos *tab[], int *i) {
  int to_add = 0;
  int j = 0;
  while ((!to_add) && (j < 3)) {
    if (tab[j] == NULL) {
      *i = j;
      to_add = 1;
    } else if (tab[j]->arr_delay_avg < tab[*i]->arr_delay_avg)
      *i = j;
    j++;
  }
}

// parcours (de façon recursive) un arbre de airl_infos d'un aeroport
// ajoute au fur et à mesure ces cases dans le tableau d'airline les
// plus en retard pour cet aeroport.
void recup_most_del_airl_inf(airl_infos *A, airl_infos *tab[]) {
  int i = 0;
  int to_add = 0;
  if (A == NULL)
    return;
  while ((!to_add) && (i < 3)) {
    if (tab[i] == NULL)
      to_add = 1;
    else if (tab[i]->arr_delay_avg < A->arr_delay_avg) {
      search_min_tab_airl_inf(tab, &i);
      to_add = 1;
    } else
      i++;
  }
  if (i < 3)
    tab[i] = A;
  recup_most_del_airl_inf(A->left, tab);
  recup_most_del_airl_inf(A->right, tab);
  return;
}


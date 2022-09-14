#include "../includes/flights.h"
// Initialise la table de hachage ou seront rangés les vols

void init_hash_fl(hash_cell_fl **F) {

  for (int i = 0; i < SIZE_HASH_FLIGHTS; i++)
    F[i] = NULL;
}

// lire un vol à partir d'un fichier

void read_flight_from_line(Flight **F, char *line) {
  Flight *new;
  new = malloc(sizeof(Flight));
  char **pline = &line;
  char *token = strsep(pline, ",");
  int compt = 0;
  while (token != NULL) {
    if (compt == 0)
      (new)->d.month = atoi(token);
    if (compt == 1)
      (new)->d.day = atoi(token);
    if (compt == 2)
      (new)->d.weekday = atoi(token);
    if (compt == 3)
      strcpy((new)->airline, token);
    if (compt == 4)
      strcpy((new)->dep.airport, token);
    if (compt == 5)
      strcpy((new)->arr.airport, token);
    if (compt == 6)
      strcpy((new)->dep.time, token);
    if (compt == 7)
      (new)->dep.delay = atof(token);
    if (compt == 8)
      (new)->air_time = atof(token);
    if (compt == 9)
      (new)->dist = atoi(token);
    if (compt == 10)
      strcpy((new)->arr.time, token);
    if (compt == 11)
      (new)->arr.delay = atof(token);
    if (compt == 12)
      (new)->diverted = atoi(token);
    if (compt == 13)
      (new)->cancelled = atoi(token);
    compt++;
    token = strsep(pline, ",");
  }
  new->left = NULL;
  new->right = NULL;
  *F = new;
}

/*ajouter le vol à une arbre*/
void add_fl_abr(Flight **ptree, Flight *F) {
  if (*ptree == NULL) {
    *ptree = F;
    return;
  }
  if (strcmp((*ptree)->dep.airport, F->dep.airport) < 0)
    add_fl_abr(&((*ptree)->right), F);
  else
    add_fl_abr(&((*ptree)->left), F);
}

// recherche l'emplacement ou devrait être ajouté un Flight dans
// la table de hachage, puis l'ajoute

void ajout_alph_fl(hash_cell_fl **pH, Flight *F) {
  if (*pH == NULL) {
    hash_cell_fl *new;
    new = malloc(sizeof(hash_cell_fl));
    new->d = F->d;
    new->fl_tree = NULL;
    new->fl_tree = F;
    new->suiv = NULL;
    *pH = new;
    return;
  }

  if (!compare_dates((*pH)->d, F->d)) {
    add_fl_abr(&(*pH)->fl_tree, F);
    return;
  }
  ajout_alph_fl(&(*pH)->suiv, F);
}

// va rechercher un noeud Airline_node dans un ABR et placer
// son adresse dans a_modif

void search_airl(Airline_node **a_modif, Airline_node *A,
                 char *code) {

  if ((A == NULL) || (strcmp(A->IATA_CODE, code) == 0)) {
    *a_modif = A;
    return;
  }
  if (strcmp(A->IATA_CODE, code) > 0)
    search_airl(a_modif, A->left, code);
  if (strcmp(A->IATA_CODE, code) < 0)
    search_airl(a_modif, A->right, code);
}

// va rechercher un noeud airl_infos dans un ABR

void search_airl_inf(airl_infos ***a_modif, airl_infos **A,
                     char *code) {

  if ((*A == NULL) || (strcmp((*A)->IATA_CODE, code) == 0)) {
    *a_modif = A;
    return;
  }
  if (strcmp((*A)->IATA_CODE, code) > 0)
    search_airl_inf(a_modif, &(*A)->left, code);
  if (strcmp((*A)->IATA_CODE, code) < 0)
    search_airl_inf(a_modif, &(*A)->right, code);
}

/* Ajoute au delay total (stocké dans l'ABR d'Airline_node)
   de l'airline du vol F le delay qu'a eu ce vol
  à l'arrivée*/

void rempl_delay_tot(Flight *F, Airline_node **A) {

  Airline_node *a_modif;
  a_modif = NULL;
  char code[4];
  int nb;
  float delay;
  strcpy(code, F->airline);
  search_airl(&a_modif, *A, code);

  nb = (a_modif)->nb_fl;
  delay = (a_modif)->delay_moy;
  (a_modif)->nb_fl++;
  (a_modif)->delay_moy = (delay * nb + F->arr.delay) / (nb + 1);
}

/* mets à jour les infos de l'airline de ce vol dans les
   ABR d'airl_infos de l'aeroport de depart et celui d'arrivée*/

void rempl_infos_airp(Flight *F, Airport **H) {

  airl_infos **dep;
  // dep = malloc(sizeof(airl_infos *));
  dep = NULL;
  airl_infos **arr;
  // arr = malloc(sizeof(airl_infos *));
  arr = NULL;

  // depart
  int index = get_hashed_index(F->dep.airport, SIZE_HASH_AIRPORT);
  Airport **tmp;
  tmp = H;
  collisions_airp(&H, index, F->dep.airport);
  search_airl_inf(&dep, &(*H)->infos_on_airl_at_airp, F->airline);

  if (*dep == NULL) {
    *dep = malloc(sizeof(airl_infos));
    strcpy((*dep)->IATA_CODE, F->airline);
    (*dep)->arrives = 0;
    (*dep)->arr_delay_avg = 0;
    (*dep)->arr_nb_fl = 0;
    (*dep)->left = NULL;
    (*dep)->right = NULL;
  }
  (*dep)->departs = 1;
  H = tmp;

  // arrivee
  index = get_hashed_index(F->arr.airport, SIZE_HASH_AIRPORT);
  collisions_airp(&H, index, F->arr.airport);

  search_airl_inf(&arr, &(*H)->infos_on_airl_at_airp, F->airline);
  if (*arr == NULL) {
    *arr = malloc(sizeof(airl_infos));
    strcpy((*arr)->IATA_CODE, F->airline);
    (*arr)->departs = 1;
    (*arr)->arr_delay_avg = 0;
    (*arr)->arr_nb_fl = 0;
    (*arr)->left = NULL;
    (*arr)->right = NULL;
  }
  (*arr)->arrives = 1;
  if (!F->cancelled) {

    int nb = ((*arr)->arr_nb_fl);
    float delay = (*arr)->arr_delay_avg;
    (*arr)->arr_delay_avg = (delay * nb + F->arr.delay) / (nb + 1);
    (*arr)->arr_nb_fl++;
  }
}

// recherche de la date souhaité dans la table de hachage de vols,
// prend en compte les eventuelles collisions
void collisions_fl(hash_cell_fl ***pH, int index, Date date) {
  *pH = &((*pH)[index]);
  while (compare_dates((**pH)->d, date) != 0)
    *pH = &((**pH)->suiv);
  return;
}

// initialise les cases d'un tableau de Flight à NULL
void init_tab_fl(Flight *tab[], int size) {
  for (int i = 0; i < size; i++) {
    tab[i] = NULL;
  }
}

// recherche le vol avec le retard le plus faible dans le tableau
// des vols les plus retardé. Recupère dans i l'index de ce vol.
void search_min_tab_fl(Flight *tab[], int *i) {
  int to_add = 0;
  int j = 0;
  while ((!to_add) && (j < 5)) {
    if (tab[j] == NULL) {
      *i = j;
      to_add = 1;
    } else if (tab[j]->arr.delay < tab[*i]->arr.delay)
      *i = j;
    j++;
  }
}

// remplie le tableau des vols avec le plus de retards selon si le
// vol passé en paramètre a un retard plus grand que les vols présents
// dans le tableau
void recup_most_del_fl(Flight *F, Flight *tab[]) {
  int i = 0;
  int to_add = 0;

  while ((!to_add) && (i < 5)) {
    if (tab[i] == NULL)
      to_add = 1;
    else if (tab[i]->arr.delay < F->arr.delay) {
      search_min_tab_fl(tab, &i);
      to_add = 1;
    } else
      i++;
  }
  if (i < 5)
    tab[i] = F;
  return;
}

// recupère les vols d'un fichier pour stocker ses infos dans les SD

void recup_flights(FILE *fp, hash_cell_fl **Hash_fl,
                   Airline_node **airl_tree, Airport **Hash_airp,
                   Flight *tab_del_fl[]) {

  Flight *F;
  F = NULL;
  int index = 0;
  // int nb = 0;
  char *line = NULL;
  size_t len_line = 0;
  getline(&line, &len_line, fp);
  while ((getline(&line, &len_line, fp)) != -1) {
    read_flight_from_line(&F, line);
    index = date_hash(F->d) % SIZE_HASH_FLIGHTS;
    ajout_alph_fl(&Hash_fl[index], F);
    rempl_delay_tot(F, airl_tree);
    rempl_infos_airp(F, Hash_airp);
    recup_most_del_fl(F, tab_del_fl);

    // nb++;
  }
  free(line);
}

/*
 * FONCTIONS POUR REQUETES
 */

// utilisée pour la requete CHANGED-FLIGHT, affiche un vol si celui-ci
// a été annulé ou devié

void print_if_changed(Flight *F) {
  if (F == NULL)
    return;
  print_if_changed(F->left);
  if (F->cancelled)
    printf("  C");
  if (F->diverted)
    printf("  D");
  if ((F->cancelled) || (F->diverted))
    print_flight(F);
  print_if_changed(F->right);
}

// utilisé pour AVG-FLIGHT-DURATION
// parcourt l'arbre de vols d'une certaine date et recupère les temps
// de vol des vols partant de dep et arrivant à arr. Modifie avg_dur
// et nb_fl en consequence
void get_if_right_path(char *dep, char *arr, float *avg_dur,
                       int *nb_fl, Flight *F) {
  if (F == NULL)
    return;

  if ((!strcmp(F->dep.airport, dep)) &&
      (!strcmp(F->arr.airport, arr))) {
    *avg_dur = (*avg_dur) * (*nb_fl) / ((*nb_fl) + 1) +
               F->air_time * 1.0 / ((*nb_fl) + 1);
    *nb_fl = *nb_fl + 1;
  }

  if (strcmp(F->dep.airport, dep) < 0)
    return get_if_right_path(dep, arr, avg_dur, nb_fl, F->right);
  else
    return get_if_right_path(dep, arr, avg_dur, nb_fl, F->left);
}

void add_fl_tab(Flight *tab[], Flight *F, int size) {
  int i = 0;
  while ((tab[i] != NULL) && (i < size))
    i++;
  tab[i] = F;
}

int nb_flights_tab(Flight *tab[]) {
  int i = 0;
  while (tab[i] != NULL) {
    i++;
  }
  return i;
}

int max_stops_reached(Flight *tab[], int max) {
  return nb_flights_tab(tab) >= max;
}

void init_para(Consigne *ini, char *dep, int *test_time, char *time) {
  strcpy(dep, ini->dep);
  strcpy(time, ini->time);
  *test_time = ini->test_time;
}


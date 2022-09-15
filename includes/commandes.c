#include "../includes/commandes.h"

// desallocations

void free_airl_tree(Airline_node *A) {
  if (A == NULL)
    return;

  free_airl_tree((A)->left);
  free_airl_tree((A)->right);
  free(A);
}

void free_fl_tree(Flight **pnode) {
  if (*pnode == NULL)
    return;

  free_fl_tree(&((*pnode)->left));
  free_fl_tree(&((*pnode)->right));
  free(*pnode);
}

void free_fl_hash(hash_cell_fl *H[]) {
  hash_cell_fl *parcours = NULL;
  hash_cell_fl *supp = NULL;
  for (int i = 0; i < SIZE_HASH_FLIGHTS; i++) {
    parcours = H[i];
    while (parcours != NULL) {
      free_fl_tree(&(parcours->fl_tree));
      supp = parcours;
      parcours = parcours->suiv;
      free(supp);
    }
  }
}

void free_airl_inf_tree(airl_infos *A) {
  if (A == NULL)
    return;
  free_airl_inf_tree(A->left);
  free_airl_inf_tree(A->right);
  free(A);
}

void free_airp_hash(Airport **H) {
  Airport *parcours = NULL;
  Airport *supp = NULL;
  for (int i = 0; i < SIZE_HASH_AIRPORT; i++) {
    parcours = H[i];
    while (parcours != NULL) {
      free_airl_inf_tree(parcours->infos_on_airl_at_airp);
      supp = parcours;
      parcours = parcours->suiv;
      free(supp);
    }
  }
}

// fonctions pour recuperer des données

void recup_date(Date *date, char *temp) {
  char *token = strtok(temp, "-");
  date->month = atoi(token);
  token = strtok(NULL, "\0");
  date->day = atoi(token);
  date->weekday = -1;
}

void recup_time_or_lim(char *time, int *limit, int *test_time,
                       int *test_lim, char *temp) {
  char *token;
  if (temp[0] == 'l') {
    *test_lim = 1;
    token = strtok(temp, "=");
    token = strtok(NULL, "\0");
    strcpy(temp, token);
    *limit = atoi(temp);
    // printf("%d,", limit);
  } else {
    *test_time = 1;
    strcpy(time, temp);
    // printf("%s,", time);
  }
}

int test_x_hours(char *time1, char *time2, int h, int next_day) {
  int t2 = atoi(time2);
  int t1 = atoi(time1);
  int time_max;
  if (!next_day) {
    time_max = t2 + h * 100;
    if (time_max > 2359)
      time_max = 2360;
    return ((t1 >= t2) && (t1 <= time_max));
  } else {
    time_max = t2 + (h - 24) * 100;
    return (t1 < time_max);
  }
}

void copy_tab_consigne(Consigne *path, Consigne *temp, int size) {
  for (int i = 0; i < size - 1; i++) {
    path[i] = temp[i];
  }
}

void init_consigne(Consigne *ini) {
  strcpy(ini->dep, "rrr");
  strcpy(ini->arr, "rrr");
  strcpy(ini->time, "2400");
  ini->test_lim = 0;
  ini->test_time = 0;
  ini->limit = 0;
  strcpy(ini->date2, "0000");
}

// LES REQUETES

void show_airports(char *airl, Airport **Airp_hash) {

  Airport *temp = NULL;
  airl_infos *temp2 = NULL;

  for (int index = 0; index < SIZE_HASH_AIRPORT; index++) {
    if (Airp_hash[index] != NULL) {
      temp = Airp_hash[index];
      while (temp != NULL) {
        temp2 = search_airl_infos(temp->infos_on_airl_at_airp, airl);
        if ((temp2 != NULL) && (temp2->departs))
          print_airport(temp);
        temp = temp->suiv;
      }
    }
  }
}

void show_airlines(char *airp, Airline_node *Airl_tree,
                   Airport **Airp_hash) {
  int index = get_hashed_index(airp, SIZE_HASH_AIRPORT);
  Airport **tmp;
  tmp = Airp_hash;
  collisions_airp(&tmp, index, airp);
  print_airl_if_dep(Airl_tree, (*tmp)->infos_on_airl_at_airp);
  // free(tmp);
  return;
}

void show_flights(char *arg, hash_cell_fl **H) {
  char *token = strtok(arg, " ");
  char airp[4];
  Date date;
  char temp[13];
  char date2[6];

  int test_time = 0;
  int test_lim = 0;
  char time[5];
  int compt = 0;
  int limit = -1;

  // récuperation des arguments
  strcpy(airp, token);
  if ((airp == NULL) || (strlen(airp) != AIRP_LEN) ||
      ((airp[0] > 'Z') || (airp[0] < 'A'))) {
    print_err_lect();
    return;
  }

  token = strtok(NULL, " ");
  strcpy(date2, token);
  if ((date2 == NULL) || (strlen(date2) < DATE_LEN_MIN) ||
      ((date2[0] > '9') || (date2[0] < '0'))) {
    print_err_lect();
    return;
  }

  token = strtok(NULL, " ");

  while (token != NULL) {
    strcpy(temp, token);
    recup_time_or_lim(time, &limit, &test_time, &test_lim, temp);
    token = strtok(NULL, " ");
  }

  recup_date(&date, date2);

  // recherche et affichage des vols correspondants
  int index = date_hash(date) % SIZE_HASH_FLIGHTS;
  collisions_fl(&H, index, date);
  Flight *F = (*H)->fl_tree;
  while ((F != NULL) && (!test_lim || (compt < limit))) {
    if (!strcmp(F->dep.airport, airp) &&
        ((!test_time) || (test_x_hours(F->dep.time, time, 4, 0)))) {
      compt++;
      print_flight(F);
    }
    if (strcmp(F->dep.airport, airp) < 0)
      F = F->right;
    else
      F = F->left;
  }
}

void most_delayed_flights(Flight **tab) {
  for (int i = 0; i < SIZE_TAB_DELAY; i++) {
    if (tab[i] != NULL)
      print_flight(tab[i]);
  }
}

void most_delayed_airlines(Airline_node *most_del[]) {
  // Airline_node *most = A;
  // find_most_del_airl(A,&most);
  for (int i = 0; i < SIZE_TAB_DELAY; i++) {
    if (most_del[i] != NULL)
      print_airline(most_del[i]);
  }
  return;
}

void most_delayed_airlines_at_airport(char *airp,
                                      Airport *Airp_hash[],
                                      Airline_node *Airl_tree) {
  airl_infos *tab[3];
  init_tab_airl_inf(tab);
  int index = get_hashed_index(airp, SIZE_HASH_AIRPORT);
  collisions_airp(&Airp_hash, index, airp);
  recup_most_del_airl_inf((*Airp_hash)->infos_on_airl_at_airp, tab);
  for (int i = 0; i < 3; i++) {
    if (tab[i] != NULL) {
      printf("  %.2f", tab[i]->arr_delay_avg);
      print_airline(search_airline_ABR(Airl_tree, tab[i]->IATA_CODE));
    }
  }
}

void delayed_airlines(char *airl, Airline_node *Airl_tree) {
  Airline_node *A;
  A = NULL;
  search_airl(&A, Airl_tree, airl);
  printf("  %s's average delay : %.2f\n", A->IATA_CODE, A->delay_moy);
}

void changed_flights(char *date, hash_cell_fl *fl_hash[]) {
  char *token = strtok(date, "-");
  Date d;
  d.weekday = 1;
  d.month = atoi(token);
  token = strtok(NULL, "\n");
  d.day = atoi(token);
  int index = date_hash(d) % SIZE_HASH_FLIGHTS;

  collisions_fl(&fl_hash, index, d);
  print_if_changed((*fl_hash)->fl_tree);
}

void avg_flight_duration(char *arg, hash_cell_fl *Fl_hash[]) {
  float avg_dur = 0;
  int nb_fl = 0;
  char dep[4];
  char arr[4];
  hash_cell_fl *parcours = NULL;

  char *token = strtok(arg, " ");
  strcpy(dep, token);
  if ((dep == NULL) || (strlen(dep) != AIRP_LEN)) {
    printf("ici\n");
    print_err_lect();
    return;
  }
  token = strtok(NULL, "\n");
  strcpy(arr, token);
  if ((arr == NULL) || (strlen(arr) != AIRP_LEN)) {
    printf("la\n");
    print_err_lect();
    return;
  }

  for (int i = 0; i < SIZE_HASH_FLIGHTS; i++) {
    parcours = Fl_hash[i];
    while (parcours != NULL) {
      get_if_right_path(dep, arr, &avg_dur, &nb_fl,
                        parcours->fl_tree);
      parcours = parcours->suiv;
    }
  }

  printf("  Average duration between %s and %s: %.2f ( %d flights)\n",
         dep, arr, avg_dur, nb_fl);
}

// solutions intermédiaire pour la recherche d'itinéraire
// solution 1 : trajets sans escale
void find_direct_it(hash_cell_fl *Fl_hash[], Date date, int *compt,
                    char *dep, char *arr, int test_lim, int limit,
                    int test_time, char *time) {
  int index = date_hash(date) % SIZE_HASH_FLIGHTS;
  hash_cell_fl **temp = Fl_hash;
  collisions_fl(&temp, index, date);
  Flight *F = (*temp)->fl_tree;
  while ((F != NULL) && (!test_lim || (*compt < limit))) {
    if ((!strcmp(F->dep.airport, dep)) &&
        (!strcmp(F->arr.airport, arr)) &&
        ((!test_time) ||
         (test_x_hours(F->dep.time, time, ESC_TIME_MAX, 0)))) {
      (*compt)++;
      print_flight(F);
      printf("\n");
    }
    if (strcmp(F->dep.airport, dep) < 0)
      F = F->right;
    else
      F = F->left;
  }

  if ((test_time) && (test_x_hours("2360", time, ESC_TIME_MAX, 0) &&
                      (next_day(&date)))) {
    index = date_hash(date) % SIZE_HASH_FLIGHTS;
    temp = Fl_hash;
    collisions_fl(&temp, index, date);
    Flight *F = (*temp)->fl_tree;
    while ((F != NULL) && (!test_lim || (*compt < limit))) {
      if ((!strcmp(F->dep.airport, dep)) &&
          (!strcmp(F->arr.airport, arr)) &&
          ((!test_time) ||
           (test_x_hours(F->dep.time, time, ESC_TIME_MAX, 1)))) {
        (*compt)++;
        print_flight(F);
        printf("\n");
      }
      if (strcmp(F->dep.airport, dep) < 0)
        F = F->right;
      else
        F = F->left;
    }
  }
}
// soluton 2: jusque 3 escales
void find_escales_it(Consigne *ini, hash_cell_fl *Fl_hash[],
                     Date date, int *compt, char *dep, char *arr,
                     int test_lim, int limit, int test_time,
                     char *time, int days_limit, Flight *tab[]) {

  char temp2[4];
  int index = date_hash(date);
  index = index % SIZE_HASH_FLIGHTS;
  hash_cell_fl **temp = Fl_hash;
  collisions_fl(&temp, index, date);
  Flight *F = (*temp)->fl_tree;

  while ((F != NULL) && (!test_lim || (*compt < limit))) {
    if ((!strcmp(F->dep.airport, dep)) &&
        ((!test_time) || (test_x_hours(F->dep.time, time, 12, 0)))) {
      add_fl_tab(tab, F, SIZE_TAB_DELAY);
      if (!strcmp(F->arr.airport, arr)) {
        (*compt)++;
        print_tab_fl(tab, SIZE_TAB_DELAY);
        printf("\n");
        init_tab_fl(tab, SIZE_TAB_DELAY);
        init_para(ini, dep, &test_time, time);
      } else {
        if (!max_stops_reached(tab, 3)) {
          strcpy(temp2, F->arr.airport);
          find_escales_it(ini, Fl_hash, date, compt, temp2, arr,
                          test_lim, limit, 1, F->arr.time, days_limit,
                          tab);
        } else {
          tab[nb_flights_tab(tab) - 1] = NULL;
          init_para(ini, dep, &test_time, time);
          // return;
        }
      }
    }
    if (strcmp(F->dep.airport, dep) < 0)
      F = F->right;
    else
      F = F->left;
  }

  if ((days_limit < 2) && (test_time) &&
      (test_x_hours("2360", time, 12, 0) && (next_day(&date)))) {
    index = date_hash(date) % SIZE_HASH_FLIGHTS;
    temp = Fl_hash;
    collisions_fl(&temp, index, date);
    Flight *F = (*temp)->fl_tree;
    days_limit++;
    while ((F != NULL) && (!test_lim || (*compt < limit))) {
      if ((!strcmp(F->dep.airport, dep)) &&
          ((!test_time) ||
           (test_x_hours(F->dep.time, time, 12, 1)))) {
        add_fl_tab(tab, F, SIZE_TAB_DELAY);
        if (!strcmp(F->arr.airport, arr)) {
          (*compt)++;
          print_tab_fl(tab, SIZE_TAB_DELAY);
          init_tab_fl(tab, SIZE_TAB_DELAY);
          printf("\n");
          init_para(ini, dep, &test_time, time);
        } else {
          if (!max_stops_reached(tab, 3)) {
            strcpy(temp2, F->arr.airport);
            find_escales_it(ini, Fl_hash, date, compt, temp2, arr,
                            test_lim, limit, 1, F->arr.time,
                            days_limit, tab);
          } else {
            tab[nb_flights_tab(tab) - 1] = NULL;
            init_para(ini, dep, &test_time, time);

            // return;
          }
        }
      }
      if (strcmp(F->dep.airport, dep) < 0)
        F = F->right;
      else
        F = F->left;
    }
  }

  tab[nb_flights_tab(tab) - 1] = NULL;
}

void find_itinerary(char *arg, hash_cell_fl *Fl_hash[]) {
  Flight *tab[SIZE_TAB_DELAY];
  init_tab_fl(tab, SIZE_TAB_DELAY);

  char dep[4];
  char arr[4];

  char *token = strtok(arg, " ");
  Date date;
  char temp[13];
  char date2[6];

  int test_time = 0;
  int test_lim = 0;
  char time[5] = "2400";
  int compt = 0;
  int limit = -1;
  Consigne initial;
  init_consigne(&initial);
  // récuperation des arguments

  strcpy(dep, token);
  if ((dep == NULL) || (strlen(dep) != AIRP_LEN)) {
    print_err_lect();
    return;
  }
  token = strtok(NULL, " ");
  strcpy(arr, token);

  if ((arr == NULL) || (strlen(arr) != AIRP_LEN)) {
    print_err_lect();
    return;
  }

  token = strtok(NULL, " ");

  strcpy(date2, token);
  if ((date2 == NULL) || (strlen(date2) < DATE_LEN_MIN)) {
    print_err_lect();
    return;
  }

  token = strtok(NULL, " ");

  while (token != NULL) {
    strcpy(temp, token);
    recup_time_or_lim(time, &limit, &test_time, &test_lim, temp);
    token = strtok(NULL, " ");
  }

  recup_date(&date, date2);
  // recherche des vols

  strcpy(initial.dep, dep);
  strcpy(initial.arr, arr);
  initial.date = date;
  strcpy(initial.time, time);
  initial.limit = limit;
  initial.test_time = test_time;
  initial.test_lim = test_lim;

  find_escales_it(&initial, Fl_hash, date, &compt, dep, arr, test_lim,
                  limit, test_time, time, 0, tab);
}

void find_multicity_itinerary(char *arg, hash_cell_fl *Fl_hash[]) {
  Flight *tab[SIZE_TAB_DELAY];
  init_tab_fl(tab, SIZE_TAB_DELAY);

  char *token = strtok(arg, " ");
  int size = 1;
  Consigne path[size];
  path[size - 1].test_time = 0;
  strcpy(path[size - 1].time, "2360");
  Consigne *inter = NULL;
  init_consigne(&path[0]);

  char temp[6];

  int compt = 0;

  // récuperation des arguments
  int nb_elem = 0;

  while (token != NULL) {
    if (nb_elem == 0) {
      strcpy(path[size - 1].dep, token);
      if (strlen(path[size - 1].dep) != AIRP_LEN) {
        print_err_lect();
        return;
      }
    }
    if (nb_elem == 1) {
      strcpy(path[size - 1].arr, token);
      if (strlen(path[size - 1].arr) != AIRP_LEN) {
        print_err_lect();
        return;
      }
    }
    if (nb_elem == 2) {
      strcpy(path[size - 1].date2, token);
    }
    if (nb_elem == 3) {
      strcpy(temp, token);
      if ((temp[0] <= 'Z') && (temp[0] >= 'A') &&
          (strlen(temp) == AIRP_LEN)) {
        size++;
        inter = path;
        Consigne path[size];
        copy_tab_consigne(path, inter, size);
        init_consigne(&path[size - 1]);
        nb_elem = 1;
        strcpy(path[size - 1].arr, token);
      } else if ((temp[0] <= '9') && (temp[0] >= '0') &&
                 (strlen(temp) == TIME_LEN)) {
        path[size - 1].test_time = 1;
        strcpy(path[size - 1].time, token);
        nb_elem = 2;
      } else {
        print_err_lect();
        return;
      }
    }
    token = strtok(NULL, " ");
    nb_elem++;
  }

  for (int i = 0; i + 1 < size; i++) {
    recup_date(&path[i].date, path[i].date2);
    strcpy(path[i + 1].dep, path[i].arr);
    path[i].test_lim = 0;
    path[i].limit = -1;
    find_escales_it(&path[i], Fl_hash, path[i].date, &compt,
                    path[i].dep, path[i].arr, path[i].test_lim,
                    path[i].limit, path[i].test_time, path[i].time, 0,
                    tab);
  }
}

void quit(hash_cell_fl **Fl_hash, Airline_node **Airl_tree,
          Airport **Airp_hash) {
  free_fl_hash(Fl_hash);
  free_airl_tree(*Airl_tree);
  free_airp_hash(Airp_hash);
}

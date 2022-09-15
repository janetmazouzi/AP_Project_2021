#include "../includes/fonctions.h"

/*
 * Hash function
 */

int asciis_modif(char *word) {
  return word[0] * 10 + word[1] * 5 + word[2] - 3 * 'A';
}

int date_hash(Date d) { return (d.month * 100 + d.day); }

int compare_dates(Date d1, Date d2) {
  if (d1.month < d2.month)
    return -1;
  else if (d1.month > d2.month)
    return 1;
  else {
    if (d1.day < d2.day)
      return -1;
    else if (d1.day > d2.day)
      return 1;
    else
      return 0;
  }
}

int next_day(Date *d) {
  if (d->day <= 27) {
    d->day++;
    return 1;
  }
  if (d->month == 2) {
    d->month++;
    d->day = 1;
    return 1;
  }
  if (d->day == 31) {
    if (d->month == 12)
      return 0;
    d->month++;
    d->day = 1;
    return 1;
  }

  if (((!(d->month % 2)) && (d->month <= 7)) ||
      ((d->month % 2) && (d->month > 7))) {
    if (d->day == 30) {
      d->month++;
      d->day = 1;
    } else {
      d->day++;
    }
    return 1;
  }
  d->day++;
  return 1;
}

/*
 * Index function: hashes the word and returns a valid index
 */
int get_hashed_index(char *word, int table_size) {
  return (asciis_modif(word) % table_size);
}

// AFFICHAGE

void print_flight(Flight *F) {
  printf("  %d,%d,%d,%s,%s,%s,%s,%.1f,%.1f,%d,%s,%.1f,%d,%d\n",
         F->d.month, F->d.day, F->d.weekday, F->airline,
         F->dep.airport, F->arr.airport, F->dep.time, F->dep.delay,
         F->air_time, F->dist, F->arr.time, F->arr.delay, F->diverted,
         F->cancelled);
}

void print_airport(Airport *A) {
  printf("  %s,%s,%s,%s,%s\n", A->IATA_CODE, A->name, A->city,
         A->state, A->country);
}

void print_airline(Airline_node *A) {
  printf("  %s,%s", A->IATA_CODE, A->name);
}

void print_airline_tree(Airline_node *A) {
  if (A == NULL)
    return;
  print_airline_tree(A->left);
  print_airline(A);
  print_airline_tree(A->right);
}

void print_tab_fl(Flight *tab[], int size) {
  for (int i = 0; i < size; i++) {
    if (tab[i] != NULL)
      print_flight(tab[i]);
  }
}

int print_err_lect() {
  printf(
      "Saisie incorrecte, veuillez vous referrer au fichier "
      "README.md pour executer les requetes dans un format valide");
  return 0;
}

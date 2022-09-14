#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_HASH_FLIGHTS 331
#define SIZE_HASH_AIRPORT 320

#define SIZE_TAB_DELAY 5

#define AIRP_LEN 3
#define AIRL_LEN 2
#define DATE_LEN_MIN 3
#define TIME_LEN 4

#define ESC_TIME_MAX 12

/*
 * DEFINITION DES STRUCTURES DE BASE
 */

struct date {
  int month;
  int day;
  int weekday;
};

typedef struct date Date;

struct horaire {
  char time[5];
  double delay;
  char airport[4];
};

typedef struct horaire Horaire;

struct flight {
  Date d;
  Horaire dep;
  Horaire arr;
  char airline[3];
  double air_time;
  int dist;
  bool diverted;
  bool cancelled;
  struct flight *left;
  struct flight *right;
};
typedef struct flight Flight;

struct hash_cell_fl {
  Date d;
  Flight *fl_tree;
  struct hash_cell_fl *suiv;
};
typedef struct hash_cell_fl hash_cell_fl;

// stocke les infos sur les airline par airport: s'ils ont des vols
// qui y partent, qui y arrivent, le delay moyen, ect
struct airl_infos {
  char IATA_CODE[3];
  bool arrives;
  bool departs;
  double arr_delay_avg;
  int arr_nb_fl;
  struct airl_infos *left;
  struct airl_infos *right;
};
typedef struct airl_infos airl_infos;

// Les aéroports seont rangés dans une table de hashage selon le
// IATA_CODE
struct airport {
  char IATA_CODE[4];
  char name[40];
  char city[20];
  char state[5];
  char country[20];
  char latitude[20];
  char longitude[20];
  airl_infos *infos_on_airl_at_airp;
  struct airport *suiv;
};
typedef struct airport Airport;

// Les compagnies seront rangés dans un ABR trié par ordre
// alphabétique d'IATA_CODE
struct airline_node {
  char IATA_CODE[3];
  char name[40];
  double delay_moy;
  int nb_fl;
  struct airline_node *left;
  struct airline_node *right;
};
typedef struct airline_node Airline_node;

struct consigne {
  char dep[4];
  char arr[4];
  Date date;
  int test_lim;
  int test_time;
  char time[5];
  int limit;
  char date2[6];
};
typedef struct consigne Consigne;

/*
 * FONCTIONS UTILES
 */

// HASH

int asciis_modif(char *);
int date_hash(Date);
int get_hashed_index(char *, int table_size);
int compare_dates(Date, Date);
int next_day(Date *);
// DIVERS

// Affichage

void print_flight(Flight *);
void print_airport(Airport *);
void print_airline(Airline_node *);
void print_airline_tree(Airline_node *);
void print_tab_fl(Flight **, int);
int print_err_lect();

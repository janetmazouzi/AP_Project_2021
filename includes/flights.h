#include "airports.h"

// fonctions pour les remplissages de SD
void init_hash_fl(hash_cell_fl **);
void read_flight_from_line(Flight **, char *);

int compare_dates(Date, Date);
void add_fl_abr(Flight **, Flight *);
void ajout_alph_fl(hash_cell_fl **, Flight *);

void search_airl(Airline_node **, Airline_node *, char *);
void search_airl_inf(airl_infos ***, airl_infos **, char *);
void rempl_delay_tot(Flight *, Airline_node **);
void rempl_infos_airp(Flight *, Airport **);
void init_tab_fl(Flight **, int);
void search_min_tab_fl(Flight **, int *);
void recup_most_del_fl(Flight *, Flight **);

void recup_flights(FILE *, hash_cell_fl **, Airline_node **,
                   Airport **, Flight **);

// fonctions pour requetes
void collisions_fl(hash_cell_fl ***, int, Date);
void print_if_changed(Flight *);
void get_if_right_path(char *, char *, float *, int *, Flight *);

void add_fl_tab(Flight **, Flight *, int);
int nb_flights_tab(Flight **);
int max_stops_reached(Flight **, int max);
void init_para(Consigne *, char *, int *, char *);

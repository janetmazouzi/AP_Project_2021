#include "flights.h"

// désallocations
void free_airl_tree(Airline_node *);
void free_fl_tree(Flight **);
void free_fl_hash(hash_cell_fl **);
void free_airl_inf_tree(airl_infos *);
void free_airp_hash(Airport **);

// fonctions utiles pour la recup de données

void recup_date(Date *, char *);

void recup_time_or_lime(char *, int *, int *, int *, char *);

int test_x_hours(char *, char *, int, int);

void copy_tab_consigne(Consigne *, Consigne *, int);

// traitement de requetes
void show_airports(char *, Airport **);

void show_airlines(char *, Airline_node *, Airport **);

void show_flights(char *, hash_cell_fl **);

void most_delayed_flights(Flight **);

void most_delayed_airlines(Airline_node **);

void most_delayed_airlines_at_airport(char *, Airport **,
                                      Airline_node *);

void delayed_airlines(char *, Airline_node *);

void changed_flights(char *, hash_cell_fl **);

void avg_flight_duration(char *, hash_cell_fl **);

void find_direct_it(hash_cell_fl **, Date, int *, char *, char *, int,
                    int, int, char *);
void find_escales_it(Consigne *, hash_cell_fl **, Date, int *, char *,
                     char *, int, int, int, char *, int, Flight **);

void find_itinerary(char *, hash_cell_fl **);

void find_multicity_itinerary(char *, hash_cell_fl **);

void quit(hash_cell_fl **, Airline_node **, Airport **);

void init_consigne(Consigne *);

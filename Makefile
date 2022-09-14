#####################################################
CC=gcc # compilateur
CFLAGS=-g -W -Wall -Wextra -O0 # warnings

$(shell mkdir -p build)
#### Variables liées à la compilation des sources
OBJ=build/fonctions.o build/airlines.o build/airports.o build/flights.o build/commandes.o

EXEC=build/projet


########### Compilation du code principal ###########
main: $(EXEC)

$(EXEC): build/projet.o $(OBJ)
	$(CC) $^ -o $(EXEC)

build/projet.o: src/projet.c includes/commandes.h 
	$(CC) $(CFLAGS) -c $< -o $@

build/fonctions.o: src/fonctions.c includes/fonctions.h
	$(CC) $(CFLAGS) -c $< -o $@

build/airlines.o: src/airlines.c includes/airlines.h includes/fonctions.h
	$(CC) $(CFLAGS) -c $< -o $@
	
build/airports.o: src/airports.c includes/airports.h includes/airlines.h
	$(CC) $(CFLAGS) -c $< -o $@

build/flights.o: src/flights.c includes/flights.h includes/airports.h
	$(CC) $(CFLAGS) -c $< -o $@

build/commandes.o: src/commandes.c includes/commandes.h includes/flights.h
	$(CC) $(CFLAGS) -c $< -o $@
#####################################################



#################### Nettoyage #####################
clean:
	@rm -rf build/*.o build/*.gc* *.gcov src/*.dump
#####################################################

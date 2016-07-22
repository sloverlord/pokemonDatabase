#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POKNUM 152

// attack storage
typedef struct attack {
	int id;
	char name[20];
	char type[10];
	char class[10];		// special, physical, status
	
	int range;
	int power;
	int accuracy;
} attack;

// basic stat storage
typedef struct pokemon {
	char name[20];
	int hp, atk, def, spatk, spdef, spd;
	int id;
	
	char type1[10];
	char type2[10];
} pokemon;

// Storage for an entire charachter
typedef struct charachter {
	pokemon *class;
	char *name;
	int level;
	int exp;
	// ability points
	int ap;
	
	//bonuses
	int hp;
	int atk;
	int def;
	int spatk;
	int spdef;
	int spd;
	
	struct charachter *next;
} charachter;

pokemon *database[POKNUM];
charachter *allChars;

// prints all stats of a pokemon
void printPokeInfo(FILE *fp, pokemon *toPrint){
	fprintf(fp, "%d\t%s\t%s\t%s\n%d\t%d\t%d\t%d\t%d\t%d\t\n", toPrint->id, toPrint->name, toPrint->type1, toPrint->type2, toPrint->hp, toPrint->atk, toPrint->def, toPrint->spatk, toPrint->spdef, toPrint->spd);
}

// looks through list for passed in named charachter
charachter *findChar(char *name){
	charachter *looker = allChars;
	
	while (looker != NULL && strcmp(looker->name, name)){
		looker = looker->next;
	}
		
	return looker;
}

void giveExp(char *toGet, int exp){
	charachter *finder = findChar(toGet);
	
	finder->exp += exp;
	
	// level up code, change as needed
	while (finder->exp > finder->level){
		finder->exp -= finder->level;
		finder->level += 1;
		finder->ap += 1;
		finder->hp += 1;
	}
}

// adds a charachter to list if not already in list
void addChar(charachter *toAdd){	
	if (findChar(toAdd->name) != NULL){
		fprintf(stdout, "Player already exists\n");
		return;
	}
	
	toAdd->next = allChars;
	allChars = toAdd;
}

// returns the number id of a pokemon given by name, -1 if not found
int findByName(char *name){
	int i;
	
	for (i = 0; i < POKNUM; i += 1){
		if (database[i] != NULL && strcmp(database[i]->name, name) == 0) {
			return i;
		}
	}
	
	return -1;
}

// add a new pokemon
void newPokemon(){
	pokemon *newPok = malloc(sizeof(pokemon));
	
	fprintf(stdout, "enter name: ");
	fscanf(stdin, "%s", newPok->name);
	
	fprintf(stdout, "enter hp: ");
	fscanf(stdin, "%d", &newPok->hp);
	fprintf(stdout, "enter atk: ");
	fscanf(stdin, "%d", &newPok->atk);
	fprintf(stdout, "enter def: ");
	fscanf(stdin, "%d", &newPok->def);
	fprintf(stdout, "enter spatk: ");
	fscanf(stdin, "%d", &newPok->spatk);
	fprintf(stdout, "enter spdef: ");
	fscanf(stdin, "%d", &newPok->spdef);
	fprintf(stdout, "enter spd: ");
	fscanf(stdin, "%d", &newPok->spd);
	
	fprintf(stdout, "Enter id: ");
	fscanf(stdin, "%d", &newPok->id);
	
	fprintf(stdout, "enter type 1: ");
	fscanf(stdin, "%s", newPok->type1);
	
	fprintf(stdout, "enter type 2: ");
	fscanf(stdin, "%s", newPok->type2);
	
	database[newPok->id] = newPok;
}

// displays stats for specified pokemon
void displayPokStats(char *name){
	int id = findByName(name);
	
	if (database[id] == NULL || id < 0){
		fprintf(stdout, "Not an entry\n");
	} else {
		printPokeInfo(stdout, database[id]);
	}
}

// save the database to a file
void saveDatabase(char *fName){
	FILE *fp = fopen(fName, "w");
	
	int id;
	for (id = 1; id < POKNUM; id += 1){
		if (database[id] != NULL){
			printPokeInfo(fp, database[id]);
		}
	}
	
	fclose(fp);
}

// Load a file into database (individual pokemon)
void loadDatabase(char *fName){
	FILE *fp = fopen(fName, "r");
	
	while (!feof(fp)){
		pokemon *newPok = malloc(sizeof(pokemon));
	
		fscanf(fp, "%d%s%s%s%d%d%d%d%d%d", &newPok->id, newPok->name, newPok->type1, newPok->type2, &newPok->hp, &newPok->atk, &newPok->def, &newPok->spatk, &newPok->spdef, &newPok->spd);
		
		fprintf(stdout, "loaded: %s\n", newPok->name);
		
		database[newPok->id] = newPok;
	}
	
	fclose(fp);
}

// Load a file into database (individual moves)
void loadDatabase(char *fName){
	FILE *fp = fopen(fName, "r");
	
	while (!feof(fp)){
		pokemon *newPok = malloc(sizeof(pokemon));
	
		fscanf(fp, "%d%s%s%s%d%d%d%d%d%d", &newPok->id, newPok->name, newPok->type1, newPok->type2, &newPok->hp, &newPok->atk, &newPok->def, &newPok->spatk, &newPok->spdef, &newPok->spd);
		
		fprintf(stdout, "loaded: %s\n", newPok->name);
		
		database[newPok->id] = newPok;
	}
	
	fclose(fp);
}

// make a new charachter
void newCharachter(){
	charachter *newPok = malloc(sizeof(charachter));
	char *name = malloc(sizeof(char *));
	
	fprintf(stdout, "enter pokemon to be: ");
	fscanf(stdin, "%s", name);
	
	int id = findByName(name);
	
	if (database[id] != NULL){
		int level;
		fprintf(stdout, "Enter starting level: ");
		fscanf(stdin, "%d", &level);
	
		fprintf(stdout, "enter charachter name to be: ");
		fscanf(stdin, "%s", newPok->name);
		
		newPok->level = 0;
		
		newPok->hp = 0;
		newPok->atk = 0;
		newPok->def = 0;
		newPok->spatk = 0;
		newPok->spdef = 0;
		newPok->spd = 0;
		
		newPok->class = database[id];

		addChar(newPok);
		
		while (newPok->level < level){
			giveExp(newPok->name, 1);
		}
	}
}

// save a charachter to a file
void saveChar(char *toSave, char*fName){
	charachter *finder = findChar(toSave);
	
	FILE *fp = fopen(fName, "w");
	
	if (finder != NULL){
		fprintf(fp, "%s - level: %d %s %d\n", finder->name, finder->level, finder->class->name, finder->ap);
		fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\n", finder->hp, finder->atk, finder->def, finder->spatk, finder->spdef, finder->spd);
	} else {
		fprintf(stdout, "Could not find\n");
	}
	
	fclose(fp);
}

// print a charachter to stdout
void printChar(charachter *finder){
	if (finder != NULL){
		fprintf(stdout, "%s\t%d (%d)\n",finder->name, finder->level, finder->ap);
		printPokeInfo(stdout, finder->class);
		fprintf(stdout, "%d\t%d\t%d\t%d\t%d\t%d\n", finder->hp, finder->atk, finder->def, finder->spatk, finder->spdef, finder->spd);
	} else {
		fprintf(stdout, "Could Not Find\n");
	}
}

// Load a file into data base (individual player)
void loadPlayer(char *fName){
	char *toss = malloc(sizeof(char *));
	char *temp = malloc(sizeof(char *));
	charachter *newChar = malloc(sizeof(charachter));
	
	FILE *fp = fopen(fName, "r");
	
	fscanf(fp, "%s", temp);
	newChar->name = temp;
	fscanf(fp, "%s", toss);
	fscanf(fp, "%s", toss);
	fscanf(fp, "%d", &newChar->level);
	fscanf(fp, "%s", toss);
	fscanf(fp, "%d", &newChar->ap);
	
	//fscanf(fp, "%s%s%s%d%s%d", newChar->name, toss, toss, &newChar->level, toss, &newChar->ap);
	fscanf(fp, "%d%d%d%d%d%d", &newChar->hp, &newChar->atk, &newChar->def, &newChar->spatk, &newChar->spdef, &newChar->spd);
	
	int id = findByName(toss);
	
	if (id != -1){
		newChar->class = database[id];
	
		addChar(newChar);
	} else {
		fprintf(stdout, "Could not load\n");
	}
	
	fclose(fp);
}

// apply ability points
void spendAp(char *toGet){
	charachter *finder = findChar(toGet);
	
	while (finder->ap > 0){
		int stat;
		
		fprintf(stdout, "You have %i ability points remaining\n", finder->ap);
		
		fprintf(stdout, "\n0 - quit\t6 - show stats\n1 - hp\t2 - atk\t3 - def\t4 - spatk\t5 - spdef\nEnter the stat to increase: ");
		fscanf(stdin, "%i", &stat);
		
		if (stat == 0){
			return;
		} else if (stat == 1){
			finder->hp += 1;
			finder->ap -= 1;
		} else if (stat == 2){
			finder->atk += 1;
			finder->ap -= 1;
		} else if (stat == 3){
			finder->def += 1;
			finder->ap -= 1;
		} else if (stat == 4){
			finder->spatk += 1;
			finder->ap -= 1;
		} else if (stat == 5){
			finder->spdef += 1;
			finder->ap -= 1;
		} else if (stat == 6){
			printChar(finder);
		} else {
			fprintf(stdout, "wrong number\n");
		}
	}
}

// list input options
void listOptions(){
	fprintf(stdout, "0: options\n1: new pokemon\n");
	fprintf(stdout, "2: display a pokemon\n");
	fprintf(stdout, "3: save pokemon database\n");
	fprintf(stdout, "4: load pokemon database\n5: new charachter\n");
	fprintf(stdout, "6: save a charachter\n");
	fprintf(stdout, "7: print a charachter\n");
	fprintf(stdout, "8: load a charachter\n");
	fprintf(stdout, "9: give exp\n");
	fprintf(stdout, "10: spend ability points\n");
	fprintf(stdout, "11: load moves database\n");
	fprintf(stdout, ".\n.\n.\n");
	fprintf(stdout, "99: auto-start\n");
	fprintf(stdout, "100: exit\n");
}

int main(void){
	int mode = 0;
	
	fprintf(stdout, "Enter action (0 for options): ");
	fscanf(stdin, "%i", &mode);
	
	// loop until told to exit
	while(mode != 100){
		if (mode == 0){
			listOptions();
		} else if (mode == 1){
			newPokemon();
		} else if (mode == 2){
			char *name = malloc(sizeof(char *));
	
			fprintf(stdout, "enter name: ");
			fscanf(stdin, "%s", name);
	
			displayPokStats(name);
		} else if (mode == 3){
			char *fName = malloc(sizeof(char *));
			
			fprintf(stdout, "Enter a file to save database to: ");
			fscanf(stdin, "%s", fName);
			
			saveDatabase(fName);
		} else if (mode == 4){
			char *fName = malloc(sizeof(char *));
			
			fprintf(stdout, "Enter a file to load pokemon: ");
			fscanf(stdin, "%s", fName);
			
			loadDatabase(fName);
		} else if (mode == 5){
			newCharachter();
		} else if (mode == 6){
			char *toSave = malloc(sizeof(char *));
			char *fName = malloc(sizeof(char *));
			
			fprintf(stdout, "Enter a file to save char to: ");
			fscanf(stdin, "%s", fName);
			
			fprintf(stdout, "Enter a charachter to save: ");
			fscanf(stdin, "%s", toSave);
			
			saveChar(toSave, fName);
		} else if (mode == 7){
			char *toFind = malloc(sizeof(char *));
			fprintf(stdout, "Enter a charachter to print: ");
			fscanf(stdin, "%s", toFind);
	
			charachter *finder = findChar(toFind);
	
			printChar(finder);
		} else if (mode == 8){
			char *fName = malloc(sizeof(char *));
	
			fprintf(stdout, "Enter a file to load player: ");
			fscanf(stdin, "%s", fName);
	
			loadPlayer(fName);
		} else if (mode == 9){
			char *toGet;
			int exp;
			
			fprintf(stdout, "Enter the player to get experience: ");
			fscanf(stdin, "%s", toGet);
			
			fprintf(stdout, "Enter exp to give");
			fscanf(stdin, "%i", &exp);
			
			giveExp(toGet, exp);
		} else if (mode == 10){
			char *toGet;
			
			fprintf(stdout, "Enter the player to spend ability points: ");
			fscanf(stdin, "%s", toGet);
			
			spendAp(toGet);
		} else if (mode == 11){
			char *fName = malloc(sizeof(char *));
			
			fprintf(stdout, "Enter a file to load moves: ");
			fscanf(stdin, "%s", fName);
			
			loadMoves(fName);
		} else if (mode == 99){
			// auto load databases and players
			loadDatabase("test.txt");
			loadMoves("moves.txt");
			loadPlayer("dcr.dat");
			loadPlayer("alex.dat");
			loadPlayer("cry.dat");
		}
		
		mode = 100;
		
		fprintf(stdout, "Enter action (0 for options): ");
		fscanf(stdin, "%i", &mode);
	}
	
	return 100;
}

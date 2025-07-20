#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

struct Contact {
    char label[50];
    char nr_tel[12];
    char companie[30];
    char email[50];
};

void adauga_contact(char path[]);
void afisare_contacte(char path[]);
void contacte_blocate();
void sterge_contact(char path[]);
void cauta_contact(char path[]);
bool valideaza_telefon(char* tel);
bool valideaza_email(char* email);

int main()
{	
	char path[] = "fisier.txt";
	int choice = 0;
	while(true)
	{
		system("clear");
		printf(BLUE "\n=== MENIU PRINCIPAL ===\n" RESET);
		printf("1) " GREEN "Adăugare contact" RESET "\n");
		printf("2) " RED "Ștergere contact" RESET "\n");
		printf("3) Vizualizare agendă\n");
		printf("4) " YELLOW "Contacte blocate" RESET "\n");
		printf("5) Căutare contact\n");
		printf("0) Ieșire\n");
		printf("Selectați opțiunea: ");
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:
				adauga_contact(path);
				break;
			case 2:
				sterge_contact(path);
				break;
			case 3:
				afisare_contacte(path);
				break;
			case 4:
				contacte_blocate();
				break;
			case 5:
				cauta_contact(path);
				break;
			case 0:
				printf(GREEN "La revedere!\n" RESET);
				exit(0);
			default:
				printf(RED "Opțiune invalidă!\n" RESET);
		}
		printf("\nApasa enter...");
		getchar();
		getchar();
	}
}

void adauga_contact(char path[])
{
	FILE * filePointer;
	filePointer = fopen(path, "a");

	struct Contact contact;

	system("clear");
	printf(BLUE "\n=== ADĂUGARE CONTACT ===\n" RESET);
	
	do {
		printf("Label (nume complet, minim 2 caractere): ");
		getchar();
		fgets(contact.label, sizeof(contact.label), stdin);
		contact.label[strcspn(contact.label, "\n")] = 0;
		
		if(strlen(contact.label) < 2) {
			printf(RED "Numele trebuie sa aiba minim 2 caractere!\n" RESET);
		}
	} while(strlen(contact.label) < 2);
	
	do {
		printf("Număr de telefon (format: 07xxxxxxxx sau +407xxxxxxxx): ");
		fgets(contact.nr_tel, sizeof(contact.nr_tel), stdin);
		contact.nr_tel[strcspn(contact.nr_tel, "\n")] = 0;
		
		if(!valideaza_telefon(contact.nr_tel)) {
			printf(RED "Numar invalid! Foloseste 07xxxxxxxx sau +407xxxxxxxx\n" RESET);
		}
	} while(!valideaza_telefon(contact.nr_tel));
	
	do {
		printf("Companie (minim 1 caracter): ");
		fgets(contact.companie, sizeof(contact.companie), stdin);
		contact.companie[strcspn(contact.companie, "\n")] = 0;
		
		if(strlen(contact.companie) < 1) {
			printf(RED "Compania nu poate fi goala!\n" RESET);
		}
	} while(strlen(contact.companie) < 1);
	
	do {
		printf("Email (format: nume@domeniu.com): ");
		fgets(contact.email, sizeof(contact.email), stdin);
		contact.email[strcspn(contact.email, "\n")] = 0;
		
		if(!valideaza_email(contact.email)) {
			printf(RED "Email invalid! Trebuie sa contina @ si .\n" RESET);
		}
	} while(!valideaza_email(contact.email));

	if ( filePointer == NULL )
    	{
        	printf(RED "Fisierul nu poate fi accesat.\n" RESET) ;
    	}
    	else
    	{
        	fprintf(filePointer, "%s,%s,%s,%s\n", contact.label, contact.nr_tel, contact.companie, contact.email);
        	fclose(filePointer) ;
    	}

	printf(GREEN "Contactul %s cu numărul %s a fost adăugat!\n" RESET, contact.label, contact.nr_tel);
}

void afisare_contacte(char path[])
{
	FILE * filePointer;
    	filePointer = fopen(path, "r");

    	char linie[200];
	system("clear");
	printf(BLUE "\nLISTĂ CONTACTE\n" RESET);

    	if ( filePointer == NULL )
    	{
        	printf(RED "Fisierul nu poate fi accesat.\n" RESET) ;
    	}
    	else
    	{
		int nr = 1;
		while (fgets (linie, 200, filePointer)) {
            		if (linie[0] != '\n') {
                	linie[strcspn(linie, "\n")] = '\0';
                	
                	char* nume = strtok(linie, ",");
                	char* telefon = strtok(NULL, ",");
                	char* companie = strtok(NULL, ",");
                	char* email = strtok(NULL, ",");
                	
                	printf("%d. " GREEN "%s" RESET " - " YELLOW "%s" RESET " - %s - " BLUE "%s" RESET "\n", 
                	       nr++, nume, telefon, companie, email);
            		}
        	}
        	fclose(filePointer) ;
    	}
}

void sterge_contact(char path[])
{
	system("clear");
	printf(RED "\n=== ȘTERGERE CONTACT ===\n" RESET);
	
	afisare_contacte(path);
	
	char telefon_sters[12];
	printf("Introdu numarul de telefon al contactului de sters: ");
	getchar();
	fgets(telefon_sters, sizeof(telefon_sters), stdin);
	telefon_sters[strcspn(telefon_sters, "\n")] = 0;
	
	FILE* file = fopen(path, "r");
	FILE* temp = fopen("temp.txt", "w");
	
	if(file == NULL || temp == NULL) {
		printf(RED "Eroare la deschiderea fisierelor!\n" RESET);
		return;
	}
	
	char linie[200];
	bool gasit = false;
	
	while(fgets(linie, 200, file)) {
		char linie_copy[200];
		strcpy(linie_copy, linie);
		
		char* nume = strtok(linie_copy, ",");
		char* telefon = strtok(NULL, ",");
		
		if(telefon != NULL && strcmp(telefon, telefon_sters) == 0) {
			gasit = true;
			printf(GREEN "Contact %s sters!\n" RESET, nume);
		} else {
			fprintf(temp, "%s", linie);
		}
	}
	
	fclose(file);
	fclose(temp);
	
	if(gasit) {
		remove(path);
		rename("temp.txt", path);
	} else {
		remove("temp.txt");
		printf(RED "Contactul nu a fost gasit!\n" RESET);
	}
}

void cauta_contact(char path[])
{
	system("clear");
	printf(BLUE "\n=== CĂUTARE CONTACT ===\n" RESET);
	
	char termen[50];
	printf("Introdu termenul de cautare (nume, telefon, companie sau email): ");
	getchar();
	fgets(termen, sizeof(termen), stdin);
	termen[strcspn(termen, "\n")] = 0;
	
	FILE* file = fopen(path, "r");
	if(file == NULL) {
		printf(RED "Nu pot deschide fisierul!\n" RESET);
		return;
	}
	
	char linie[200];
	bool gasit = false;
	int nr = 1;
	
	printf("\nRezultate:\n");
	while(fgets(linie, 200, file)) {
		if(strstr(linie, termen) != NULL) {
			gasit = true;
			linie[strcspn(linie, "\n")] = 0;
			
			char* nume = strtok(linie, ",");
			char* telefon = strtok(NULL, ",");
			char* companie = strtok(NULL, ",");
			char* email = strtok(NULL, ",");
			
			printf("%d. " GREEN "%s" RESET " - " YELLOW "%s" RESET " - %s - " BLUE "%s" RESET "\n", 
			       nr++, nume, telefon, companie, email);
		}
	}
	
	if(!gasit) {
		printf(RED "Nu s-au gasit contacte!\n" RESET);
	}
	
	fclose(file);
}

void contacte_blocate()
{
	char nr_tel[12];
	int choice = 0;

	do 
	{	
		system("clear");
		printf(YELLOW "\n=== CONTACTE BLOCATE ===\n" RESET);
		printf("1) " RED "Blochează contact" RESET "\n");
		printf("2) Listă contacte blocate\n");
		printf("0) Revenire la meniul principal\n");
		printf("Selectează opțiunea: ");
		scanf("%d", &choice);
		
		switch(choice)
		{
			case 1:
				system("clear");
				printf(RED "\n=== BLOCARE CONTACT ===\n" RESET);
				printf("Introdu numărul de telefon (format 07xxxxxxxx): ");
				getchar();
				fgets(nr_tel, sizeof(nr_tel), stdin);
				nr_tel[strcspn(nr_tel, "\n")] = 0;
				
				if(valideaza_telefon(nr_tel)) {
					FILE* blocked = fopen("blocked.txt", "a");
					if(blocked != NULL) {
						fprintf(blocked, "%s\n", nr_tel);
						fclose(blocked);
						printf(GREEN "Contactul %s a fost blocat!\n" RESET, nr_tel);
					}
				} else {
					printf(RED "Numar invalid!\n" RESET);
				}
				printf("Apasa enter...");
				getchar();
				break;
				
			case 2:
				system("clear");
				printf(YELLOW "\n=== LISTA BLOCATE ===\n" RESET);
				FILE* blocked = fopen("blocked.txt", "r");
				if(blocked != NULL) {
					char linie[15];
					int nr = 1;
					while(fgets(linie, 15, blocked)) {
						linie[strcspn(linie, "\n")] = 0;
						printf("%d. " RED "%s" RESET " - BLOCAT\n", nr++, linie);
					}
					fclose(blocked);
				} else {
					printf("Nu exista contacte blocate.\n");
				}
				printf("Apasa enter...");
				getchar();
				getchar();
				break;
				
			case 0:
				break;
		}
		
	} while (choice != 0);
}

bool valideaza_telefon(char* tel) 
{
	int len = strlen(tel);
	
	if(len == 10 && tel[0] == '0' && tel[1] == '7') {
		for(int i = 2; i < len; i++) {
			if(tel[i] < '0' || tel[i] > '9') return false;
		}
		return true;
	}
	
	if(len == 12 && tel[0] == '+' && tel[1] == '4' && tel[2] == '0' && tel[3] == '7') {
		for(int i = 4; i < len; i++) {
			if(tel[i] < '0' || tel[i] > '9') return false;
		}
		return true;
	}
	
	return false;
}

bool valideaza_email(char* email) 
{
	if(strlen(email) < 5) return false;
	
	char* at = strchr(email, '@');
	char* dot = strrchr(email, '.');
	
	if(at == NULL || dot == NULL) return false;
	if(at > dot) return false; 
	if(at == email || dot == email + strlen(email) - 1) return false;
	
	return true;
}

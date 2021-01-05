////////////////////////////////////
//
//	Iks-oks na tabli 4x4
//
////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	int i, j;
	int score;
} Polje;

void tutorijal();
void inicijalizuj(int [][4]);
void stamapaj(int [][4]);
int slobodna_polja(int [][4]);
int game(int [][4], int [], int);
Polje masina_potez(int [][4], int, int, int, int, int*);
int winner(int [][4]);


int main() {
	int Tabla[4][4];
	int Igrac[] = {-1, 1};
	
	int pobednik;
	
	unsigned masina;
	
	inicijalizuj(Tabla);

	do {
		printf("\nDa li zelite da igrate:\n\t1  covek protiv coveka\n\t2  protiv masine\n\nIzbor: ");
		scanf("%d", &masina);
		
		if(masina - 1 > 1) {
			printf("Pogresan unos!\n");
		}
		else break;
	} while(1);

	pobednik = game(Tabla, Igrac, masina - 1);
	
	switch(pobednik) {
		case 0 : printf("\nNereseno\n"); break;
		case -10 : masina? printf("Pobedio je covek. Svaka cast!\n") : printf("\nPobedio je prvi igrac\n"); break;
		case 10 : masina? printf("Pobedila je masina\n") : printf("\nPobedio je drugi igrac\n"); break;
	}
	return 0;
}

void inicijalizuj(int A[][4]) {
// postavljanje svih polja table na nulu
	int i, j;
	
	for(i = 0 ; i < 4 ; i ++) {
		for(j = 0 ; j < 4 ; j++)
			A[i][j] = 0;
	}
}

void stampaj(int A[][4]) {
	int i, j;
	for(i = 0 ; i < 4 ; i ++) {
		printf("\n\t");
		for(j = 0 ; j < 4 ; j++)
			printf(" %c\t", A[i][j] == -1 ? 'X' : A[i][j] == 1 ? 'O' : '-');
	}
	printf("\n\n\n\n");
}

void tutorijal() {
	printf("Dovoljno je da unesete redni broj polja da biste odigrali potez");
	int i, j;
	for(i = 0 ; i < 4 ; i ++) {
		printf("\n\t");
		for(j = 0 ; j < 4 ; j++)
		printf(" %d\t", 4*i + j);
	}
	printf("\n\n");
}

int slobodna_polja(int A[][4]) {
	int i, j;
	int brojac = 0;
	
	for(i = 0; i < 4 ; i++) {
		for(j = 0 ; j < 4 ; j++)
			if(A[i][j] == 0)
				brojac++;
	}
	return brojac;
}

int game(int A[][4], int I[], int masina) {
	int potez;
	
	int w = 0;	// tu ce biti da li imamo pobednika ili ne
	
	int TEZINA = 6;	// koristi se u funkciji masina_potez
// racunar ce 'najpametnije' igrati ukoliko je TEZINA postavljena na 0, ali ce se i program
// znatno sporije izvrsavati prvih par poteza
// TEZINA zapravo predstavlja broj praznih polja do koje ce racunar analizirati pozicije

// npr. za TEZINA = 2, racunar ce analizirati sve pozicije koje postoje zakljucno sa onim
// kojima su preostala 2 slobodna polja na tabli
	
	srand((unsigned)time(NULL));
	int igrac = rand() % 2;			// nasumicno generisemo koji igrac igra prvi
	
	int brojac = 0;				// brojac odigranih poteza
	
	Polje p;				// koristi se za poziv funkcije masina_potezi
	
	system("cls");		// brisemo sadrzaj konzole (radi samo na Windows-u)
				// isti efekat dobijamo na Linux-u komandom system("clear");
	
	while(++brojac % 17) {
		if(igrac && masina) {
			if(brojac == 1) {	// ukoliko masina prva igra, prvi potez ce joj biti nasumican
						// jer bi u suprotnom uvek igru zapocinjala istim potezom
				int random = rand() % 16;
				A[random / 4][random % 4] = 1;
				TEZINA -= 2;
			}
			else {
				p = masina_potez(A, igrac, slobodna_polja(A), -10000, 10000, &TEZINA);
				A[p.i][p.j] = 1;
				if(brojac == 2) TEZINA = 2;
				else TEZINA = 0;
				
			}
		}
		else {
			tutorijal();
			
			printf("\nIgra igrac %c : ", igrac? 'O' : 'X');
			scanf("%d", &potez);
			
			system("cls");
			
			if(potez < 0 || potez > 15) {
				printf("Uneli ste broj van opsega!\n");
				brojac--;
				
				stampaj(A);
				
				continue;
			}
			else if(A[potez / 4][potez % 4] != 0) {
				printf("Dato polje je zauzeto!\n");
				brojac--;
				
				stampaj(A);
				
				continue;
			}
			
			// igramo potez
			A[potez / 4][potez % 4] = I[igrac];
			
		}
		stampaj(A);	// ukoliko poziv ove 2 funkcije stavimo na pocetak petlje onda kada
				// jedan od igraca pobedi, necemo videti poslednji odigran potez
		
		w = winner(A);
		if(w) return w;
		
		igrac = !igrac;	// menjamo koji igrac igra
	
	}
}

Polje masina_potez(int A[][4], int igrac, int depth, int alfa, int beta, int *TEZINA) {

// za pronalazenje najboljeg poteza sam koristio minimax algoritam sa alfa-beta izbacivanjem
// depth predstavlja broj preostalih praznih mesta na tabli


	Polje best_score, score;
	int i, j;
	
	// postavljamo inicijalne vrednosti
	best_score.i = -1;
	best_score.j = -1;
	score.i = -1;
	score.j = -1;
	
	if(depth <= *TEZINA) {
		score.score = winner(A) * (slobodna_polja(A) + 1);
		return score;
	}
	else {
		int w = winner(A);
		if(w) { 
			score.score = w * (slobodna_polja(A) + 1);	// odredjemo koliko je potez dobar
						// ukoliko dobijemo veliku pozitivnu vrednost to znaci da je taj potez dobar za masinu
			return score;
		}
	}
	
	if(igrac) {					// masina
		best_score.score = -10000;		// inicijalna vrednost
		for(i = 0 ; i < 4 ; i++) {
			for(j = 0 ; j < 4 ; j++) {
				if(A[i][j] == 0) {
					
					A[i][j] = 1;
					score = masina_potez(A, !igrac, depth - 1, alfa, beta, TEZINA);
					
					A[i][j] = 0;
					
					if(score.score > best_score.score) {
						best_score.i = i;
						best_score.j = j;
						best_score.score = score.score;
					}
					
					if(score.score >= beta)
						return best_score;
					
					if(score.score >= alfa)
						alfa = score.score;
					
				}
			}
		}
		return best_score;
	}
	else {						// 'covek'	ova grana simulira najbolji odgovor na potez masine
		best_score.score = 10000;		// inicijalna vrednost
		for(i = 0 ; i < 4 ; i++) {
			for(j = 0 ; j < 4 ; j++) {
				if(A[i][j] == 0) {
					
					A[i][j] = -1;
					score = masina_potez(A, !igrac, depth-1, alfa, beta, TEZINA);
					
					A[i][j] = 0;
					
					if(score.score < best_score.score) {
						best_score.i = i;
						best_score.j = j;
						best_score.score = score.score;
					}
					
					if(score.score <= alfa)
						return best_score;
						
					if(score.score < beta)
						beta = score.score;
					
				}
			}
		}
		return best_score;
	}
}

int winner(int A[][4]) {
//	ako funkcija vrati -10 onda pobedio je prvi igrac, tj. X
//	ako funkcija vrati 10 onda pobedio je drugi igrac ili masina, tj. O


	int i, j;
	int tmp;
	
	int ver_broj = 0;	// ispitujemo da li je red popunjen istim brojem
	int hor_broj = 0;	// ispitujemo da li je vrsta popunjena istim brojem
	

	// proveravamo glavnu dijagonalu
	tmp = A[0][0] + A[1][1] + A[2][2] + A[3][3];
	
	if(tmp == -4) return -10;
	if(tmp == 4) return 10;
	
	// proveravamo sporednu dijagonalu
	tmp = A[0][3] + A[1][2] + A[2][1] + A[3][0];
	
	if(tmp == -4) return -10;
	if(tmp == 4) return 10;	
	
	
	// redovi i kolone
	for(i = 0 ; i < 4 ; i++) {
		hor_broj = A[i][0];
		ver_broj = A[0][i];
		
		for(j = 0 ; j < 4 ; j++) {
			
			// redovi
			if(hor_broj != A[i][j]) {	// ukoliko je sledeci broj u redu razlicit od prethodnog
				hor_broj = 0;		// onda ne mozemo da imamo 4 ista broja zaredom
			}
			
			//kolone
			if(ver_broj != A[j][i]) {
				ver_broj = 0;
			}
			
			
			// ispitujemo kvadrate
			if(i < 3 && j <3) {
				tmp = A[i][j] + A[i + 1][j] + A[i][j + 1] + A[i + 1][j + 1];
				if(tmp == -4) return -10;
				if(tmp == 4) return 10;
			}	
		}
		if(hor_broj != 0) {
			if(hor_broj == 1)
				return 10;
			else
				return -10;
		}
			
		if(ver_broj != 0) {
			if(ver_broj == 1)
				return 10;
			else
				return -10;
		}
		
	}
	
	return 0;	// vracamo 0 ako trenutno nemamo pobednika
}


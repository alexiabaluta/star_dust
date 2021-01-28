// Copyright 2020 Alexia-Elena Baluta 311CA

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int get_border_sum(int **map, int *m, int n);

void task_modify(char data_type, int **line_ptr, int data_index, void *new_val,
				 int *line_size);

void task_swap(char data_type, int *line_ptr, int data_index);

void task_delete(char data_type, int *line_ptr, int data_index);

void make_query(int **map, int *m);

void flood_fill(char **map_in_bytes, int n, int *m, int line, int row,
				int *spr_msv);

int main(void)
{
	int n, *m = NULL, **map = NULL;
	scanf("%d", &n);
	alloc_vect(n, &m);
	map = read_matrix(n, m);

	// task 1
	int num_bytes = sizeof(int) * (m[0] + m[n - 1]) + 2 * (n - 2);
	int border_sum = get_border_sum(map, m, n);
	double bytes_avg = (double)border_sum / num_bytes;
	printf("task 1\n%0.8f\n", bytes_avg);

	// task 2
	int k;
	scanf("%d", &k);
	for (int i = 0; i < k; i++)
		make_query(map, m);
	printf("task 2\n");
	print_matrix(n, m, map);

	// task 3
	int s_i, s_j, s = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m[i] * (int)sizeof(int); j++) {
			int spr_msv = 0;
			flood_fill((char **)map, n, m, i, j, &spr_msv);
			if (spr_msv > s) {
				s = spr_msv;
				s_i = i;
				s_j = j;
			}
		}
	}
	printf("task 3\n%d %d %d\n", s_i, s_j, s);

	// se salveaza niste ponei aici
	map = free_matrix(n, map);
	free(m);
	return 0;
}

// Se calculeaza suma octetilor de pe conturul hartii.
// Se aduna toti octetii de pe prima si ultima linie, iar de pe liniile din
// interior se aduna doar primul si ultimul octet.
int get_border_sum(int **map, int *m, int n)
{
	int border_sum = 0;

	border_sum += add_bytes(map[0], m[0] * sizeof(int));
	border_sum += add_bytes(map[n - 1], m[n - 1] * sizeof(int));

	for (int i = 1; i < n - 1; i++) {
		// p primeste adresa primului element, apoi a ultimului byte.
		char *p = (char *)(map[i] + m[i] - 1);
		p += 3;

		border_sum += add_bytes(map[i], sizeof(char));
		border_sum += add_bytes(p, sizeof(char));
	}

	return border_sum;
}

// Se modifica un bloc de date de dimensiune data, de pe pozitia data_index - 1
// cu o valoare data.
// Daca linia data nu are dimensiunea suficienta, se face o realocare de memorie
// si blocurile noi se completeaza cu 0.
void task_modify(char data_type, int **line_ptr, int data_index, void *new_val,
				 int *line_size)
{
	unsigned long data_size = (data_type == 'C') ? sizeof(char) :
							  ((data_type == 'S') ? sizeof(short) :
							  sizeof(int));

	// Cazul in care nu este suficienta memorie.
	if (data_index * data_size > (*line_size) * sizeof(int)) {
		// Linia finala trebuie sa memoreze int-uri, de aceea noua dimensiune
		// trebuie sa fie divizibila cu sizeof(int).
		int new_size = (data_index - 1) * data_size / sizeof(int) + 1;

		// Se utilizeaza o variabila temporara pentru realocarea memoriei.
		int *tmp = *line_ptr;
		tmp = realloc(*line_ptr, new_size * sizeof(int));
		if (!(*tmp)) {
			fprintf(stderr, "malloc() failed");
			return;
		}
		*line_ptr = tmp;

		for (int i = *line_size; i < new_size; i++)
			tmp[i] = 0;

		// Se modifica in vectorul de dimensiuni valoarea liniei curente.
		*line_size = new_size;
	}

	// Se modifica valoarea de pe pozitia data_type - 1 in functie de data_type
	if (data_type == 'C') {
		char *p = (char *)*line_ptr;
		mem_cpy(p + data_index - 1, new_val, sizeof(char));
	} else if (data_type == 'S') {
		short *p = (short *)*line_ptr;
		mem_cpy(p + data_index - 1, new_val, sizeof(short));
	} else {
		mem_cpy(*line_ptr + data_index - 1, new_val, sizeof(int));
	}
}

// Se inverseaza octetii dintr-un bloc de memorie de dimensiune data, de pe
// pozitia data_index.
void task_swap(char data_type, int *line_ptr, int data_index)
{
	char *byte = NULL;

	if (data_type == 'S') {
		// Tipul short are 2 octeti.
		// Este suficient sa se interschimbe valorile celor 2 octeti.
		short *sh_ptr = (short *)line_ptr;
		byte = (char *)&sh_ptr[data_index];
		swap_byte(byte, byte + 1);
	} else if (data_type == 'I') {
		// Tipul int are 4 octeti.
		// Este nevoie de 2 interschimbari.
		byte = (char *)&line_ptr[data_index];
		swap_byte(byte, byte + 3);
		swap_byte(byte + 1, byte + 2);
	}
	// Tipul char are un singur byte, deci nu este nevoie de nici o modificare.
}

// Se suprascrie 0 peste un bloc de date de dimensiune data, de pe pozitia
// data_index - 1.
void task_delete(char data_type, int *line_ptr, int data_index)
{
	char *char_ptr = NULL;
	short *short_ptr = NULL;

	switch (data_type) {
	case 'C':
		// chr_ptr primeste adresa blocului de date care trebuie modificat
		char_ptr = (char *)line_ptr;
		char_ptr[data_index - 1] = 0;
		break;

	case 'S':
		// sh_ptr primeste adresa blocului de date care trebuie modificat
		short_ptr = (short *)line_ptr;
		short_ptr[data_index - 1] = 0;
		break;

	case 'I':
		line_ptr[data_index - 1] = 0;
		break;

	default:
		break;
	}
}

// Se primesc modificarie, se decodifica sensul lor si se trimit datele de
// intrare la functiile care indeplinesc operatiile cerute.
void make_query(int **map, int *m)
{
	char op, data_type;
	int line, data_index, new_val;
	scanf(" %c %c %d %d", &op, &data_type, &line, &data_index);

	if (op == 'M') {
		scanf("%X", &new_val);
		task_modify(data_type, &map[line], data_index, &new_val, &m[line]);
	} else if (op == 'S') {
		task_swap(data_type, map[line], data_index);
	} else {
		task_delete(data_type, map[line], data_index);
	}
}

// Functia cauta, marcheaza si numara recursiv toate gaurile negre incepand de
// pe pozitia de coordonate (line, row)
void flood_fill(char **map_in_bytes, int n, int *m, int line, int row,
				int *spr_msv)
{
	// Se verifica daca blocul a mai fost parcurs sau daca nu e gaura neagra.
	if (map_in_bytes[line][row])
		return;

	// Se marcheaza blocul parcurs si se contorizeaza gaura neagra.
	map_in_bytes[line][row] = 1;
	(*spr_msv)++;

	// Vectori de deplasare
	// Codifica directiile STG, SUS, DR, JOS.
	int line_step[] = {0, 1, 0, -1};
	int row_step[] = {-1, 0, 1, 0};

	// Se itereaza cele 4 directii de deplasare.
	for (int k = 0; k < 4; k++) {
		// Se calculeaza coordonatele urmatorului pas.
		int next_line_step = line + line_step[k];
		int row_pos = row + row_step[k];

		// Se verifica daca urmatorul pas iese din limitele liniei.
		if (next_line_step < 0 || next_line_step >= n)
			continue;

		// Se verifica daca urmatorul pas iese din limitele coloanei.
		if (row_pos >= 0 && row_pos < m[next_line_step] * (int)sizeof(int))
			flood_fill(map_in_bytes, n, m, next_line_step, row_pos, spr_msv);
	}
}

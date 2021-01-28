// Copyright 2020 Alexia-Elena Baluta 311CA

#include <stdio.h>
#include <stdlib.h>

// Se calculeaza suma unei secvente de octeti de dimensiune data care incepe de
// la o anumita adresa.
int add_bytes(void *src, int num_bytes)
{
	int bytes_sum = 0;
	char *byte = (char *)src;
	for (int i = 0; i < num_bytes; i++)
		bytes_sum += (int)byte[i];
	return bytes_sum;
}

// Se copiaza datele unui bloc de memorie sursa, de dimensiune adresa date,
// peste un bloc de memorie destinatie.
void mem_cpy(void *dst, const void *src, int num_bytes)
{
	// Datele de intrare nu sunt valide.
	if (!dst || !src)
		return;

	char *dst_byte = (char *)dst;
	char *src_byte = (char *)src;

	// Se itereaza fiecare octet si se copiaza octetul din sursa in destinatie
	for (int i = 0; i < num_bytes; i++) {
		*dst_byte = *src_byte;
		dst_byte++;
		src_byte++;
	}
}

// Interschimba doi octeti
// Se utilizeaza char*, deoarece dimensiunea unui char este de un octet.
void swap_byte(char *a, char *b)
{
	char tmp = *a;
	*a = *b;
	*b = tmp;
}

// Se aloca dinamic un vector de numere intregi.
void alloc_vect(int n, int **v)
{
	int *vv = (int *)malloc(n * sizeof(int));
	if (!vv) {
		fprintf(stderr, "malloc() failed");
		return;
	}
	*v = vv;
}

// Se aloca dinamic si se citesc elementele unei matrice.
// Dimensiunea fiecarei linii este memorata intr-un vector separat.
int **read_matrix(int n, int *m)
{
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		fprintf(stderr, "malloc() failed");
		return NULL;
	}

	for (int i = 0; i < n; i++) {
		scanf("%d", m + i);
		a[i] = (int *)malloc(m[i] * sizeof(int));
		if (!a) {
			fprintf(stderr, "malloc() failed");
			return NULL;
		}
		for (int j = 0; j < m[i]; j++)
			scanf("%X", &a[i][j]);
	}
	return a;
}

// Se printeaza o matrice care are dimensiunea liniilor stocate intr-un vector.
void print_matrix(int n, int *m, int **a)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m[i]; j++)
			printf("%08X ", a[i][j]);
		printf("\n");
	}
}

// Se dezaloca spatiul utilizat de o matrice alocata dinamic
int **free_matrix(int n, int **a)
{
	for (int i = 0; i < n; i++)
		free(a[i]);
	free(a);
	return NULL;
}

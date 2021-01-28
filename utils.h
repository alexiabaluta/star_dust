// Copyright 2020 Alexia-Elena Baluta 311CA
// Biblioteca contine o serie de functii utilitare pentru lucrul cu octeti
// si alocare dinamica.

int add_bytes(void *src, int num_bytes);

void mem_cpy(void *dst, const void *src, int num_bytes);

void swap_byte(char *a, char *b);

void alloc_vect(int n, int **v);

int **read_matrix(int n, int *m);

void print_matrix(int n, int *m, int **a);

int **free_matrix(int n, int **a);

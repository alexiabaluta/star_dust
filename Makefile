# Copyright 2020 Alexia-Elena Baluta 311CA

CFLAGS=-Wall -Wextra -std=c99

build:
	gcc $(CFLAGS) -o star_dust utils.c star_dust.c

pack:
	zip -FSr 311CA_BalutaAlexia_Tema2.zip README Makefile *.c *.h

clean:
	rm -f star_dust utils

.PHONY: pack clean
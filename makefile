all:
	gcc coin_em.c -std=c99 -o coin_em -lm -g
clean:
	-rm coin_em

all:
	gcc dice_em.c -std=c99 -o dice_em -lm -g
clean:
	-rm dice_em

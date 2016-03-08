default:
	gcc -O3 -o eqtide *.c -lm

debug:
	gcc -g -o eqtide *.c -lm

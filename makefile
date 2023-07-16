main: main.c
	gcc main.c -o ccr
	./ccr

debug: main.c
	gcc -g main.c -o ccr
	gdb ccr

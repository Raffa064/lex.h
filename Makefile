test: test.c lex.h
	cc test.c -o test

run: test
	./test

.PHONY: run

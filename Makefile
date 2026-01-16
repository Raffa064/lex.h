build/%: showcases/% | build lex.h
	cc $^ -o $@

build:
	mkdir -p build

run: build/$(SHOWCASE)
	@if [ -z '$(SHOWCASE)' ]; then\
		echo "Available showcases: (Run with 'make SHOWCASE=name')";\
		ls ./showcases;\
	else\
		./build/$(SHOWCASE);\
	fi

.PHONY: run

SHOWCASE := $(SHOWCASE)$(S)

run: build/$(SHOWCASE)
	@if [ -z '$(SHOWCASE)' ]; then\
		echo "Available showcases: (Run with 'make SHOWCASE=name')";\
		ls ./showcases;\
	else\
		./build/$(SHOWCASE);\
	fi

build/%: showcases/% lex.h | build
	cc $^ -o $@

build:
	mkdir -p build


.PHONY: run

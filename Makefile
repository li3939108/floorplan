cc=gcc
fp : fp.c random.c sa.c 
	$(cc) -g $+ -o -lm $@

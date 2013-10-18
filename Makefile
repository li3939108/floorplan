cc=gcc
fp : fp.c random.c
	$(cc) -g $+ -o $@

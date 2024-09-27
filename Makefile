m800pro-drv: main.c
	cc -lusb-1.0 main.c -o $@

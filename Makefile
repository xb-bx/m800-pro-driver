m800pro-drv: main.c buttons.h keys.h
	cc -lusb-1.0 main.c -o $@

buttons.h: buttons.json gen-buttons.sh
	./gen-buttons.sh
keys.h: keys.json gen-keys.sh
	./gen-keys.sh

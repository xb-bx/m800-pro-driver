m800pro-drv: main.c buttons.h keys.h
	cc -lusb-1.0 main.c -o $@

.PHONY: clean
clean:
	rm -r m800pro-drv


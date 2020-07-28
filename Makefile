CC = gcc
src_dir = src
src = $(wildcard $(src_dir)/*.c)
obj = $(src:.c=.o)

#CFLAGS =`pkg-config --cflags --libs python3` `pkg-config --cflags --libs gtk+-3.0` -Iinc/
CFLAGS =`python3.8-config --cflags --ldflags --embed` `pkg-config --cflags --libs gtk+-3.0` -Iinc/ -fPIE

browser: $(obj)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) browser
	rm -fr src/*.o

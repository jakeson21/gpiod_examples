all: gpiod_4lines gpiod_bulk

gpiod_4lines: gpiod_4lines.cpp
	g++ gpiod_4lines.cpp -O0 -g -fPIC -I. -lgpiod -o gpiod_4lines

gpiod_bulk: gpiod_bulk.cpp
	g++ gpiod_bulk.cpp  -O0 -g -fPIC -I. -lgpiod -o gpiod_bulk
clean:
	rm -f gpiod_4lines gpiod_bulk

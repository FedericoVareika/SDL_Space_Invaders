build: 
	gcc -Wall -std=c99 -I/opt/homebrew/include -L/opt/homebrew/lib -o spaceinvaders ./src/*.c -lSDL2

run:
	./spaceinvaders

clean: 
	rm spaceinvaders

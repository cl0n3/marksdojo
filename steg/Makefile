all: clean compile

clean:
	@rm -rf *o steg

compile:
	@gcc steg.c bitmap.c -o steg

run: compile
	./steg frog.bmp smalltest.txt cipherfrog.bmp
	./steg cipherfrog.bmp decode.txt
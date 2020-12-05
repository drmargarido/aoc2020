all:
	ls -1 | grep .c | grep -v utils | cut -d. -f1 | xargs -I{} gcc {}.c -lm -o {}

clean:
	ls -1 | grep .c | grep -v utils | cut -d. -f1 | xargs -I{} rm -f {}

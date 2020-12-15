all:
	ls -1 | grep .c | grep -v utils | grep -v vec2 | cut -d. -f1 | xargs -I{} gcc -O3 -march=native {}.c -lm -o {}

debug:
	ls -1 | grep .c | grep -v utils | grep -v vec2 | cut -d. -f1 | xargs -I{} gcc -g {}.c -lm -o {}

clean:
	ls -1 | grep .c | grep -v utils | grep -v vec2 | cut -d. -f1 | xargs -I{} rm -f {}

test: all
	ls -1 | grep .c | grep -v utils | grep -v vec2 | cut -d. -f1 | xargs -I{} bash -c "echo '---- {} ----' && time ./{}"


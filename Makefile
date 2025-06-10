.POSIX:
.SUFFIXES:
.PHONY: \
	examples \
	clean \
	format

examples:
	time sh sage-build raylib examples/readme.c
	time sh sage-build raylib examples/simple.c
	time sh sage-build raylib examples/simple.sunder
	time sh sage-build web examples/readme.c
	time sh sage-build web examples/simple.c
	time sh sage-build web examples/simple.sunder

clean:
	rm -f examples/*.out
	rm -f examples/*.html

format:
	clang-format -i *.h *.c examples/*.c

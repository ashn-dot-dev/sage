.POSIX:
.SUFFIXES:
.PHONY: \
	examples \
	clean \
	format

examples:
	sh sage-build raylib examples/readme.c
	sh sage-build raylib examples/simple.c
	sh sage-build raylib examples/simple.sunder
	sh sage-build web examples/readme.c
	sh sage-build web examples/simple.c
	sh sage-build web examples/simple.sunder

clean:
	rm -f examples/*.out
	rm -f examples/*.html

format:
	clang-format -i *.h *.c examples/*.c

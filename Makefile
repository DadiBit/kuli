clean:
	rm -f test.kulimap test.kulidata write.out read.out

write:
	gcc -O0 src/kuli.c tests/write.c -o write.out \
	&& ./write.out

read:
	gcc -O0 src/kuli.c tests/read.c -o read.out \
	&& ./read.out
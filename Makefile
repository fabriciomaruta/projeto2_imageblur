
all:
	gcc -omain_p main_multiprocess.c imageprocessing.c blur.c -I./ -lfreeimage
	gcc -omain_t main_thread.c imageprocessing.c blur.c -I./ -lfreeimage -lpthread
	gcc -omain main.c imageprocessing.c blur.c -I./ -lfreeimage
test:all
	sh script.sh
clean:
	rm -f main
	rm -f main_p
	rm -f main_t
	rm -f blur
	rm -fr out

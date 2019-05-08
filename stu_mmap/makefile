.PHONY:all
all:mmap mmap_ipc mmap_noname
mmap_noname:mmap_noname.c
	gcc $^ -o $@
mmap_ipc:mmap_ipc.c
	gcc $^ -o $@
mmap:mmap.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm -rf mmap mmap_ipc mmap_noname

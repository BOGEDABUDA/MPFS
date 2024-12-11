CFLAGS=-O0 -g -fPIC -mclwb -mclflushopt -Wall -pthread -mavx512f #-DRAM_VER
# CFLAGS=-O3 -fPIC -mclwb -mclflushopt -Wall -pthread -mavx512f #-DRAM_VER
GCC=gcc

all: libctfs.so mpfs_test mkfs fstest mpfs_fstest


libctfs.so: ctfs.a ctfs_wrapper.c ffile.o
	$(GCC) -shared $(CFLAGS) -o bld/libctfs.so ctfs_wrapper.c bld/ctfs.a bld/ffile.o -ldl

ctfs.a: ctfs_bitmap.o ctfs_func.o ctfs_inode.o ctfs_pgg.o ctfs_runtime.o lib_dax.o ctfs_cpy.o ctfs_func2.o mpfs_parse.o 
	ar cru bld/ctfs.a bld/ctfs_bitmap.o bld/ctfs_func.o bld/ctfs_inode.o bld/ctfs_pgg.o bld/ctfs_runtime.o bld/lib_dax.o bld/ctfs_cpy.o bld/ctfs_func2.o bld/mpfs_parse.o 


ctfs_bitmap.o: ctfs_bitmap.c
	$(GCC) -c $(CFLAGS) ctfs_bitmap.c -o bld/ctfs_bitmap.o

ctfs_func.o: ctfs_func.c mpfs_main_task.c
	$(GCC) -c $(CFLAGS) ctfs_func.c -o bld/ctfs_func.o

ctfs_func2.o: ctfs_func2.c
	$(GCC) -c $(CFLAGS) ctfs_func2.c -o bld/ctfs_func2.o
	
ctfs_inode.o: ctfs_inode.c
	$(GCC) -c $(CFLAGS) ctfs_inode.c -o bld/ctfs_inode.o

ctfs_pgg.o: ctfs_pgg.c
	$(GCC) -c $(CFLAGS) ctfs_pgg.c -o bld/ctfs_pgg.o

ctfs_runtime.o: ctfs_runtime.c
	$(GCC) -c $(CFLAGS) ctfs_runtime.c -o bld/ctfs_runtime.o

lib_dax.o: lib_dax.c
	$(GCC) -c $(CFLAGS) lib_dax.c -o bld/lib_dax.o

ctfs_cpy.o: ctfs_cpy.c
	$(GCC) -c ctfs_cpy.c -g -mclwb -Wall -pthread -mavx512f -o bld/ctfs_cpy.o

ffile.o: glibc/ffile.c
	cd glibc && $(MAKE)
# mpfs-supplement
mpfs_parse.o: mpfs_parse.c mpfs_parse.h mpfs_main_task.h
	$(GCC) -c $(CFLAGS) mpfs_parse.c -o bld/mpfs_parse.o

mpfs_test.o: mpfs_test.c
	$(GCC) -c $(CFLAGS) mpfs_test.c  -o bld/mpfs_test.o

mpfs_test: ctfs.a mpfs_test.o
	$(GCC) $(CFLAGS) bld/mpfs_test.o -lc -lctfs -L./bld -o mpfs_test

# entering subdirectory to build mkfs
mkfs: ctfs.a test/mkfs.c
	$(MAKE) -C test VAR=mkfs

fstest: ctfs.a test/fstest.c
	$(MAKE) -C test VAR=fstest

mpfs_fstest: ctfs.a test/mpfs_fstest.c
	$(MAKE) -C test VAR=mpfs_fstest

clean:
	rm -r bld/* && rm test/mkfs && rm test/fstest && rm mpfs_test

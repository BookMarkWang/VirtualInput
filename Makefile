all:
	make -fbuild_virtlib.mk
	make -fbuild_virtin.mk
	make -fbuild_virtkey.mk
clean:
	make -fbuild_virtlib.mk clean
	make -fbuild_virtin.mk clean
	make -fbuild_virtkey.mk clean

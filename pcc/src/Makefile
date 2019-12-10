libipt_pcc.so: libipt_pcc.o
	$(CC) -shared -lxtables -o $@ $^;

libipt_pcc.o: libipt_pcc.c
	$(CC) ${CFLAGS} -fPIC -D_INIT=$*_init -c -o $@ $<;

obj-m += xt_pcc.o

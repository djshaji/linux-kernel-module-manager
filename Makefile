
modman:
	cc -c *.c `libapril-config --cflags` -w
	cc *.o `libapril-config --libs ` -o Modman -w

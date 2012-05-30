CC=gcc
CFLAGS=-shared -fPIC -o lib_mysqludf_haversine.so 
MYSQLCONFIG=$$(mysql_config --cflags) -lm -DMYSQL_DYNAMIC_PLUGIN 
main: 
	$(CC) $(CFLAGS) lib_mysqludf_haversine.c $(MYSQLCONFIG)

clean:
	rm -f lib_mysqludf_haversine.so

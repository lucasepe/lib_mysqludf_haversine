lib_mysqludf_haversine
======================

MySQL UDF to compute the [Haversine formula](http://en.wikipedia.org/wiki/Haversine_formula)

Returns the haversine distance from two points

Note: MySQL 5.7 introduced the native [`st_distance_sphere`](https://dev.mysql.com/doc/refman/5.7/en/spatial-convenience-functions.html#function_st-distance-sphere) function, so this project here is not required anymore.

Input parameters
----------------

lat1 (real), lng1 (real), lat2 (real), lng2 (real), type (string - optinal - 'km', 'ft', 'mi')

Output
------

Distance between the two points. The measurement is returned in kilometers (real)
by default, however if you specify the 5th *type* argument you may return in other
measurements.

Install
=======

To compile
----------

    gcc -shared -fPIC -o lib_mysqludf_haversine.so lib_mysqludf_haversine.c $(mysql_config --cflags) \
        -lm -DMYSQL_DYNAMIC_PLUGIN

To register this function
-------------------------

At mysql commandline prompt, type:

    CREATE FUNCTION haversine_distance RETURNS REAL SONAME 'lib_mysqludf_haversine.so';

To get rid of the function
--------------------------

At mysql commandline prompt, type:

    DROP FUNCTION haversine_distance;

To list all installed functions
-------------------------------

At mysql commandline prompt, type:

    SELECT  FROM mysql.func;

Troubleshooting
---------------

If you have permission problems installing function such as:

    ERROR 1126 (HY000): Can't open shared library 'lib_mysqludf_haversine.so' 
    (errno: 22 /usr/lib/mysql/plugin/lib_mysqludf_haversine.so: failed to map segment from shared object: Permission denied)

Follow this steps:

 1. type: `sudo vi /etc/apparmor.d/usr.sbin.mysqld`;
 2. add this line: `/usr/lib/mysql/plugin/** mr,` and save;
 3. type: `sudo /etc/init.d/apparmor restart`

then register the function in MySQL.

How to use
==========

Once installed, to calculate distance between two Latitude/Longitude points:

    SELECT id, name FROM MY_PLACES WHERE haversine_distance(lat1, lng1, lat2, lng2) < 40;

The `SELECT` will return all the records with a distance less then 40 kilometers.

    SELECT id, name FROM MY_PLACES WHERE haversine_distance(lat1, lng1, lat2, lng2, 'ft') < 25;

The `SELET` will return all the records with a distance less then 25 feet.

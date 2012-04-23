/*
 * * lib_mysqludf_haversine.c:  MySQL UDF to compute the Haversine formula.
 *
 * Copyright (C) 2012 by Luca Sepe ( luca.sepe@gmail.com )
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 *
 * To compile:
 *
 *	gcc -shared -fPIC -o lib_mysqludf_haversine.so lib_mysqludf_haversine.c $(mysql_config --cflags) -lm -DMYSQL_DYNAMIC_PLUGIN
 *
 *
 *	returns the haversine distance from two points 
 *
 * 		input parameters:
 * 			lat1 (real)
 * 			lng1 (real)
 * 			lat2 (real)
 * 			lng2 (real)
 *
 *
 * 		output:
 * 			distance in kilometers (real)
 *
 *
 * To register this function:
 *
 * 		CREATE FUNCTION haversine_distance RETURNS REAL SONAME 'lib_mysqludf_haversine.so';
 *
 * To get rid of the function:
 *
 * 		DROP FUNCTION haversine_distance;
 *
 * To list all installed functions:
 *
 *		SELECT * FROM mysql.func;
 *
 *
 * If you have permission problems installing function such as:
 *
 *	ERROR 1126 (HY000): Can't open shared library 'lib_mysqludf_haversine.so' 
 *				(errno: 22 /usr/lib/mysql/plugin/lib_mysqludf_haversine.so: failed to map segment from shared object: Permission denied)
 *
 * Follow this steps:
 *
 *		[1] - sudo vi /etc/apparmor.d/usr.sbin.mysqld
 *		[2] - add:  /usr/lib/mysql/plugin/** mr,
 *		[3] - sudo /etc/init.d/apparmor restart
 *
 *	then register the function in MySQL
 */


#ifdef STANDARD
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;	
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#endif
#include <mysql.h>
#include <m_ctype.h>
#include <m_string.h>		
#include <ctype.h>

#include <math.h>

#ifdef HAVE_DLOPEN


my_bool 
haversine_distance_init( UDF_INIT* initid, UDF_ARGS* args, char* message );

void 
haversine_distance_deinit( UDF_INIT* initid );

double 
haversine_distance( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );



my_bool 
haversine_distance_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {

	if ( args->arg_count != 4 ) {
		strcpy(message,"wrong number of arguments: haversine_distance() requires four arguments");
    	return 1;
  	}

  	if ( (args->arg_type[0] != REAL_RESULT) ) {
		//strcpy( message, "haversine_distance() requires real as parameters 1" ); return 1;
		args->arg_type[0] = REAL_RESULT;
  	}

  	if ( (args->arg_type[1] != REAL_RESULT) ) {
		//strcpy( message, "haversine_distance() requires real as parameters 2" ); return 1;
		args->arg_type[1] = REAL_RESULT;
  	}

  	if ( (args->arg_type[2] != REAL_RESULT) ) {
		//strcpy( message, "haversine_distance() requires real as parameters 3" ); return 1;
		args->arg_type[2] = REAL_RESULT;
  	}

  	if ( (args->arg_type[3] != REAL_RESULT) ) {
		//strcpy( message, "haversine_distance() requires real as parameters 4" ); return 1;
		args->arg_type[3] = REAL_RESULT;
  	}

  	double *dist = malloc( sizeof(double) );
	if ( dist == NULL ) {
		strcpy(message, "haversine_distance() allocation error");
		return 1;
	}

	*dist = -1.0;

	initid->decimals 	= 6;
	initid->maybe_null	= 1;
  	initid->max_length	= 64;
	initid->ptr = (char*)dist;

	return 0;
}


void 
haversine_distance_deinit( UDF_INIT* initid ) {
	free( initid->ptr );
}


double 
haversine_distance( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error ) {

	double result = *(double*) initid->ptr;
	
	/*Earth Radius in Kilometers.*/
	double R = 6372.797560856;
	
	double DEG_TO_RAD = M_PI/180.0;
	double RAD_TO_DEG = 180.0/M_PI;
  
	double lat1 = *(double*) args->args[0];
	double lon1 = *(double*) args->args[1];
	double lat2 = *(double*) args->args[2];
	double lon2 = *(double*) args->args[3];

	double dlon = (lon2 - lon1) * DEG_TO_RAD;
	double dlat = (lat2 - lat1) * DEG_TO_RAD;
	double a =  pow(sin(dlat * 0.5),2)+ cos(lat1*DEG_TO_RAD) * cos(lat2*DEG_TO_RAD) * pow(sin(dlon * 0.5),2);
	double c = 2.0 * atan2(sqrt(a), sqrt(1-a));
	
	result = ( R * c );

	return result;
}


#endif

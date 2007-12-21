#include <CoreFoundation/CoreFoundation.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

/*
 * parseline: a simple argument parser.
 *
 * parameters:
 *	line: buffer containing string to parse. Modified.
 *	lineav: argv resulting from parsing.
 *
 * return value:
 *	 0: success, line broken into vector of 3 arguments
 *	-1: failed
 *
 */
    int
parseline( char *line, char ***lineav )
{
    static char		**pav = NULL;
    int			i; 

    if ( pav == NULL ) {
	if (( pav = ( char ** )malloc( 3 * sizeof( char * ))) == NULL ) {
	    perror( "malloc" );
	    exit( 2 );
	}
    }

    pav[ 0 ] = line;

    for ( i = 1; i < 3; i++ ) {
	while ( isspace( *line )) line++;
	while ( *line != '\0' && !isspace( *line )) line++;
	*line++ = '\0';
	while ( isspace( *line )) line++;

	pav[ i ] = line;
	if ( *pav[ i ] == '\0' ) {
	    break;
	}
    }
    if ( i != 3 ) {
	return( -1 );
    }

    *lineav = pav;

    return( 0 );
}

/*
 * c2cf: convert a C string to CFStringRef
 *
 * parameters:
 *	cstr: the C string to convert
 *	cfstr: will point to result of conversion. must be CFRelease'd.
 *
 * return value:
 *	 0: success
 *	-1: failure
 */
    int
c2cf( char *cstr, CFStringRef *cfstr )
{
    CFStringRef		cftmp;

    if ( cstr == NULL ) {
	fprintf( stderr, "%s: null C String\n", __FUNCTION__ );
	return( -1 );
    }

    if (( cftmp = CFStringCreateWithBytes( kCFAllocatorDefault,
			( UInt8 * )cstr, ( CFIndex )strlen( cstr ),
			kCFStringEncodingUTF8, false )) == NULL ) {
	fprintf( stderr, "Failed to convert C string to CFStringRef\n" );
	return( -1 );
    }

    *cfstr = cftmp;

    return( 0 );
}

/*
 * cf2c: convert a CFStringRef to a C string
 *
 * parameters:
 *	cfstr: CFStringRef to convert to C string
 *	cstr: char buffer that will contain result of conversion
 *	len: size of cstr buffer
 *
 * return value:
 *	-1: conversion failed
 *	 0: success
 */
    int
cf2c( CFStringRef cfstr, char *cstr, int len )
{
    if ( cfstr == NULL ) {
	fprintf( stderr, "%s: null CFStringRef\n", __FUNCTION__ );
	return( -1 );
    }

    if ( CFStringGetCString( cfstr, cstr, ( CFIndex )len,
		kCFStringEncodingUTF8 ) == false ) {
	fprintf( stderr, "Failed to convert CFStringRef to C String\n" );
	return( -1 );
    }

    return( 0 );
}

/*
 *
 * @author Timo Furrer
 *
 * @version 0.00.02
 * @copyright GNU General Public License
 *
 * @reopsitory https://github.com/timofurrer/nsconverter
 *
 */

#include "nsconverter.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// usage
#define USAGE               "Usage: %s -i <inputbase> -o <outputbase> NUMBER\n"

// return values
#define CONVERT_SUCCESSFUL  0
#define E_UNSUPPORTED_CHAR  1
#define E_ALLOCATION_FAILED 2

#define VALID_BASE          0
#define INVALID_BASE        1

// buffers
#define INPUT_BUF_MAX       13
#define OUTPUT_BUF_MAX      256

// convert limits
#define BASE_MIN            2
#define BASE_MAX            32

int main( int argc, char **argv )
{
  int ret;
  int inputBase  = -1;
  int outputBase = -1;
  char input[INPUT_BUF_MAX];
  char *result   = NULL;
  char argument;

  if( argc == 1 )
  {
    printf( "Please input a number from any system to convert (just %d digits are read): ", INPUT_BUF_MAX );
    scanf( "%13s", input );

    printf( "Please input the base of the number you put in before: " );
    scanf( "%d", &inputBase );

    printf( "Please input the base of the output number system: " );
    scanf( "%d", &outputBase );

    if( validateBase( inputBase ) == INVALID_BASE )
    {
      fprintf( stderr, "Error: Please specify an input base between %d and %d using the option `-i`!\n", BASE_MIN, BASE_MAX );
      return EXIT_FAILURE;
    }

    if( validateBase( outputBase ) == INVALID_BASE )
    {
      fprintf( stderr, "Error: Please specify an output base between %d and %d using the option `-o`!\n", BASE_MIN, BASE_MAX );
      return EXIT_FAILURE;
    }
  }
  else
  {
    // disable getopt error output
    opterr = 0;
    while(( argument = getopt( argc, argv, "i:o:" )) != -1 )
    {
      switch( argument )
      {
        case 'i':
          inputBase  = atoi( optarg );
          break;
        case 'o':
          outputBase = atoi( optarg );
          break;
        case '?':
          if( optopt == 'c' )
            fprintf( stderr, "Error: -%c requires an argument!\n", optopt );
          else if( isprint( optopt ))
            fprintf( stderr, "Error: Unknown option `-%c`!\n", optopt );
          else
            fprintf( stderr, "Error: Unkown option character `\\x%x`!.\n", optopt );
        default:
          return EXIT_FAILURE;
      }
    }

    if( validateBase( inputBase ) == INVALID_BASE )
    {
      fprintf( stderr, "Error: Please specify an input base between %d and %d using the option `-i`!\n", BASE_MIN, BASE_MAX );
      return EXIT_FAILURE;
    }

    if( validateBase( outputBase ) == INVALID_BASE )
    {
      fprintf( stderr, "Error: Please specify an output base between %d and %d using the option `-o`!\n", BASE_MIN, BASE_MAX );
      return EXIT_FAILURE;
    }

    if( !( argc - optind ))
    {
      fprintf( stderr, USAGE, *argv );
      fprintf( stderr, "\nError: Please specfiy the number to convert from base `%d` to `%d`!\n", inputBase, outputBase );
      return EXIT_FAILURE;
    }

    if( strlen( argv[optind] ) > INPUT_BUF_MAX )
      printf( "Warning: Just %d digits of your %ld passed digits are read\n", INPUT_BUF_MAX, strlen( argv[3] ));
    strncpy( input, argv[optind], INPUT_BUF_MAX );
  }

  // allocate result buffer memory (will be reallocated)
  result = malloc( 0 );

  // convert
  if(( ret = convert( input, inputBase, outputBase, result )) != CONVERT_SUCCESSFUL )
  {
    switch( ret )
    {
      case E_UNSUPPORTED_CHAR:
        fprintf( stderr, "Error: The number %s is not valid in numbering system with base %d!\n", input, inputBase );
        break;
      case E_ALLOCATION_FAILED:
        fprintf( stderr, "Error: Allocation for result buffer failed!\n" );
        break;
      default:
        fprintf( stderr, "Error: Unkown error occured!\n" );
    }

    // freeing result and exit with failure
    free( result );
    return EXIT_FAILURE;
  }

  // print result
  printf( "%s(%d) => %s(%d)\n", input, inputBase, result, outputBase );

  // freeing result and exit with success
  free( result );
  return EXIT_SUCCESS;
}

int validateBase( int base )
{
  return ( base >= BASE_MIN && base <= BASE_MAX ) ? VALID_BASE : INVALID_BASE;
}

int convert( char *input, int inputBase, int outputBase, char *result )
{
  int calculated[OUTPUT_BUF_MAX] = { 0 };
  long long int decimal          = 0;
  int length                     = 0;
  int i, j;

  for( i = strlen( input ) - 1; i >= 0; i-- )
  {
    char tmp;
    if( input[i] >= '0' && input[i] <= '9' )
      tmp = input[i] - 48;
    else if( input[i] >= 'A' && input[i] <= 'Z' )
      tmp = input[i] - 55;
    else if( input[i] >= 'a' && input[i] <= 'z' )
      tmp = input[i] - 87;
    else
      return E_UNSUPPORTED_CHAR;

    if( tmp >= inputBase )
      return E_UNSUPPORTED_CHAR;

    decimal += tmp * pow( inputBase, strlen( input ) - 1 - i );
  }

  i = 0;
  while( decimal > 0 )
  {
    calculated[i++]  = decimal % outputBase;
    decimal         /= outputBase;
  }

  // set buffer length
  length = i;

  // reallocate buffer memory
  result = (char *) realloc( result, length * sizeof( char ) + 1 );

  if( result == NULL )
    return E_ALLOCATION_FAILED;

  // write in buffer
  for( i = length - 1, j = 0; i >= 0; i--, j++ )
    result[j] = calculated[i] + (( calculated[i] >= 10 ) ? 55 : 48 );
  result[j] = '\0';

  return CONVERT_SUCCESSFUL;
}

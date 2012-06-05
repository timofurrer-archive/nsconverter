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
#include <string.h>
#include <math.h>

// usage
#define USAGE               "Usage: %s <inputbase> <outputbase> <number>\n"

// return values
#define CONVERT_SUCCESSFUL  0
#define E_UNSUPPORTED_CHAR  1
#define E_ALLOCATION_FAILED 2

// buffers
#define INPUT_BUF_MAX       13
#define OUTPUT_BUF_MAX      256

int main( int argc, char **argv )
{
  int ret;
  int inputBase  = -1;
  int outputBase = -1;
  char input[INPUT_BUF_MAX];
  char *result = malloc( 0 );

  if( argc == 1 )
  {
    // FIXME: check for valid values
    printf( "Please input a number from any system to convert (just %d digits are read): ", INPUT_BUF_MAX );
    scanf( "%13s", input );

    printf( "Please input the base of the number you put in before: " );
    scanf( "%d", &inputBase );

    printf( "Please input the base of the output number system: " );
    scanf( "%d", &outputBase );
  }
  else
  {
    if( argc != 4 )
    {
      fprintf( stderr, USAGE, *argv );
      return EXIT_FAILURE;
    }

    // FIXME: check for valid values
    inputBase  = atoi( argv[1] );
    outputBase = atoi( argv[2] );
    if( strlen( argv[3] ) > INPUT_BUF_MAX )
      printf( "Warning: Just %d digits of your %ld passed digits are read\n", INPUT_BUF_MAX, strlen( argv[3] ));
    strncpy( input, argv[3], INPUT_BUF_MAX );
  }

  // convert
  if(( ret = convert( input, inputBase, outputBase, result )) != CONVERT_SUCCESSFUL )
  {
    switch( ret )
    {
      case E_UNSUPPORTED_CHAR:
      {
        fprintf( stderr, "Error: The number %s is not valid in numbering system with base %d!\n", input, inputBase );
        break;
      }
      case E_ALLOCATION_FAILED:
      {
        fprintf( stderr, "Error: Allocation for result buffer failed!\n" );
        break;
      }
      default:
      {
        fprintf( stderr, "Error: Unkown error occured!\n" );
      }
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

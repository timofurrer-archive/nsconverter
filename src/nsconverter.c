/*
 *
 * @author Timo Furrer
 *
 * @version 0.00.01
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
#define USAGE              "Usage: %s <inputbase> <outputbase> <number>\n"

// return values
#define CONVERT_SUCCESSFUL 0
#define E_UNSUPPORTED_CHAR 1

// buffers
#define INPUT_BUF_MAX      13
#define OUTPUT_BUF_MAX     256

int main( int argc, char **argv )
{
  int inputBase  = -1;
  int outputBase = -1;
  char input[INPUT_BUF_MAX];

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

  // convert and print result
  if( convert( input, inputBase, outputBase ) != CONVERT_SUCCESSFUL )
  {
    fprintf( stderr, "Error: The number %s is not valid in numbering system with base %d!\n", input, inputBase );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int convert( char *input, int inputBase, int outputBase )
{
  int result[OUTPUT_BUF_MAX] = { 0 };
  long long int decimal      = 0;
  int firstOcc               = 0;
  int i;

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

  for( i = 0; decimal > 0; i++ )
  {
    result[i] = decimal % outputBase;
    decimal /= outputBase;
  }

  printf( "%s(%d) => ", input, inputBase );
  for( i = OUTPUT_BUF_MAX - 1; i >= 0; i-- )
  {
    if( firstOcc == 1 )
    {
      if( result[i] >= 10 )
        printf( "%c", result[i] + 55 );
      else
        printf( "%d", result[i] );

      if( i % 8 == 0 && i != 0 )
        printf( " " );
      continue;
    }

    if( firstOcc == 0 && result[i] == 0 && result[i - 1] != 0 )
      firstOcc = 1;
  }
  printf( "(%d)\n", outputBase );

  return CONVERT_SUCCESSFUL;
}

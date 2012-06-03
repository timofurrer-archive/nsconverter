#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define USAGE         "Usage: %s <inputbase> <outputbase> <number>\n"
#define INPUT_BUF_MAX 32

int convert( char *input, int inputBase, int outputBase );

int main( int argc, char **argv )
{
  int inputBase  = -1;
  int outputBase = -1;
  char input[INPUT_BUF_MAX] = { 0 };

  if( argc == 1 )
  {
    // FIXME: check for valid values
    printf( "Please input a number from any system to convert: " );
    scanf( "%s", &input );

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
    strncpy( input, argv[3], INPUT_BUF_MAX );
  }

  // convert and print result
  convert( input, inputBase, outputBase );

  return EXIT_SUCCESS;
}

int convert( char *input, int inputBase, int outputBase )
{
  int result[INPUT_BUF_MAX] = { 0 };
  int decimal               = 0;
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
      return 1; // unsupported character

    decimal += tmp * pow( inputBase, strlen( input ) - 1 - i );
  }

  for( i = 0; decimal > 0; i++ )
  {
    result[i] = decimal % outputBase;
    decimal /= outputBase;
  }

  for( i = INPUT_BUF_MAX - 1; i >= 0; i-- )
  {
    if( result[i] >= 10 )
      printf( "%c", result[i] + 55 );
    else
      printf( "%d", result[i] );

    if( i % 8 == 0 )
      printf( " " );
  }
  printf( "\n" );

  return 0;
}

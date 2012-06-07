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

/**
 * @brief Convert and print a number [input] from any numbering system [inputBase] to any other [outputBase]
 *
 * @param input      The number to convert
 * @param inputBase  The base of the number to convert
 * @param outputBase The base to convert to
 *
 * @return integer   return 0 if it was successful and >0 if the conversation failed
 */
int convert( char *input, int inputBase, int outputBase, char *result );

/**
 * @brief Valide a base with minimum and maximum
 *
 * @param base     The base to validate
 *
 * @return integer if it was valid or not
 */
int validateBase( int base );

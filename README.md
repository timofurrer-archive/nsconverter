# Number system converter
> This smart tool converts a number from any numbering system to any other<br />
> *Version: 0.00.02*

**Author:** Timo Furrer<br />
**Version:** 0.00.02<br />
**Copyright:** GPL<br />

## How to use

### Build & Install
First you have to compile the converter with gcc or something similar like clang.
You can use the Makefile to compile with gcc:

    $ make

After you compiled successfully you can install it with make as well:

    $ sudo make install

### Using
Use the NSConverter like this:

    Usage: nsconverter -i <inputbase> -o <outputbase> NUMBER

    $ nsconverter -i 10 -o 16 654321

Or you don't pass any arguments and you will be prompt by nsconverter to enter the values.

    Please input a number from any system to convert: 654321
    Please input the base of the number you put in before: 10
    Please input the base of the output number system: 16

In both cases the output looks like this:

    654321(10) => 9FBF1(16)

### Uninstall
To unintall use `make uninstall`:

    $ sudo make uninstall

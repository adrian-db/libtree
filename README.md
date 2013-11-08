libtree
=======

A little C library for making self-balancing binary trees.

This little project isn't intended for serious use - the reasons for creating it are:
- to refresh my C knowledge with a decently complicated example.
- to understand how self-balancing binary trees work, by making one.
- to get to grips with how to unit test C code.

Quick Start
-----------

You need autotools (autoconf version 2.69 or better) and the [Check](http://check.sourceforge.net "Check unit testing library for C") C unit testing library (I'm using 0.9.10).

Then type the following into the terminal from the project directory, to get autotools to generate all the templates that it needs:

`autoreconf --install`

Assuming that goes well, type the following commands to build the project:

`./configure`

`make`

If you want to run the unit tests, type the following:

`make check`

The header file is in the include subdirectory, and 'make' should have created a libtree.a static library in the src subdirectory.

Eventually I'll have a demo app to demonstrate how to use the library, but for now, you have to look at the header and the code itself.

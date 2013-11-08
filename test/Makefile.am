AM_CPPFLAGS = -I$(top_builddir)/include

TESTS = tb_tree_tests
check_PROGRAMS = tb_tree_tests
tb_tree_tests_SOURCES = tb-tree-tests.c $(top_builddir)/include/rb-tree.h
tb_tree_tests_CFLAGS = @CHECK_CFLAGS@
tb_tree_tests_LDADD = $(top_builddir)/src/libtree.a @CHECK_LIBS@

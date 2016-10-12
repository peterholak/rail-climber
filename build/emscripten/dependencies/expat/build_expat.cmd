call emcc -I. -DHAVE_EXPAT_CONFIG_H -c xmlparse.c xmlrole.c xmltok.c xmltok_impl.c xmltok_ns.c
call emcc -o expat.bc xmlparse.o xmlrole.o xmltok.o xmltok_impl.o xmltok_ns.o

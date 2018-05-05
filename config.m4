PHP_ARG_ENABLE(velocypack,
    [Whether to enable the "velocypack" extension],
    [  --enable-velocypack      Enable "velocypack" extension support])

if test $PHP_VELOCYPACK != "no"; then
    for i in /usr /usr/local; do
      if test -f $i/include/velocypack/vpack.h; then
        VELOCYPACK_INCLUDE_DIR=$i
        AC_MSG_RESULT(found velocypack includes in $i)
        break
      fi
    done

    for i in /usr /usr/local; do
      if test -f $i/lib/libvelocypack.a; then
        VELOCYPACK_LIB_DIR=$i
        AC_MSG_RESULT(found velocypack lib in $i)
        break
      fi
    done

    PHP_ADD_INCLUDE($VELOCYPACK_INCLUDE_DIR)

    CXXFLAGS="-std=c++11"
    PHP_REQUIRE_CXX()

    PHP_SUBST(VELOCYPACK_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, VELOCYPACK_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(velocypack, $VELOCYPACK_LIB_DIR, VELOCYPACK_SHARED_LIBADD)
    PHP_NEW_EXTENSION(velocypack, php_velocypack.cpp src/vpack.cpp, $ext_shared)
fi

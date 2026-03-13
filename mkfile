# Window Manager - Plan 9 mk build file

<../mkfile.common

# Directories
SYS=../sys
LIB9=$SYS/src/lib/9
LIB9_INCLUDE=$SYS/include
LIB9_LIB=$ROOT/amd64/lib/lib9.a

INCLUDE=include
SRC=src
BUILD=build
BIN=bin

# Marrow graphics library
MARROW=../marrow
MARROW_INCLUDE=$MARROW/include

# TaijiOS Toolkit (TK) - for window/widget APIs
TK=../tk
TK_INCLUDE=$TK/include
TK_LIB=$TK/build/libtk.a

# Sources
WM_SRC=main

# Targets
WM_BIN=$BIN/wm

# Default target
all:V: setup $WM_BIN

# Create directories
setup:V:
	mkdir -p $BUILD $BIN

# WM binary
$WM_BIN: ${WM_SRC:%=$BUILD/%.$O} $TK_LIB $LIB9_LIB
	$LD -Wall -g -I$INCLUDE -I$SRC -I$MARROW_INCLUDE -I$LIB9_INCLUDE -I$TK_INCLUDE \
		${WM_SRC:%=$BUILD/%.$O} \
		-L$TK/build -L$MARROW/build -L$LIB9 -ltk -lmarrow -l9 -o $target $LDFLAGS
	chmod +x $target

# Compile rules
$BUILD/%.$O: $SRC/%.c
	$CC $CFLAGS -I$INCLUDE -I$SRC -I$MARROW_INCLUDE -I$LIB9_INCLUDE -I$TK_INCLUDE -c $prereq -o $target

# Clean
clean:V:
	rm -rf $BUILD $BIN

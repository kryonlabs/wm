# Window Manager - Plan 9 mk build file
# Independent of TK - follows rio pattern (rio is independent of tk)

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

# Mu graphics library
MU=../mu
MU_INCLUDE=$MU/include

# Sources
WM_SRC=main menu shell wmgr

# Targets
WM_BIN=$BIN/wm

# Default target
all:V: setup $WM_BIN

# Create directories
setup:V:
	mkdir -p $BUILD $BIN

# WM binary (independent of tk - only depends on mu and lib9)
$WM_BIN: ${WM_SRC:%=$BUILD/%.$O} $LIB9_LIB
	$LD -Wall -g -I$INCLUDE -I$SRC -I$MU_INCLUDE -I$LIB9_INCLUDE \
		${WM_SRC:%=$BUILD/%.$O} \
		-L$MU/build -L$LIB9 -lmu -l9 -o $target $LDFLAGS
	chmod +x $target

# Compile rules
$BUILD/%.$O: $SRC/%.c
	$CC $CFLAGS -I$INCLUDE -I$SRC -I$MU_INCLUDE -I$LIB9_INCLUDE -c $prereq -o $target

# Clean
clean:V:
	rm -rf $BUILD $BIN

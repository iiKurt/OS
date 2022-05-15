# Setup and download

mkdir -p Tools
cd Tools

if [ ! -d "GCC" ]; then
	wget -nc https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.gz -O GCC.tar.gz
	mkdir -p GCC
	tar -xzf GCC.tar.gz -C GCC --strip-components 1
	rm -rf GCC.tar.gz
fi

if [ ! -d "Binutils" ]; then
	wget -nc https://ftp.gnu.org/gnu/binutils/binutils-2.35.1.tar.gz -O Binutils.tar.gz
	mkdir -p Binutils
	tar -xzf Binutils.tar.gz -C Binutils --strip-components 1
	rm -rf Binutils.tar.gz
fi

# Build the cross compiler

BASEDIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

PREFIX="$BASEDIR/Cross"
TARGET="x86_64-elf"
# should only apply for the current session
export PATH="$PREFIX/bin:$PATH"

# Binutils

mkdir -p Build-Binutils
cd Build-Binutils

../Binutils/configure --prefix=$PREFIX \
--target=$TARGET \
--enable-interwork --enable-multilib \
--disable-nls --disable-werror
make
make install

echo "DONE: Binutils"

cd ..

# GCC

cd GCC
./contrib/download_prerequisites
cd ..

mkdir -p Build-GCC
cd Build-GCC

# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH

../GCC/configure --prefix=$PREFIX \
--target=$TARGET \
--disable-nls \
--enable-languages=c,c++ --without-headers \
--enable-interwork --enable-multilib

make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

# TODO:
# ask user if they want to delete the Source and Building folders and just keep the binaries

#!/usr/bin/env bash

# Alexis Megas.

if [ ! -r biblioteq.pro ]
then
    echo "Please execute $0 from the primary directory."
    exit 1
fi

VERSION=$(grep -oP '(?<=BIBLIOTEQ_VERSION ").*(?=")' Source/biblioteq.h)

# Prepare ./biblioteq.

make distclean 2>/dev/null
mkdir -p ./biblioteq/Documentation
mkdir -p ./biblioteq/Lib
mkdir -p ./biblioteq/SQL
qmake -o Makefile biblioteq.pro
lupdate biblioteq.pro 2>/dev/null
lrelease biblioteq.pro 2>/dev/null
make -j $(nproc)
cp -p ./BiblioteQ ./biblioteq/.
cp -p ./Icons/book.png ./biblioteq/.
cp -p ./biblioteq.conf ./biblioteq/.
cp -p ./biblioteq.sh ./biblioteq/.
cp -pr ./Data ./biblioteq/.
cp -pr ./Documentation/* ./biblioteq/Documentation/.
cp -pr ./SQL/* ./biblioteq/SQL/.

# Prepare a tar bundle.

cp $(ldd ./BiblioteQ | awk '{print $3}' | grep -e '^/') ./biblioteq/Lib/.
chmod -x ./biblioteq/Lib/*.so*
tar -cvz -f BiblioteQ-${VERSION}.tar.gz ./biblioteq
make distclean
rm -fr ./biblioteq

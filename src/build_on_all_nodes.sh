#!/bin/bash

SOURCE="./Web_ImageProcessing/image_processing.c"
USER="mpiu"
MACHINEFILE="machinefile"
echo "Compiling versions"

while read NODE; do
 echo "Connecting to $NODE..."
 ssh "$USER@$NODE" bash -s << 'ENDSSH'

ARCH_LOCAL=$(uname -m)
BINARY_NAME="mpiu_$ARCH_LOCAL"
TARGET="./Web_ImageProcessing/$BINARY_NAME"

if [ -f "\$TARGET" ]; then
 echo "Binary already compiled for $ARCH_LOCAL on $HOSTNAME: $BINARY_NAME"
else
 echo "Compiling $BINARY_NAME on $HOSTNAME..."
# mpicc mpiu_hello.c -o "$BINARY_NAME"
  mpicc -fopenmp ./Web_ImageProcessing/image_processing.c -o "$TARGET"
 echo "Compilation complete: $BINARY_NAME"
fi

ENDSSH

done < "$MACHINEFILE"


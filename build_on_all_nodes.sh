#!/bin/bash

SOURCE="./ImplementacionRedes/image_processing.c"
USER="mpiu"
MACHINEFILE="machinefile"
echo "Compilando versiones"

while read NODE; do
 echo "Conectando a $NODE..."
 ssh "$USER@$NODE" bash -s << 'ENDSSH'

ARCH_LOCAL=$(uname -m)
BINARY_NAME="mpiu_$ARCH_LOCAL"
TARGET="./ImplementacionRedes/$BINARY_NAME"

if [ -f "\$TARGET" ]; then
 echo "Binario ya compilado para $ARCH_LOCAL en $HOSTNAME: $BINARY_NAME"
else
 echo "Compilando $BINARY_NAME en $HOSTNAME..."
# mpicc mpiu_hello.c -o "$BINARY_NAME"
  mpicc -fopenmp ./ImplementacionRedes/image_processing.c -o "$TARGET"
 echo "Compilacion completa: $BINARY_NAME"
fi

ENDSSH

done < "$MACHINEFILE"


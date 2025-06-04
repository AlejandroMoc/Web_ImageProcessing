#!/bin/bash

# Ruta al archivo machinefile
MACHINEFILE="machinefile"

# Limpiar archivo anterior
> "$MACHINEFILE"

# Definir hosts y slots
declare -A hosts_slots
hosts_slots["ub0"]=6
hosts_slots["ub1"]=1
hosts_slots["ub2"]=1

# Intentar hacer ping a cada host
for host in "${!hosts_slots[@]}"; do
    echo -n "Comprobando $host... "
    
    # -c 1 = 1 solo paquete, -W 1 = timeout de 1 segundo
    if ping -c 1 -W 1 "$host" &>/dev/null; then
        echo "activo"
        echo "$host slots=${hosts_slots[$host]}" >> "$MACHINEFILE"
    else
        echo "NO disponible"
    fi
done

echo -e "\nContenido generado en $MACHINEFILE:"
cat "$MACHINEFILE"
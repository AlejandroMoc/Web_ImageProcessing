#!/bin/bash

# Path to the machinefile
MACHINEFILE="machinefile"

# Clear previous file
> "$MACHINEFILE"

# Define hosts and slots
declare -A hosts_slots
hosts_slots["ub0"]=6
hosts_slots["ub1"]=1
hosts_slots["ub2"]=1

# Try to ping each host
for host in "${!hosts_slots[@]}"; do
    echo -n "Checking $host... "
    
    # -c 1 = 1 packet only, -W 1 = 1 second timeout
    if ping -c 1 -W 1 "$host" &>/dev/null; then
        echo "Active"
        echo "$host slots=${hosts_slots[$host]}" >> "$MACHINEFILE"
    else
        echo "NOT available"
    fi
done

echo -e "\nGenerated content in $MACHINEFILE:"
cat "$MACHINEFILE"
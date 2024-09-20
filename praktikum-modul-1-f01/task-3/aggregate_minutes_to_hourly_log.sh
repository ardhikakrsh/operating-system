#!/bin/bash

metrics_dir="/home/$(whoami)/metrics"
agg_files=$(ls "$metrics_dir"/metrics* | grep metrics_2024)
value_path_size=()
maximum=0
minimum=999653638144
total_size=0
count=0
min_entry=""
max_entry=""
avg_entry=""
timestamp=$(date +'%Y%m%d%H')

for file in $agg_files; do
    values=$(awk 'NR==2{print $0}' "$file")
    avg_value=$(awk -F ',' 'NR==2{ print $1 $2 $3 $4 $5 $6 $7 $8 $9 $10}' "$file")
    path_sizes=$(awk -F ',' 'NR==2{print $11}' "$file")

    for path_size in $path_sizes; do
        if [ -z "$path_size" ]; then
            continue
        fi

        numeric=$(echo "$path_size" | sed 's/[^0-9]//g')
        unit=$(echo "$path_size" | sed 's/[0-9]*//g')

        case $unit in
            "K") size_bytes=$(($numeric * 1024));;
            "M") size_bytes=$(($numeric * 1024 * 1024));;
            "G") size_bytes=$(($numeric * 1024 * 1024 * 1024));;
            *) size_bytes=$numeric;;
        esac

        if [ "$size_bytes" -lt "$minimum" ]; then
            minimum=$size_bytes
            min_entry="$values"
        fi
        if [ "$size_bytes" -gt "$maximum" ]; then
            maximum=$size_bytes
            max_entry="$values"
        fi

        total_size=$(($total_size + $size_bytes))
        count=$(($count + 1))
    done
done

if [ "$count" -gt 0 ]; then
    avg=$(($total_size / $count))

    if [ "$avg" -ge 1073741824 ]; then
        avg=$(($avg / 1073741824))
        unit="G"
    elif [ "$avg" -ge 1048576 ]; then
        avg=$(($avg / 1048576))
        unit="M"
    elif [ "$avg" -ge 1024 ]; then
        avg=$(($avg / 1024))
        unit="K"
    else
        unit="B"
    fi

    avg_entry="$avg_value,$avg$unit"
fi

echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log
echo "minimum,$min_entry" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log
echo "maximum,$max_entry" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log
echo "average,$avg_entry" >> /home/$(whoami)/metrics/metrics_agg_${timestamp}.log

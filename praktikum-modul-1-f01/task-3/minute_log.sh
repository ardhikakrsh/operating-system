#!/bin/bash

i=0

metrics_dir="/home/$(whoami)/metrics"

if [ ! -d "$metrics_dir" ]; then
    mkdir -p "$metrics_dir"
fi

while read -r line
do
    random_directory[$i]="$line"
    (( i++ ))
done < <(cd ~ && ls -d */)

declare -r DIRECTORY=${random_directory[$RANDOM% ${#random_directory[@]}]}
timestamp=$(date +'%Y%m%d%H%M%S')

memory_metrics=$(free -m | awk 'NR==2 {print $2","$3","$4","$5","$6","$7}')
swap_metrics=$(free -m | awk 'NR==3 {print $2","$3","$4}')
disk_metrics=$(du -sh "/home/$(whoami)/$DIRECTORY" | awk '{print $1}')

echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" >> /home/$(whoami)/metrics/metrics_${timestamp}.log
echo "$memory_metrics,$swap_metrics,/home/$(whoami)/$DIRECTORY,$disk_metrics" >> /home/$(whoami)/metrics/metrics_${timestamp}.log

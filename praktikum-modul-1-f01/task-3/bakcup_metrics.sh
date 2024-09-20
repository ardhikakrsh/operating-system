#!/bin/bash

timestamp=$(date +'%Y%m%d%H')

backup_file="backup_metrics_${timestamp}.tar.gz"

cd /home/$(whoami)/metrics/

log_files=$(ls metrics_agg_*.log 2>/dev/null)
log=$(ls metrics_*.log)

if [ -n "$log_files" ]; then
    tar -cf "${backup_file}" ${log_files}
    chmod 400 ${log}
    gzip "${backup_file}"
fi

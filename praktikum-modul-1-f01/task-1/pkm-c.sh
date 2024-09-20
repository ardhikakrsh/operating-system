#!/bin/bash

tsv_file="../resources/DataPKM.tsv"

awk -F '\t' 'NR > 1 {
    proposals[$2]++;
    advisors[$2][$6] = 1; 
}
END {
    for (student in proposals) {
        if (proposals[student] > 1) {
            print "Pembimbing:";
            for (advisor in advisors[student]) {
                print "- ", advisor;
            }
        }
    }
}' "$tsv_file"

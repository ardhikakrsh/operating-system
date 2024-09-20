# ! /bin/bash

tsv_file="../resources/DataPKM.tsv"

cut -f7 "$tsv_file" \
  | sort \
  | uniq -c \
  | sort -nr \
  | head -1

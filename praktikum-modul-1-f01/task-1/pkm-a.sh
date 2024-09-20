# ! /bin/bash

tsv_file="../resources/DataPKM.tsv"

awk -F '\t' '{
            gsub(/_/, " ", $2);
            maxCharacterTitle=20
            num_words = split($5, words, " ");
            if (num_words > maxCharacterTitle)
                printf NR". %s\nDepartemen: %s\n\n", $2, $3
}' "$tsv_file"

#!/bin/sh
KEYS=$(cat keys.json)
LEN=$(echo "$KEYS" | jq '. | length')
rm -f keys.h
cat >> keys.h << EOF 
typedef struct {
    const char  name[64];
    const char  id[3];
} Key;
EOF
echo "Key keys[] = {" >> keys.h
for I in $(seq 0 $(($LEN-1)) ); 
do
    NAME=$(echo "$KEYS" | jq ".[$I].name" -r)
    ID=$(echo "$KEYS" | jq ".[$I].id" -r)
    echo "  { .name = \"$NAME\", .id = {$ID} }," >> keys.h
done

echo "};" >> keys.h
echo "#define KEYS_COUNT $LEN" >> keys.h

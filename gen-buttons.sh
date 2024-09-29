#!/bin/sh
BUTTONS=$(cat buttons.json)
LEN=$(echo "$BUTTONS" | jq '. | length')
rm -f buttons.h
cat >> buttons.h << EOF 
typedef struct {
    const char  name[64];
    int         id;
} Button;
EOF
echo "Button buttons[] = {" >> buttons.h
for I in $(seq 0 $(($LEN-1)) ); 
do
    NAME=$(echo "$BUTTONS" | jq ".[$I].name" -r)
    ID=$(echo "$BUTTONS" | jq ".[$I].id" -r)
    echo "  { .name = \"$NAME\", .id = $ID }," >> buttons.h
done

echo "};" >> buttons.h
echo "#define BUTTONS_COUNT $LEN" >> buttons.h

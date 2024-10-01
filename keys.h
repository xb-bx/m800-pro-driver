typedef struct {
    const char          name[64];
    const unsigned char id[3];
} Key;
Key keys[] = {
    {.name = "LeftClick", 			.id = {0x10, 0x01, 0x00}},
    {.name = "RightClick", 			.id = {0x10, 0x02, 0x00}},
    {.name = "MiddleClick", 		.id = {0x10, 0x04, 0x00}},
    {.name = "Forward", 			.id = {0x10, 0x10, 0x00}},
    {.name = "Backward", 			.id = {0x10, 0x08, 0x00}},
    {.name = "DPILock/200", 		.id = {0x50, 0x01, 0x00}},
    {.name = "DPILock/300", 		.id = {0x50, 0x02, 0x00}},
    {.name = "DPILock/400", 		.id = {0x50, 0x03, 0x00}},
    {.name = "DPILock/500", 		.id = {0x50, 0x04, 0x00}},
    {.name = "DPILock/600", 		.id = {0x50, 0x05, 0x00}},
    {.name = "DPILock/700", 		.id = {0x50, 0x06, 0x00}},
    {.name = "DPILock/800", 		.id = {0x50, 0x07, 0x00}},
    {.name = "DPILock/900", 		.id = {0x50, 0x08, 0x00}},
    {.name = "DPILock/1000", 		.id = {0x50, 0x09, 0x00}},
    {.name = "DPILock/1100", 		.id = {0x50, 0x0a, 0x00}},
    {.name = "DPILock/1200", 		.id = {0x50, 0x0b, 0x00}},
    {.name = "DPILoop", 			.id = {0x40, 0x01, 0x00}},
    {.name = "DPI+", 				.id = {0x40, 0x02, 0x00}},
    {.name = "DPI-", 				.id = {0x40, 0x03, 0x00}},
    {.name = "Media/Select", 		.id = {0x80, 0x83, 0x01}},
    {.name = "Media/Prev-Track", 	.id = {0x80, 0xb6, 0x00}},
    {.name = "Media/Next-Track", 	.id = {0x80, 0xb5, 0x00}},
    {.name = "Media/Stop", 			.id = {0x80, 0xb7, 0x00}},
    {.name = "Media/PlayPause", 	.id = {0x80, 0xcd, 0x00}},
    {.name = "Media/Mute", 			.id = {0x80, 0xe2, 0x00}},
    {.name = "Media/Volume+", 		.id = {0x80, 0xe9, 0x00}},
    {.name = "Media/Volume-", 		.id = {0x80, 0xea, 0x00}},
    {.name = "Media/Mail", 			.id = {0x80, 0x8a, 0x01}},
    {.name = "Media/Calculator", 	.id = {0x80, 0x92, 0x01}},
    {.name = "Disable", 			.id = {0x00, 0x00, 0x00}},
};
#define KEYS_COUNT sizeof(keys) / sizeof(Key)

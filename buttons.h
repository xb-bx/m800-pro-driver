typedef struct {
    const char  name[64];
    int         id;
} Button;
Button buttons[] = {
  { .name = "LMB",          .id = 0 },
  { .name = "RMB",          .id = 1 },
  { .name = "Wheel",        .id = 2 },
  { .name = "MOUSE4",       .id = 4 },
  { .name = "MOUSE5",       .id = 3 },
  { .name = "MiddleButton", .id = 5 },
};
#define BUTTONS_COUNT sizeof(buttons)/sizeof(Button)

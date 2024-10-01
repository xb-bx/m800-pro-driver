#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "buttons.h"
#include "keys.h"
#define M800_PRO_VID 0x248a
#define M800_PRO_PID 0x5b2f
#define INTERFACE 1
#define eprintf(args...) fprintf(stderr, args)

enum PollingRate {
    Hz125   = 0x8,
    Hz250   = 0x4,
    Hz500   = 0x2,
    Hz1000  = 0x1,
};

void
usage() {
    printf("m800pro-drv <command> <args>\n");
    printf("commands:\n");
    printf("\tlist-keys\n");
    printf("\tlist-buttons\n");
    printf("\tcharge\n");
    printf("\tset\tpoll\t\t(125/250/500/1000)\n");
    printf("\tset\tpowerdown-time\t3-10\n");
    printf("\tset\tdebounce-time\t0-30\n");
    printf("\tset\tdpi\t\t0-26000 0-26000 0-26000 0-26000 0-26000\n");
    printf("\tset\tdpi-colors\tff00ff 00ff00 ffffff ff0000 0000ff\n");
    printf("\tset\tLOD\t\t1/2\n");
    printf("\tset\tmotion-wakeup\ton/off\n");
    printf("\tbind\t<btn>\t\t<key>\t\n");
}
int
set_polling_rate(libusb_device_handle *dev_handle, int packet_id, int polling_rate) {
    unsigned char payload[33] = {0xc, 0x1, 0x7, 0x0, 0x32, 0x1, 0x1, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    payload[4] = packet_id + 1;
    payload[7] = polling_rate;
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return n;
}
int
set_debounce_time(libusb_device_handle *dev_handle, int packet_id, int debounce_time) {
    unsigned char payload[33] = {0xc, 0x1, 0xa, 0x0, 0xd, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    payload[4] = packet_id + 1;
    payload[7] = debounce_time;
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return n;
}
int
set_powerdown_time(libusb_device_handle *dev_handle, int packet_id, int powerdown_time) {
    unsigned char payload[33] = {0xc, 0x1, 0xb, 0x0, 0x2, 0x1, 0x2, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    payload[4] = packet_id + 1;
    payload[7] = powerdown_time;
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return n;
}
int
set_LOD(libusb_device_handle *dev_handle, int packet_id, int lod) {
    unsigned char payload[33] = {0xc, 0x1, 0x9, 0x0, 0xb, 0x1, 0x2, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    payload[4] = packet_id + 1;
    payload[7] = lod;
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return n;
}
int
set_motion_wakeup(libusb_device_handle *dev_handle, int packet_id, int enable) {
    unsigned char payload[33] = {0xc, 0x1, 0xc, 0x0, 0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    payload[4] = packet_id + 1;
    payload[7] = enable;
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return n;
}
void
set_color(unsigned char *at, uint32_t color) {
    at[0] = (color >> 16) & 0xFF;
    at[1] = (color >>  8) & 0xFF;
    at[2] = (color >>  0) & 0xFF;
}
int
set_dpi(libusb_device_handle *dev_handle, int packet_id, int dpi[5]) {
    unsigned char payload[33]   = {0xc, 0x1, 0x5, 0x0, 0x0, 0x1, 0x12, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, };
    unsigned char sum_enabled   = 0;
    unsigned char bitmask       = 0;
    
    int shift = 0;
    for(int i = 0; i < 5; i++) {
        if(dpi[i] > 0) {
            sum_enabled++;
            bitmask |= 1 << shift;
        }
        shift++;
    }

    payload[4]  = packet_id + 1;
    payload[7] += sum_enabled; 
    payload[8]  = bitmask;
    for(int i = 0; i < 5; i++) {
        int payloadi = 9 + i * 2;
        if(dpi[i] == 0) {
            payload[payloadi]       = 0;
            payload[payloadi + 1]   = 0;
        } else {
            // DPI is represented by 16 bit little-endian number * 50 + 50
            int dpival  = dpi[i] / 50 - 1;

            payload[payloadi]       = dpival & 0xFF;
            payload[payloadi + 1]   = (dpival >> 8) & 0xFF;
        }
    }
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return 0;
}
int
set_dpi_colors(libusb_device_handle *dev_handle, int packet_id, uint32_t colors[5]) {
    unsigned char payload[33] = {0xc, 0x1, 0x6, 0x0, 0x2, 0x1, 0x18, 0x0, 0xff, 0x0, 0x0, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0x0, 0x0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x0};
    payload[4] = packet_id + 1;
    set_color(&payload[7],  colors[0]);
    set_color(&payload[10], colors[1]);
    set_color(&payload[13], colors[2]);
    set_color(&payload[16], colors[3]);
    set_color(&payload[19], colors[4]);
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return n;
}
int
find_button(char *name) {
    for(int i = 0; i < BUTTONS_COUNT; i++) {
        if(strcmp(buttons[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
int
find_key(char *name) {
    for(int i = 0; i < KEYS_COUNT; i++) {
        if(strcmp(keys[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
int
bind_key(int argc, char **argv, libusb_device_handle *dev_handle, int packet_id) {
    if(argc < 2) {
        usage(); 
        return -1;
    }
    int btni = find_button(argv[0]);
    if(btni == -1) {
        eprintf("Unkown button\n");
        return -1;
    }
    Button btn = buttons[btni];
    int keyi = find_key(argv[1]);
    if(keyi == -1) {
        eprintf("Unkown key\n");
        return -1;
    }
    Key key = keys[keyi];
    unsigned char payload[33] = {0xc, 0x1, 0x4, 0x0, 0x27, 0x1, 0x4, 0x4, 0x50, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    payload[4] = packet_id+1;
    payload[7] = btn.id;
    memcpy(&payload[8], key.id, 3);
    int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, payload, 33, 0);
    return n;
}
#define INVALID_HEX_CHAR 0x10
char
parse_hex_char(char h) {
    if(h >= '0' && h <= '9') {
        return h - '0';
    }
    if(h >= 'a' && h <= 'f') {
        return h - 'a' + 10;
    }
    if(h >= 'A' && h <= 'F') {
        return h - 'A' + 10;
    }
    return INVALID_HEX_CHAR;
}
#define INVALID_COLOR 0xFF000000
uint32_t 
parse_hex_color(const char *str) {
    int len = strlen(str);
    if(len != 6) {
        return INVALID_COLOR;
    }
    uint32_t res = 0;
    int shift = 16;
    for(int i = 0; i < 3; i++) {
        char high   = parse_hex_char(str[i * 2]);
        char low    = parse_hex_char(str[i * 2 + 1]);
        if(high == INVALID_HEX_CHAR || low == INVALID_HEX_CHAR) {
            return INVALID_COLOR;
        }
        char b = ((high & 0xF) << 4) | (low & 0xF);
        res |= (b & 0xFF) << shift;
        shift -= 8;
    }    
    return res;
}
int
set(int argc, char **argv, libusb_device_handle *dev_handle, int packet_id) {
    if(argc < 2) {
        usage(); 
        return -1;
    }
    if(strcmp("poll", argv[0]) == 0) {
        int polling_rate = 0;
        if(strcmp("125", argv[1]) == 0) {
            polling_rate = Hz125;
        } else if(strcmp("250", argv[1]) == 0) {
            polling_rate = Hz250;
        } else if(strcmp("500", argv[1]) == 0) {
            polling_rate = Hz500;
        } else if(strcmp("1000", argv[1]) == 0) {
            polling_rate = Hz1000;
        } else {
            eprintf("Unsupported polling rate\n");
            return -1;
        }
        return set_polling_rate(dev_handle, packet_id, polling_rate);
    }
    else if(strcmp("LOD", argv[0]) == 0) {
        int LOD = atoi(argv[1]);
        if(LOD == 1 || LOD == 2) {
            return set_LOD(dev_handle, packet_id, LOD);
        } else {
            eprintf("LOD must be either 1 or 2\n");
            return -1;
        }
    }
    else if(strcmp("debounce-time", argv[0]) == 0) {
        int debounce_time = atoi(argv[1]);
        if(debounce_time >= 0 && debounce_time <= 30) {
            return set_debounce_time(dev_handle, packet_id, debounce_time);
        } else {
            eprintf("Power down time must be a number from 3 to 10\n");
            return -1;
        }
    }
    else if(strcmp("powerdown-time", argv[0]) == 0) {
        int powerdown_time = atoi(argv[1]);
        if(powerdown_time >= 3 && powerdown_time <= 10) {
            return set_powerdown_time(dev_handle, packet_id, powerdown_time);
        } else {
            eprintf("Power down time must be a number from 3 to 10\n");
            return -1;
        }
    }
    else if(strcmp("motion-wakeup", argv[0]) == 0) {
        int enable = 0;
        if(strcmp(argv[1], "on") == 0) {
            enable = 1;
        } else if(strcmp(argv[1], "off") == 0) {
            enable = 0;
        } else {
            eprintf("Expected 'on' or 'off'\n");
            return -1;
        }
        set_motion_wakeup(dev_handle, packet_id, enable);
    }
    else if(strcmp("dpi", argv[0]) == 0) {
        if(argc == 6) {
            int dpi[5];         
            char *end = NULL;
            for(int i = 0; i < 5; i++) {
                dpi[i] = strtol(argv[1+i], &end, 10);
                if(end[0] != '\0' || dpi[i] < 0 || dpi[i] > 26000 || dpi[i] % 50 != 0) {
                    eprintf("DPI must be from 0 to 26000 and be divisible by 50\n");
                    return -1;
                }
            }
            return set_dpi(dev_handle, packet_id, dpi);
        } else {
            eprintf("Expected 5 values\n");
            return -1;
        }
        return 0;
    }
    else if(strcmp("dpi-colors", argv[0]) == 0) {
        if(argc == 6) {
            uint32_t colors[5];         
            for(int i = 0; i < 5; i++) {
                colors[i] = parse_hex_color(argv[1 + i]);
                if(colors[i] == INVALID_COLOR) {
                    eprintf("Invalid color\n");
                    return -1;
                }
            }
            return set_dpi_colors(dev_handle, packet_id, colors);
        } else {
            eprintf("Expected 5 colors\n");
            return -1;
        }
        return 0;
    } else {
        eprintf("Unknown\n");
        return -1;
    }
}

int 
initial_query(libusb_device_handle *dev_handle, int *packet_id) {
    int i = 1;
    while(1) {
        if(i == 0) {
            return 0;
        }
        unsigned char data[] = {0xc, 0x1, 0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
        data[4] = i++;
        int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, data, 33, 0);
        unsigned char buf[33] = {0};
        n = libusb_control_transfer(dev_handle, 0xa1, 0x1, 0x30c, 1, buf, 33, 0);
        if(n < 0) {
            return n;
        }
        if(buf[18]) {
            *packet_id = i - 1;
            return buf[18];
        }
    }
}
int 
main(int argc, char **argv) {
    if(argc < 2) {
        usage();
        return 0;
    }
    libusb_context *ctx = NULL;
    int ret = libusb_init(&ctx);
    if (ret != LIBUSB_SUCCESS) {
        printf("ERROR: %i %i\n", ret, __LINE__);
        return 0;
    }
    libusb_device *needed_device = NULL;
    libusb_device **list = NULL;
    int count = libusb_get_device_list(ctx, &list);
    for(int i = 0; i < count; i ++) {
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(list[i], &desc);
        if(desc.idVendor == M800_PRO_VID && desc.idProduct == M800_PRO_PID) {
            needed_device = list[i];
        }
    }
    if(needed_device == NULL) {
        eprintf("NOT FOUND\n");
        return 1;
    }
    libusb_device_handle *dev_handle = NULL;
    ret = libusb_open(needed_device, &dev_handle);
    if (ret != LIBUSB_SUCCESS) {
        eprintf("ERROR: %i %i %s\n", ret, __LINE__, libusb_error_name(ret));
        return 1;
    }
    int has_driver = libusb_kernel_driver_active(dev_handle, INTERFACE);
    if(has_driver) {
        ret = libusb_detach_kernel_driver(dev_handle, INTERFACE);
        if (ret != LIBUSB_SUCCESS) {
            eprintf("ERROR: %i %i %s\n", ret, __LINE__, libusb_error_name(ret));
            return 1;
        }
    }
    ret = libusb_claim_interface(dev_handle, INTERFACE);
    if (ret != LIBUSB_SUCCESS) {
        eprintf("ERROR: %i %i %s\n", ret, __LINE__, libusb_error_name(ret));
        return 1;
    }

    int id = 0;
    int charge = initial_query(dev_handle, &id);
    if(charge <= 0) {
        eprintf("ERROR: FAILED TO GET BATTERY CHARGE %i %s\n", charge, libusb_error_name(charge));
        return -1;
    }
    int res = 0;
    if(strcmp("charge", argv[1]) == 0) {
        printf("%i\n", charge);
    } else if(strcmp("set", argv[1]) == 0) {
        res = set(argc-2, argv+2, dev_handle, id);
    } else if(strcmp("bind", argv[1]) == 0) {
        res = bind_key(argc-2, argv+2, dev_handle, id);
    } else if(strcmp("list-buttons", argv[1]) == 0) {
        for(int i = 0; i < BUTTONS_COUNT; i++) {
            Button btn = buttons[i];
            printf("%s\n", btn.name); 
        }
    } else if(strcmp("list-keys", argv[1]) == 0) {
        for(int i = 0; i < KEYS_COUNT; i++) {
            Key key = keys[i];
            printf("%s\n", key.name); 
        }
    } else {
        usage();
    }

    if (res < 0) {
        eprintf("ERROR: %i %s\n", res, libusb_error_name(res));
    }

    libusb_release_interface(dev_handle, INTERFACE);
    if(has_driver) {
        ret = libusb_attach_kernel_driver(dev_handle, INTERFACE);
        if (ret != LIBUSB_SUCCESS) {
            printf("ERROR: %i %i %s\n", ret, __LINE__, libusb_error_name(ret));
            return 1;
        }
    }
    if(res < 0) return 1;
    return 0;
}

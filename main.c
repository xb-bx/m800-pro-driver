#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
    printf("\tquery (charge|poll)\n");
    printf("\tset    poll (125|250|500|1000)\n");
}
int
query(int argc, char **argv, libusb_device_handle *dev_handle, int charge, int packet_id) {
    if(argc == 0) {
        usage(); 
        return -1;
    }
    if(strcmp("charge", argv[0]) == 0) {
        printf("%i\n", charge);
        return 0;
    } else if(strcmp("poll", argv[0]) == 0) {
        eprintf("NOT IMPLEMENTED\n");
        return -1;
    } else {
        eprintf("Unknown\n");
        return -1;
    }
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

    if(strcmp("query", argv[1]) == 0) {
        query(argc-2, argv+2, dev_handle, charge, id);
    } else if(strcmp("set", argv[1]) == 0) {
        set(argc-2, argv+2, dev_handle, id);
    }

    libusb_release_interface(dev_handle, INTERFACE);
    if(has_driver) {
        ret = libusb_attach_kernel_driver(dev_handle, INTERFACE);
        if (ret != LIBUSB_SUCCESS) {
            printf("ERROR: %i %i %s\n", ret, __LINE__, libusb_error_name(ret));
            return 1;
        }
    }
}

#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdint.h>
#define M800_PRO_VID 0x248a
#define M800_PRO_PID 0x5b2f
#define INTERFACE 1
#define eprintf(args...) fprintf(stderr, args)
int main() {
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
    int res = 0;
    int i = 1;
    while(res == 0) {
        unsigned char data[] = {0xc, 0x1, 0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
        data[4] = i++;
        int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, data, 33, 0);
        unsigned char buf[33] = {0};
        n = libusb_control_transfer(dev_handle, 0xa1, 0x1, 0x30c, 1, buf, 33, 0);
        if(n < 0) {
            printf("ERROR: %i %i %s\n", n, __LINE__, libusb_error_name(n));
            return 1;
        }
        if(buf[18])
        printf("%i\n", buf[18]);
        res = buf[18];
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

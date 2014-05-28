#ifndef ANDROID_FREG_INTERFACE_H
#define ANDROID_FREG_INTERFACE_H

#include <hardware/hardware.h>

__BEGIN_DECLS

#define FREG_HARDWARE_MODULE_ID    "freg"

#define FREG_HARDWARE_DEVICE_ID    "freg"

struct freg_module_t{
	struct hw_module_t common;
};

struct freg_device_t{
	struct hw_device_t common;
	int fd;
	int ( *setVal )( struct freg_device_t* dev, int val );
	int ( *getVal )( struct freg_device_t* dev, int* val );
};

__END_DECLS

#endif

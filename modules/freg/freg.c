
#include <hardware/hardware.h>
#include <hardware/freg.h>

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LOG_TAG "freg"
#include <utils/Log.h>

#define DEVICE_NAME     "/dev/freg"
#define MODULE_NAME     "freg"
#define MODULE_AUTHOR   "lshain.gyh@gmail.com"

static int freg_device_open( const struct hw_module_t* module, const char* id, struct hw_device_t** device );
static int freg_device_close( struct hw_device_t* device );

static int freg_setVal( struct freg_device_t* dev, int val );
static int freg_getVal( struct freg_device_t* dev, int* val );

static struct hw_module_methods_t freg_module_methods = {
	.open = freg_device_open
};

struct freg_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.module_api_version = 1,
		.hal_api_version = 0,
		.id = FREG_HARDWARE_MODULE_ID,
		.name = MODULE_NAME,
		.author = MODULE_AUTHOR,
		.methods = &freg_module_methods,
	}
};

static int freg_device_open( const struct hw_module_t* module, const char* id, struct hw_device_t** device ){
	if( !strcmp(id, FREG_HARDWARE_DEVICE_ID) ){
		struct freg_device_t* dev = NULL;

		dev = ( struct freg_device_t* )malloc( sizeof(struct freg_device_t) );
		if( NULL == dev ){
			ALOGE( "Filed to alloc space for freg_device_t." );
			return -EFAULT;
		}

		memset( dev, 0, sizeof(struct freg_device_t) );

		dev->common.tag = HARDWARE_DEVICE_TAG;
		dev->common.version = 0;
		dev->common.module = (hw_module_t*)module;
		dev->common.close = freg_device_close;
		dev->setVal = freg_setVal;
		dev->getVal = freg_getVal;

		if( ( dev->fd = open( DEVICE_NAME, O_RDWR ) ) == -1 ){
			ALOGE( "Faild to open device file /dev/freg -- %s.", strerror( errno ) );
			free( dev );
			return -EFAULT;
		}

		*device = &(dev->common);

		ALOGE( "Open device file /dev/freg successfully." );

		return 0;
	}

	return -EFAULT;
}


static int freg_device_close( struct hw_device_t* device ){
	struct freg_device_t* freg_device = ( struct freg_device_t* )device;
	if( freg_device != NULL ){
		close( freg_device->fd );
		free( freg_device );
	}

	return 0;
}

static int freg_setVal( struct freg_device_t* dev, int val ){
	if( dev == NULL ){
		ALOGE( "Null dev pointer." );
		return -EFAULT;
	}

	ALOGE( "Set value %d to device file /dev/freg.", val );
	write( dev->fd, &val, sizeof(val) );

	return 0;
}

static int freg_getVal( struct freg_device_t* dev, int* val ){
	if( dev == NULL ){
		ALOGE( "Null dev poiter." );
		return -EFAULT;
	}

	if( val == NULL ){
		ALOGE( "Null val pointer." );
		return -EFAULT;
	}

	read( dev->fd, val, sizeof(*val) );

	ALOGE( "Get value %d from device file /dev/freg.", *val );

	return 0;
}

#ifndef OV2640_REGS_H
#define OV2640_REGS_H

#include <stdint.h>

// Forward declaration of sensor_reg structure to prevent circular dependency in ov2640.c
struct sensor_reg;

// Declaration of sensor_reg arrays
extern const struct sensor_reg OV2640_QVGA[];
extern const struct sensor_reg OV2640_JPEG_INIT[];
extern const struct sensor_reg OV2640_YUV422[];
extern const struct sensor_reg OV2640_JPEG[];
extern const struct sensor_reg OV2640_160x120_JPEG[];
extern const struct sensor_reg OV2640_176x144_JPEG[];
extern const struct sensor_reg OV2640_320x240_JPEG[];
extern const struct sensor_reg OV2640_352x288_JPEG[];
extern const struct sensor_reg OV2640_640x480_JPEG[];
extern const struct sensor_reg OV2640_800x600_JPEG[];
extern const struct sensor_reg OV2640_1024x768_JPEG[];
extern const struct sensor_reg OV2640_1280x1024_JPEG[];
extern const struct sensor_reg OV2640_1600x1200_JPEG[];

#endif // OV2640_REGS_H
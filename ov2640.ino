#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include "memorysaver.h"
#include "C:\Users\Adnan\Desktop\ECE\Post Graduation\Side Projects\ov2640\ov2640_regs.h"

//This code can only work on OV2640_MINI_2MP platform.
#if !(defined OV2640_MINI_2MP)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif

#define BYTE_PACKET_SIZE 256
#define BAUD_RATE 115200

#define ARDUCAM_PERIOD 4500

// OV2640 specific stuff

#define OV2640_CPLD_ADDR              0x07
#define OV2640_CPLD_REG               0x80
#define OV2640_CPLD_RESET             0x00

#define OV2640_FIFO_CONTROL			      0x04
#define OV2640_FIFO_CLEAR_MASK	      0x01
#define OV2640_FIFO_START_MASK	      0x02

#define OV2640_FIFO_SIZE1				      0x42
#define OV2640_FIFO_SIZE2				      0x43
#define OV2640_FIFO_SIZE3				      0x44

#define OV2640_CAPTURE_TRIGGER			  0x41
#define OV2640_CAPTURE_VSYNC_MASK		  0x01
#define OV2640_CAPTURE_SHUTTER_MASK	  0x02
#define OV2640_CAPTURE_DONE_MASK		  0x08

#define OV2640_FIFO_BURST_READ			  0x3C

#define OV2640_CAPTURE_MIN_LENGTH     1
#define OV2640_CAPTURE_MAX_LENGTH     0x5FFFE

typedef struct ov2640 {
	uint16_t spi_cs_pin;
  // image type (enum, but I'll probably only do JPEG)
  // image size (enum)

  uint32_t fifo_length;

  uint8_t vid;
  uint8_t pid;
} ov2640;


ov2640 my_camera;

// Arducam global variables
const int CS = 7;

void ov2640_fifo_write(ov2640 * camera, uint8_t addr, uint8_t data)
{
  digitalWrite(camera->spi_cs_pin, LOW);
  addr = addr | 0x80;
  SPI.transfer(addr);
  SPI.transfer(data);
  digitalWrite(camera->spi_cs_pin, HIGH);
}

uint8_t ov2640_fifo_read(ov2640 * camera, uint8_t addr)
{
  digitalWrite(camera->spi_cs_pin, LOW);
  addr = addr & 0x7F;
  uint8_t value;
  SPI.transfer(addr);
  value = SPI.transfer(0x00);
  digitalWrite(camera->spi_cs_pin, HIGH);
  return value;
}

void ov2640_fifo_clear(ov2640 * camera)
{
  ov2640_fifo_write(camera, OV2640_FIFO_CONTROL, OV2640_FIFO_CLEAR_MASK);
  camera->fifo_length = 0;
}

void ov2640_fifo_start(ov2640 * camera)
{
  ov2640_fifo_write(camera, OV2640_FIFO_CONTROL, OV2640_FIFO_START_MASK);
}

uint8_t ov2640_fifo_get_bit(ov2640 * camera, uint8_t addr, uint8_t mask)
{
  uint8_t temp = ov2640_fifo_read(camera, addr);
  return (temp &= mask);
}

uint32_t ov2640_fifo_read_length(ov2640 * camera)
{
  uint32_t len1,len2,len3,length=0;

  len1 = ov2640_fifo_read(camera, OV2640_FIFO_SIZE1);
  len2 = ov2640_fifo_read(camera, OV2640_FIFO_SIZE2);
  len3 = ov2640_fifo_read(camera, OV2640_FIFO_SIZE3) & 0x7f;

  length = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
  return length;
}

uint8_t ov2640_sensor_write_byte(ov2640 * camera, uint8_t reg, uint8_t data)
{
  Wire.beginTransmission(0x42 >> 1);
  Wire.write(reg & 0x00FF);
  Wire.write(data & 0x00FF);
  if(Wire.endTransmission())
  {
    return 0;
  }
  delay(1);
  return 1;
}

uint8_t ov2640_sensor_write_bytes(ov2640 * camera, const struct sensor_reg reglist[])
{
  uint8_t err = 0;
  uint16_t reg_addr = 0;
  uint16_t reg_val = 0;
  const struct sensor_reg *next = reglist;
  
  while ((reg_addr != 0xff) | (reg_val != 0xff))
  {
    reg_addr = next->reg;
    reg_val = next->val;

    err = ov2640_sensor_write_byte(camera, reg_addr, reg_val);
    next++;
  }
  return 1;
}

void ov2640_init(ov2640 * camera, uint16_t spi_cs_pin) {
  camera->spi_cs_pin = spi_cs_pin;

    // OV2640 setup
  uint8_t vid, pid;
  uint8_t temp;

  // initialize i2c
  Wire.begin();
  // initialize SPI, cs pin is active low so start at high
  pinMode(camera->spi_cs_pin, OUTPUT);
  digitalWrite(camera->spi_cs_pin, HIGH);
  SPI.begin();

  // Reset the CPLD
  ov2640_fifo_write(camera, OV2640_CPLD_ADDR, OV2640_CPLD_REG);
  delay(100);
  ov2640_fifo_write(camera, OV2640_CPLD_ADDR, OV2640_CPLD_RESET);
  delay(100);

  // Set JPEG format
  ov2640_sensor_write_byte(camera, 0xff, 0x01);
  ov2640_sensor_write_byte(camera, 0x12, 0x80);
  delay(100);

  ov2640_sensor_write_bytes(camera, OV2640_JPEG_INIT);
  ov2640_sensor_write_bytes(camera, OV2640_YUV422);
  ov2640_sensor_write_bytes(camera, OV2640_JPEG);

  // Set JPEG size
  ov2640_sensor_write_byte(camera, 0xff, 0x01);
  ov2640_sensor_write_byte(camera, 0x15, 0x00);
  ov2640_sensor_write_bytes(camera, OV2640_320x240_JPEG);

  delay(1000);

  // Clear FIFO flag
  ov2640_fifo_clear(camera);
}

void ov2640_get_capture(ov2640 * camera)
{
  // flush, clear, then start capture
  ov2640_fifo_clear(camera);
  ov2640_fifo_clear(camera);
  ov2640_fifo_start(camera);

  // wait until the capture is complete
  while(!ov2640_fifo_get_bit(camera, OV2640_CAPTURE_TRIGGER, OV2640_CAPTURE_DONE_MASK));
  delay(50);

  camera->fifo_length = ov2640_fifo_read_length(camera);

  // discard captures that are obviously invalid through length
  if(camera->fifo_length > OV2640_CAPTURE_MAX_LENGTH | camera->fifo_length < OV2640_CAPTURE_MIN_LENGTH)
  {
    ov2640_fifo_write(camera, OV2640_FIFO_CONTROL, OV2640_FIFO_CLEAR_MASK);
    camera->fifo_length = 0;
  }
}

// Helper functions for the byte buffer in ov2640_stream_capture
// the buffer gets a byte at a time added to it and is dumped to Serial whenever it's filled up

void dump_buffer(uint8_t buffer[], int index) {  
  for (int i = 0; i < index; i++) {
    Serial.println(buffer[i], HEX);
  }
  //delay(5000);
}
 
int add_to_buffer(uint8_t buffer[], int index, uint8_t elem) {
  if (index < BYTE_PACKET_SIZE) {
    buffer[index] = elem;
    return index + 1;
  } else {
    dump_buffer(buffer, BYTE_PACKET_SIZE);
    buffer[0] = elem;
    return 1;
  }
}

uint8_t ov2640_transfer_capture(ov2640 * camera, uint8_t buffer[], uint32_t buffer_size, uint32_t * buffer_filled)
{
  uint8_t rc = 0;
  static uint8_t curr_byte, prev_byte;
  uint32_t i=0;

  // start of image pattern in the header must be inserted into buffer if edge case 2 occurs on previous function call
  if(curr_byte == 0xD8 && prev_byte == 0xFF) {
    buffer[0] = 0xFF;
    buffer[1] = 0xD8;
    i += 2;
  }

  while(i<buffer_size) {
    // get the next byte from the fifo buffer and put into input buffer, keeping track of the previous byte
    prev_byte = curr_byte;
    curr_byte = SPI.transfer(0x00);    
    buffer[i] = curr_byte;

    // iterating
    camera->fifo_length--;
    ++i;

    // expected case, happens when the fifo buffer reaches the end of the current image (buffer may or may not be 100% filled)
    // function exits, and you handle the size issue by going up to buffer_filled when iterating through buffer
    if(curr_byte == 0xD9 && prev_byte == 0xFF) {
      break;
    }
    // edge case 1, happens when the fifo buffer runs out of image data without receiving end of image pattern
    // function exits, but something unexpected happened on the OV2640 side
    else if(camera->fifo_length == 0) {
      rc = 1;
      break;
    }
    
    // edge case 2, happens when the fifo buffer reaches a start of image footer before the end of image pattern of current image
    // function exits, but the buffer data is now useless. bytes indicating start of new image are preserved for next function call
    else if(curr_byte == 0xD8 && prev_byte == 0xFF) {
      rc = 2;
      break;
    }
  }

  // number of filled elements in input buffer is passed back through the iterator, use if any of the cases above occur
  *buffer_filled = i;
  return rc;
}

void setup() {
  Serial.begin(BAUD_RATE);
  ov2640_init(&my_camera, 7);
}

void loop() {
  while (my_camera.fifo_length == 0) {
    ov2640_get_capture(&my_camera);
  }

  // cs pin must be held low for the entire duration of reading the fifo buffer
  digitalWrite(my_camera.spi_cs_pin, LOW);
  {
    SPI.transfer(OV2640_FIFO_BURST_READ);

    while(my_camera.fifo_length > 0) {
      uint8_t buffer[20];
      uint32_t buffer_filled;
      uint8_t rc = ov2640_transfer_capture(&my_camera, buffer, sizeof(buffer), &buffer_filled);
      
      for (int i = 0; i < buffer_filled; i++) {
        Serial.println(buffer[i], HEX);
      }
    }
  }
  digitalWrite(my_camera.spi_cs_pin, HIGH);

  delay(ARDUCAM_PERIOD);
}
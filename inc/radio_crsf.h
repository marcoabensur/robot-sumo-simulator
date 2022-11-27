#ifndef INC_CRSF_H_
#define INC_CRSF_H_

#include <stdint.h>


#define CRSF_RX_BAUDRATE                420000
#define CRSF_TX_BAUDRATE_FAST           400000
#define CRSF_TX_BAUDRATE_SLOW           115200
#define CRSF_NUM_CHANNELS               16

#define CRSF_SYNC_BYTE                  0xC8

#define CRSF_PAYLOAD_SIZE_MAX           62
#define CRSF_FRAME_START_BYTES          2 // address + len (start of the CRSF frame, not counted to frame len)
#define CRSF_FRAME_HEADER_BYTES         2 // type + crc

#define CRSF_FRAME_SIZE(payload_size)       ((payload_size) + CRSF_FRAME_HEADER_BYTES) // See crsf_header_t.frame_size
#define CRSF_EXT_FRAME_SIZE(payload_size)   ( CRSF_FRAME_SIZE(payload_size) + CRSF_FRAME_START_BYTES)
#define CRSF_FRAME_SIZE_MAX                 (CRSF_PAYLOAD_SIZE_MAX + CRSF_FRAME_START_BYTES)

#define CRSF_MSP_FRAME_HEADER_BYTES 4 // type, dest, orig, crc
#define CRSF_MSP_FRAME_SIZE(payload_size) (CRSF_FRAME_SIZE(payload_size) + CRSF_MSP_FRAME_HEADER_BYTES)


// OUT to flight controller
#define ELRS_MIN        0
#define ELRS_MAX        1023
#define ELRS_SWITCH_MIN 0
#define ELRS_SWITCH_MAX 6

#define CRSF_MIN        172
#define CRSF_MID        992
#define CRSF_MAX        1811

#define RX_BAUDRATE     CRSF_RX_BAUDRATE


#define CRSF_FRAME_RX_MSP_FRAME_SIZE 8
#define CRSF_FRAME_TX_MSP_FRAME_SIZE 58

typedef enum crsf_packet_ret {

    CRSF_PACKET,
    CRSF_PACKET_NORMAL,
    CRSF_PACKET_COMPLETE,
    CRSF_PACKET_ERROR

} crsf_packet_ret_t;


enum crsf_frame_type_e
{
    CRSF_FRAMETYPE_GPS = 0x02,
    CRSF_FRAMETYPE_BATTERY_SENSOR = 0x08,
    CRSF_FRAMETYPE_LINK_STATISTICS = 0x14,
    CRSF_FRAMETYPE_LINK_STATISTICS_ELRS = 0x15,
    CRSF_FRAMETYPE_OPENTX_SYNC = 0x10,
    CRSF_FRAMETYPE_RADIO_ID = 0x3A,
    CRSF_FRAMETYPE_RC_CHANNELS_PACKED = 0x16,
    CRSF_FRAMETYPE_RC_CHANNELS_PACKED_ELRS = 0x17,
    CRSF_FRAMETYPE_SUBSET_RC_CHANNELS_PACKED = 0x17,
    CRSF_FRAMETYPE_LINK_STATISTICS_RX = 0x1C,
    CRSF_FRAMETYPE_LINK_STATISTICS_TX = 0x1D,
    CRSF_FRAMETYPE_ATTITUDE = 0x1E,
    CRSF_FRAMETYPE_FLIGHT_MODE = 0x21,
    // Extended Header Frames, range: 0x28 to 0x96
    CRSF_FRAMETYPE_DEVICE_PING = 0x28,
    CRSF_FRAMETYPE_DEVICE_INFO = 0x29,
    CRSF_FRAMETYPE_PARAMETER_SETTINGS_ENTRY = 0x2B,
    CRSF_FRAMETYPE_PARAMETER_READ = 0x2C,
    CRSF_FRAMETYPE_PARAMETER_WRITE = 0x2D,
    CRSF_FRAMETYPE_COMMAND = 0x32,
    // MSP commands
    CRSF_FRAMETYPE_MSP_REQ = 0x7A,   // response request using msp sequence as command
    CRSF_FRAMETYPE_MSP_RESP = 0x7B,  // reply with 58 byte chunked binary
    CRSF_FRAMETYPE_MSP_WRITE = 0x7C, // write with 8 byte chunked binary (OpenTX outbound telemetry buffer limit)
    CRSF_FRAMETYPE_DISPLAYPORT_CMD = 0x7D, // displayport control command
};

enum
{
    CRSF_COMMAND_SUBCMD_GENERAL = 0x0A,    // general command
};

enum
{
    CRSF_COMMAND_SUBCMD_GENERAL_CRSF_SPEED_PROPOSAL = 0x70,    // proposed new CRSF port speed
    CRSF_COMMAND_SUBCMD_GENERAL_CRSF_SPEED_RESPONSE = 0x71,    // response to the proposed CRSF port speed
};

enum crsf_addr_e
{
    CRSF_ADDRESS_BROADCAST = 0x00,
    CRSF_ADDRESS_USB = 0x10,
    CRSF_ADDRESS_TBS_CORE_PNP_PRO = 0x80,
    CRSF_ADDRESS_RESERVED1 = 0x8A,
    CRSF_ADDRESS_CURRENT_SENSOR = 0xC0,
    CRSF_ADDRESS_GPS = 0xC2,
    CRSF_ADDRESS_TBS_BLACKBOX = 0xC4,
    CRSF_ADDRESS_FLIGHT_CONTROLLER = 0xC8,
    CRSF_ADDRESS_RESERVED2 = 0xCA,
    CRSF_ADDRESS_RACE_TAG = 0xCC,
    CRSF_ADDRESS_RADIO_TRANSMITTER = 0xEA,
    CRSF_ADDRESS_CRSF_RECEIVER = 0xEC,
    CRSF_ADDRESS_CRSF_TRANSMITTER = 0xEE,
};



typedef struct crsf_header_s
{
    uint8_t device_addr; // from crsf_addr_e
    uint8_t frame_size;  // counts size after this byte, so it must be the payload size + 2 (type and crc)
    uint8_t type;        // from crsf_frame_type_e
} __attribute__((packed)) crsf_header_t;

typedef struct crsf_channels_s
{
    unsigned ch0 : 11;
    unsigned ch1 : 11;
    unsigned ch2 : 11;
    unsigned ch3 : 11;
    unsigned ch4 : 11;
    unsigned ch5 : 11;
    unsigned ch6 : 11;
    unsigned ch7 : 11;
    unsigned ch8 : 11;
    unsigned ch9 : 11;
    unsigned ch10 : 11;
    unsigned ch11 : 11;
    unsigned ch12 : 11;
    unsigned ch13 : 11;
    unsigned ch14 : 11;
    unsigned ch15 : 11;
} __attribute__((packed)) crsf_channels_t;




crsf_packet_ret_t crsf_parse_byte(uint8_t inChar);
void crsf_get_rc_data(uint16_t * const rc_data, uint8_t len);

#endif /* INC_CRSF_H_ */

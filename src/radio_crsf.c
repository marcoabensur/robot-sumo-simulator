/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "radio_crsf.h"
#include <stddef.h>
#include "utils.h"

/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/
#define CRSF_SUBSET_RC_STARTING_CHANNEL_BITS        5
#define CRSF_SUBSET_RC_STARTING_CHANNEL_MASK        0x1F
#define CRSF_SUBSET_RC_RES_CONFIGURATION_BITS       2
#define CRSF_SUBSET_RC_RES_CONFIGURATION_MASK       0x03
#define CRSF_SUBSET_RC_RESERVED_CONFIGURATION_BITS  1

#define CRSF_RC_CHANNEL_SCALE_LEGACY                0.62477120195241f
#define CRSF_SUBSET_RC_RES_CONF_10B                 0
#define CRSF_SUBSET_RC_RES_BITS_10B                 10
#define CRSF_SUBSET_RC_RES_MASK_10B                 0x03FF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_10B            1.0f
#define CRSF_SUBSET_RC_RES_CONF_11B                 1
#define CRSF_SUBSET_RC_RES_BITS_11B                 11
#define CRSF_SUBSET_RC_RES_MASK_11B                 0x07FF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_11B            0.5f
#define CRSF_SUBSET_RC_RES_CONF_12B                 2
#define CRSF_SUBSET_RC_RES_BITS_12B                 12
#define CRSF_SUBSET_RC_RES_MASK_12B                 0x0FFF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_12B            0.25f
#define CRSF_SUBSET_RC_RES_CONF_13B                 3
#define CRSF_SUBSET_RC_RES_BITS_13B                 13
#define CRSF_SUBSET_RC_RES_MASK_13B                 0x1FFF
#define CRSF_SUBSET_RC_CHANNEL_SCALE_13B            0.125f

#define FAILSAFE_TRIGGER_TIME 200
#define CRSF_MAX_CHAHNELS 24 // old CRSF 16, CRSFv3 24

#define ANALOG_MIN  0U
#define ANALOG_MID  1024U
#define ANALOG_MAX  2047U


/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/

enum {
    IN_TYPE_LEGACY = 0,
    IN_TYPE_V3_10B = (1 << 10) - 1,
    IN_TYPE_V3_11B = (1 << 11) - 1,
    IN_TYPE_V3_12B = (1 << 12) - 1,
    IN_TYPE_V3_13B = (1 << 13) - 1,
};

typedef struct {
    // Common header fields, see crsf_header_t
    uint8_t device_addr;
    uint8_t frame_size;
    uint8_t type;
    // Extended fields
    uint8_t dest_addr;
    uint8_t orig_addr;
    // Command fields
    uint8_t command;
    uint8_t sub_command;
    // Message
    uint8_t portID;
    uint8_t status; // true / false if baud ok
    // footer
    uint8_t crc_cmd; // crc of type + payload
    uint8_t crc;
} __attribute__((packed)) crsf_v3_speed_control_resp_t;

typedef struct {
    // Extended fields
    uint8_t dest_addr;
    uint8_t orig_addr;
    // proposal
    uint8_t command;
    uint8_t sub_command;
    uint8_t portID;
    uint32_t baudrate;
    uint8_t crc_cmd; // crc of type + payload
    uint8_t crc;
} __attribute__((packed)) crsf_v3_speed_control_req_t;

/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

static uint8_t CalcCRC(uint8_t data, uint8_t crc);

static crsf_packet_ret_t crsf_check_msg(uint8_t const * const input, uint8_t const frameLength);
// static void send_negotiate_resp(void);
static void reconfigure_uart(void); 

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/

static uint8_t SerialInBuffer[256];
static uint8_t CRSFframeActive;
static uint8_t SerialInCrc;
static uint8_t SerialInPacketStart;
static uint8_t SerialInPacketLen;      // length of the CRSF packet as measured
static uint8_t SerialInPacketPtr;      // index where we are reading/writing

static uint32_t SerialInPacketType;
static uint16_t _channels[CRSF_MAX_CHAHNELS];

static uint32_t baudrate;

static uint8_t crc8tab[256] = {
    0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
    0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
    0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
    0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
    0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
    0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
    0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
    0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
    0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
    0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
    0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
    0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
    0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
    0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
    0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
    0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9
};


/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

static uint8_t CalcCRC(uint8_t data, uint8_t crc)
{
    return crc8tab[crc ^ data];
}


static crsf_packet_ret_t crsf_check_msg(uint8_t const * const input, uint8_t const frameLength)
{
    switch (input[0]) {

        case CRSF_FRAMETYPE_RC_CHANNELS_PACKED: {
            // use ordinary RC frame structure (0x16)
            crsf_channels_t * channels = (crsf_channels_t*)&input[1];
            _channels[0] = channels->ch0;
            _channels[1] = channels->ch1;
            _channels[2] = channels->ch2;
            _channels[3] = channels->ch3;
            _channels[4] = channels->ch4;
            _channels[5] = channels->ch5;
            _channels[6] = channels->ch6;
            _channels[7] = channels->ch7;
            _channels[8] = channels->ch8;
            _channels[9] = channels->ch9;
            _channels[10] = channels->ch10;
            _channels[11] = channels->ch11;
            _channels[12] = channels->ch12;
            _channels[13] = channels->ch13;
            _channels[14] = channels->ch14;
            _channels[15] = channels->ch15;
            SerialInPacketType = IN_TYPE_LEGACY;

            return CRSF_PACKET_COMPLETE;
            break;
        }


        case CRSF_FRAMETYPE_SUBSET_RC_CHANNELS_PACKED: {
            // use subset RC frame structure (0x17) aka CRSFv3
            // This code is copied from Betaflight (https://github.com/betaflight/betaflight/)

            uint16_t channelMask;
            uint8_t channelBits;
            uint8_t readByteIndex = 1;
            // get the configuration byte
            uint8_t configByte = input[readByteIndex++];
            // get the channel number of start channel
            uint8_t const startChannel = configByte & CRSF_SUBSET_RC_STARTING_CHANNEL_MASK;
            configByte >>= CRSF_SUBSET_RC_STARTING_CHANNEL_BITS;

            // get the channel resolution settings
            uint8_t channelRes = configByte & CRSF_SUBSET_RC_RES_CONFIGURATION_MASK;
            //configByte >>= CRSF_SUBSET_RC_RES_CONFIGURATION_BITS;
            switch (channelRes) {
                case CRSF_SUBSET_RC_RES_CONF_10B:
                    channelBits = CRSF_SUBSET_RC_RES_BITS_10B;
                    channelMask = CRSF_SUBSET_RC_RES_MASK_10B;
                    SerialInPacketType = IN_TYPE_V3_10B;
                    break;
                default:
                case CRSF_SUBSET_RC_RES_CONF_11B:
                    channelBits = CRSF_SUBSET_RC_RES_BITS_11B;
                    channelMask = CRSF_SUBSET_RC_RES_MASK_11B;
                    SerialInPacketType = IN_TYPE_V3_11B;
                    break;
                case CRSF_SUBSET_RC_RES_CONF_12B:
                    channelBits = CRSF_SUBSET_RC_RES_BITS_12B;
                    channelMask = CRSF_SUBSET_RC_RES_MASK_12B;
                    SerialInPacketType = IN_TYPE_V3_12B;
                    break;
                case CRSF_SUBSET_RC_RES_CONF_13B:
                    channelBits = CRSF_SUBSET_RC_RES_BITS_13B;
                    channelMask = CRSF_SUBSET_RC_RES_MASK_13B;
                    SerialInPacketType = IN_TYPE_V3_13B;
                    break;
            }

            // do nothing for the reserved configuration bit
            //configByte >>= CRSF_SUBSET_RC_RESERVED_CONFIGURATION_BITS;

            // calculate the number of channels packed
            uint8_t const numOfChannels =
                ((frameLength - CRSF_FRAME_HEADER_BYTES - 1) * 8) / channelBits;

            // unpack the channel data
            uint8_t bitsMerged = 0;
            uint32_t readValue = 0;
            for (uint8_t n = 0; n < numOfChannels; n++) {
                while (bitsMerged < channelBits) {
                    uint8_t readByte = input[readByteIndex++];
                    readValue |= ((uint32_t) readByte) << bitsMerged;
                    bitsMerged += 8;
                }
                _channels[startChannel + n] = readValue & channelMask;
                readValue >>= channelBits;
                bitsMerged -= channelBits;
            }
            break;
        }

        case CRSF_FRAMETYPE_COMMAND: {
//            crsf_v3_speed_control_req_t * req =
//                (crsf_v3_speed_control_req_t*)&input[1];

            /* TODO: Handle baudrate negotiation */

            break;
        }
        default:
            return CRSF_PACKET;
    };

    return CRSF_PACKET_NORMAL;
}

// static void send_negotiate_resp(void) {
//     crsf_v3_speed_control_resp_t resp;
//     resp.device_addr = CRSF_SYNC_BYTE;
//     resp.frame_size = sizeof(crsf_v3_speed_control_resp_t) - CRSF_FRAME_HEADER_BYTES;
//     resp.type = CRSF_FRAMETYPE_COMMAND;
//     resp.dest_addr = CRSF_ADDRESS_CRSF_RECEIVER;
//     resp.orig_addr = CRSF_ADDRESS_FLIGHT_CONTROLLER;
//     resp.command = CRSF_COMMAND_SUBCMD_GENERAL;
//     resp.sub_command = CRSF_COMMAND_SUBCMD_GENERAL_CRSF_SPEED_RESPONSE;
//     resp.portID = portID;
//     resp.status = 1;
//     resp.crc_cmd = CalcCRC8len(&resp.type, 7, 0, 0xBA);
//     resp.crc = CalcCRClen(&resp.type, sizeof(resp) - 3, 0);
// }


static void reconfigure_uart(void) {
    if (baudrate) {
        //uart_deinit();
        //uart_init(RX_BAUDRATE, RECEIVER_UART_RX, DBG_UART_TX);
        //send_negotiate_resp();
        baudrate = 0;
    }
}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/

crsf_packet_ret_t crsf_parse_byte(uint8_t inChar) {
    crsf_packet_ret_t ret = CRSF_PACKET;

    if (SerialInPacketPtr >= sizeof(SerialInBuffer)) {
        // we reached the maximum allowable packet length,
        // so start again because shit fucked up hey.
        SerialInPacketPtr = 0;
        CRSFframeActive = 0;
    }

    // store byte
    SerialInBuffer[SerialInPacketPtr++] = inChar;

    // CRSF Frame:
    // | address | payload_len | payload* | crc |

    if (CRSFframeActive == 0) {
        if (inChar == CRSF_ADDRESS_CRSF_RECEIVER ||
                inChar == CRSF_ADDRESS_CRSF_TRANSMITTER ||
                inChar == CRSF_SYNC_BYTE ||
                inChar == CRSF_ADDRESS_FLIGHT_CONTROLLER ||
                inChar == CRSF_ADDRESS_BROADCAST) {
            CRSFframeActive = 1;
            SerialInPacketLen = 0;
        } else {
            SerialInPacketPtr = 0;
        }
    } else {
        if (SerialInPacketLen == 0) {
            // we read the packet length and save it
            SerialInCrc = 0;
            SerialInPacketLen = inChar;
            SerialInPacketStart = SerialInPacketPtr;
            if ((SerialInPacketLen < 2) || (CRSF_FRAME_SIZE_MAX < SerialInPacketLen)) {
                // failure -> discard
                CRSFframeActive = 0;
                SerialInPacketPtr = 0;
            }
        } else {
            if ((SerialInPacketPtr - SerialInPacketStart) >= (SerialInPacketLen)) {
                /* Check packet CRC */
                if (SerialInCrc == inChar) {
                    ret = crsf_check_msg(&SerialInBuffer[SerialInPacketStart], SerialInPacketLen);
                }

                // packet handled, start next
                CRSFframeActive = 0;
                SerialInPacketPtr = 0;
                SerialInPacketLen = 0;
            } else {
                // Calc crc on the fly
                SerialInCrc = CalcCRC(inChar, SerialInCrc);
            }
        }
    }

    return ret;
}


void crsf_get_rc_data(uint16_t * const rc_data, uint8_t len) {
    uint8_t iter;
    uint16_t _min = CRSF_MIN;
    uint16_t _max = CRSF_MAX;

    if (SerialInPacketType) {
        _min = 0;
        _max = SerialInPacketType;
    }

    reconfigure_uart();

    for (iter = 0; iter < len && iter < len(_channels); iter++) {
        rc_data[iter] = MAP_U16(_channels[iter],
            _min, _max, ANALOG_MIN, ANALOG_MAX);
    }

}

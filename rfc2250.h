#ifndef RFC2250_H
#define RFC2250_H
#include <netinet/ip.h>

struct RTPFixedHeader
{
#if     __BYTE_ORDER == __BIG_ENDIAN
    /// For big endian boxes
    unsigned char version:2;       ///< Version, currently 2
    unsigned char padding:1;       ///< Padding bit
    unsigned char extension:1;     ///< Extension bit
    unsigned char cc:4;            ///< CSRC count
    unsigned char marker:1;        ///< Marker bit
    unsigned char payload:7;       ///< Payload type
#else
    /// For little endian boxes
    unsigned char cc:4;            ///< CSRC count
    unsigned char extension:1;     ///< Extension bit
    unsigned char padding:1;       ///< Padding bit
    unsigned char version:2;       ///< Version, currently 2
    unsigned char payload:7;       ///< Payload type
    unsigned char marker:1;        ///< Marker bit
#endif
    u_int16_t sequence;        ///< sequence number
    u_int32_t timestamp;       ///< timestamp
    u_int32_t sources[1];      ///< contributing sources
};
//-------------------------------------------------------------------
struct RFC2250H
{
#if     __BYTE_ORDER == __BIG_ENDIAN
    /// For big endian boxes
    u_int8_t MBZ:5;
    u_int8_t T:1;
    u_int16_t temporal_refrence:10;

    u_int8_t AN:1;
    u_int8_t new_picture_header:1;
    u_int8_t sequence_header:1;
    bool     beginning_of_slice:1;
    bool     end_of_slice:1;
    u_int8_t picture_type:3;

    u_int8_t FBV:1;
    u_int8_t BFC:3;
    u_int8_t FFV:1;
    u_int8_t FFC:3;

#else
/// For little endian boxes
    u_char    padding:2;        // We lost 2 most significant bit .... BUG
    u_int8_t T:1;
    u_int8_t MBZ:5;

    u_int16_t temporal_refrence:8;

    u_int8_t picture_type:3;
    bool     end_of_slice:1;
    bool     beginning_of_slice:1;
    u_int8_t sequence_header:1;
    u_int8_t new_picture_header:1;
    u_int8_t AN:1;

    u_int8_t FFC:3;
    u_int8_t FFV:1;
    u_int8_t BFC:3;
    u_int8_t FBV:1;
#endif
};

#endif // RFC2250_H

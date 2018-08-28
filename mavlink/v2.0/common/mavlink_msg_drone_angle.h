// MESSAGE DRONE_ANGLE PACKING

#define MAVLINK_MSG_ID_DRONE_ANGLE 13

MAVPACKED(
typedef struct __mavlink_drone_angle_t {
 int32_t lat; /*< Latitude (WGS84), in degrees * 1E7*/
 int32_t lon; /*< Longitude (WGS84), in degrees * 1E7*/
}) mavlink_drone_angle_t;

#define MAVLINK_MSG_ID_DRONE_ANGLE_LEN 8
#define MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN 8
#define MAVLINK_MSG_ID_13_LEN 8
#define MAVLINK_MSG_ID_13_MIN_LEN 8

#define MAVLINK_MSG_ID_DRONE_ANGLE_CRC 166
#define MAVLINK_MSG_ID_13_CRC 166



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DRONE_ANGLE { \
	13, \
	"DRONE_ANGLE", \
	2, \
	{  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_drone_angle_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_drone_angle_t, lon) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DRONE_ANGLE { \
	"DRONE_ANGLE", \
	2, \
	{  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_drone_angle_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_drone_angle_t, lon) }, \
         } \
}
#endif

/**
 * @brief Pack a drone_angle message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param lat Latitude (WGS84), in degrees * 1E7
 * @param lon Longitude (WGS84), in degrees * 1E7
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_drone_angle_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       int32_t lat, int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DRONE_ANGLE_LEN];
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DRONE_ANGLE_LEN);
#else
	mavlink_drone_angle_t packet;
	packet.lat = lat;
	packet.lon = lon;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DRONE_ANGLE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_DRONE_ANGLE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_CRC);
}

/**
 * @brief Pack a drone_angle message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param lat Latitude (WGS84), in degrees * 1E7
 * @param lon Longitude (WGS84), in degrees * 1E7
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_drone_angle_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           int32_t lat,int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DRONE_ANGLE_LEN];
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DRONE_ANGLE_LEN);
#else
	mavlink_drone_angle_t packet;
	packet.lat = lat;
	packet.lon = lon;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DRONE_ANGLE_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_DRONE_ANGLE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_CRC);
}

/**
 * @brief Encode a drone_angle struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param drone_angle C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_drone_angle_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_drone_angle_t* drone_angle)
{
	return mavlink_msg_drone_angle_pack(system_id, component_id, msg, drone_angle->lat, drone_angle->lon);
}

/**
 * @brief Encode a drone_angle struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param drone_angle C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_drone_angle_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_drone_angle_t* drone_angle)
{
	return mavlink_msg_drone_angle_pack_chan(system_id, component_id, chan, msg, drone_angle->lat, drone_angle->lon);
}

/**
 * @brief Send a drone_angle message
 * @param chan MAVLink channel to send the message
 *
 * @param lat Latitude (WGS84), in degrees * 1E7
 * @param lon Longitude (WGS84), in degrees * 1E7
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_drone_angle_send(mavlink_channel_t chan, int32_t lat, int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DRONE_ANGLE_LEN];
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DRONE_ANGLE, buf, MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_CRC);
#else
	mavlink_drone_angle_t packet;
	packet.lat = lat;
	packet.lon = lon;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DRONE_ANGLE, (const char *)&packet, MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_CRC);
#endif
}

/**
 * @brief Send a drone_angle message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_drone_angle_send_struct(mavlink_channel_t chan, const mavlink_drone_angle_t* drone_angle)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_drone_angle_send(chan, drone_angle->lat, drone_angle->lon);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DRONE_ANGLE, (const char *)drone_angle, MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_CRC);
#endif
}

#if MAVLINK_MSG_ID_DRONE_ANGLE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_drone_angle_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int32_t lat, int32_t lon)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DRONE_ANGLE, buf, MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_CRC);
#else
	mavlink_drone_angle_t *packet = (mavlink_drone_angle_t *)msgbuf;
	packet->lat = lat;
	packet->lon = lon;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DRONE_ANGLE, (const char *)packet, MAVLINK_MSG_ID_DRONE_ANGLE_MIN_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_LEN, MAVLINK_MSG_ID_DRONE_ANGLE_CRC);
#endif
}
#endif

#endif

// MESSAGE DRONE_ANGLE UNPACKING


/**
 * @brief Get field lat from drone_angle message
 *
 * @return Latitude (WGS84), in degrees * 1E7
 */
static inline int32_t mavlink_msg_drone_angle_get_lat(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field lon from drone_angle message
 *
 * @return Longitude (WGS84), in degrees * 1E7
 */
static inline int32_t mavlink_msg_drone_angle_get_lon(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Decode a drone_angle message into a struct
 *
 * @param msg The message to decode
 * @param drone_angle C-struct to decode the message contents into
 */
static inline void mavlink_msg_drone_angle_decode(const mavlink_message_t* msg, mavlink_drone_angle_t* drone_angle)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	drone_angle->lat = mavlink_msg_drone_angle_get_lat(msg);
	drone_angle->lon = mavlink_msg_drone_angle_get_lon(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DRONE_ANGLE_LEN? msg->len : MAVLINK_MSG_ID_DRONE_ANGLE_LEN;
        memset(drone_angle, 0, MAVLINK_MSG_ID_DRONE_ANGLE_LEN);
	memcpy(drone_angle, _MAV_PAYLOAD(msg), len);
#endif
}

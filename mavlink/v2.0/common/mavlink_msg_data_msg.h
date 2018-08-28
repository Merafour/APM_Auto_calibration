// MESSAGE DATA_MSG PACKING

#define MAVLINK_MSG_ID_DATA_MSG 12

MAVPACKED(
typedef struct __mavlink_data_msg_t {
 int32_t lat; /*< Latitude (WGS84), in degrees * 1E7*/
 int32_t lon; /*< Longitude (WGS84), in degrees * 1E7*/
 int16_t alt; /*< alt vertical_angle*/
 int16_t vol_bat; /*< The new base mode*/
 uint8_t gps_num; /*< The new base mode*/
 uint8_t rc_flag; /*< The new base mode*/
}) mavlink_data_msg_t;

#define MAVLINK_MSG_ID_DATA_MSG_LEN 14
#define MAVLINK_MSG_ID_DATA_MSG_MIN_LEN 14
#define MAVLINK_MSG_ID_12_LEN 14
#define MAVLINK_MSG_ID_12_MIN_LEN 14

#define MAVLINK_MSG_ID_DATA_MSG_CRC 120
#define MAVLINK_MSG_ID_12_CRC 120



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DATA_MSG { \
	12, \
	"DATA_MSG", \
	6, \
	{  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_data_msg_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_data_msg_t, lon) }, \
         { "alt", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_data_msg_t, alt) }, \
         { "vol_bat", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_data_msg_t, vol_bat) }, \
         { "gps_num", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_data_msg_t, gps_num) }, \
         { "rc_flag", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_data_msg_t, rc_flag) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DATA_MSG { \
	"DATA_MSG", \
	6, \
	{  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_data_msg_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_data_msg_t, lon) }, \
         { "alt", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_data_msg_t, alt) }, \
         { "vol_bat", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_data_msg_t, vol_bat) }, \
         { "gps_num", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_data_msg_t, gps_num) }, \
         { "rc_flag", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_data_msg_t, rc_flag) }, \
         } \
}
#endif

/**
 * @brief Pack a data_msg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param lat Latitude (WGS84), in degrees * 1E7
 * @param lon Longitude (WGS84), in degrees * 1E7
 * @param alt alt vertical_angle
 * @param vol_bat The new base mode
 * @param gps_num The new base mode
 * @param rc_flag The new base mode
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_data_msg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       int32_t lat, int32_t lon, int16_t alt, int16_t vol_bat, uint8_t gps_num, uint8_t rc_flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DATA_MSG_LEN];
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);
	_mav_put_int16_t(buf, 8, alt);
	_mav_put_int16_t(buf, 10, vol_bat);
	_mav_put_uint8_t(buf, 12, gps_num);
	_mav_put_uint8_t(buf, 13, rc_flag);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DATA_MSG_LEN);
#else
	mavlink_data_msg_t packet;
	packet.lat = lat;
	packet.lon = lon;
	packet.alt = alt;
	packet.vol_bat = vol_bat;
	packet.gps_num = gps_num;
	packet.rc_flag = rc_flag;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DATA_MSG_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_DATA_MSG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_MSG_LEN, MAVLINK_MSG_ID_DATA_MSG_CRC);
}

/**
 * @brief Pack a data_msg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param lat Latitude (WGS84), in degrees * 1E7
 * @param lon Longitude (WGS84), in degrees * 1E7
 * @param alt alt vertical_angle
 * @param vol_bat The new base mode
 * @param gps_num The new base mode
 * @param rc_flag The new base mode
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_data_msg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           int32_t lat,int32_t lon,int16_t alt,int16_t vol_bat,uint8_t gps_num,uint8_t rc_flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DATA_MSG_LEN];
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);
	_mav_put_int16_t(buf, 8, alt);
	_mav_put_int16_t(buf, 10, vol_bat);
	_mav_put_uint8_t(buf, 12, gps_num);
	_mav_put_uint8_t(buf, 13, rc_flag);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DATA_MSG_LEN);
#else
	mavlink_data_msg_t packet;
	packet.lat = lat;
	packet.lon = lon;
	packet.alt = alt;
	packet.vol_bat = vol_bat;
	packet.gps_num = gps_num;
	packet.rc_flag = rc_flag;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DATA_MSG_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_DATA_MSG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_MSG_LEN, MAVLINK_MSG_ID_DATA_MSG_CRC);
}

/**
 * @brief Encode a data_msg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param data_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_data_msg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_data_msg_t* data_msg)
{
	return mavlink_msg_data_msg_pack(system_id, component_id, msg, data_msg->lat, data_msg->lon, data_msg->alt, data_msg->vol_bat, data_msg->gps_num, data_msg->rc_flag);
}

/**
 * @brief Encode a data_msg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param data_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_data_msg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_data_msg_t* data_msg)
{
	return mavlink_msg_data_msg_pack_chan(system_id, component_id, chan, msg, data_msg->lat, data_msg->lon, data_msg->alt, data_msg->vol_bat, data_msg->gps_num, data_msg->rc_flag);
}

/**
 * @brief Send a data_msg message
 * @param chan MAVLink channel to send the message
 *
 * @param lat Latitude (WGS84), in degrees * 1E7
 * @param lon Longitude (WGS84), in degrees * 1E7
 * @param alt alt vertical_angle
 * @param vol_bat The new base mode
 * @param gps_num The new base mode
 * @param rc_flag The new base mode
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_data_msg_send(mavlink_channel_t chan, int32_t lat, int32_t lon, int16_t alt, int16_t vol_bat, uint8_t gps_num, uint8_t rc_flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DATA_MSG_LEN];
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);
	_mav_put_int16_t(buf, 8, alt);
	_mav_put_int16_t(buf, 10, vol_bat);
	_mav_put_uint8_t(buf, 12, gps_num);
	_mav_put_uint8_t(buf, 13, rc_flag);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_MSG, buf, MAVLINK_MSG_ID_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_MSG_LEN, MAVLINK_MSG_ID_DATA_MSG_CRC);
#else
	mavlink_data_msg_t packet;
	packet.lat = lat;
	packet.lon = lon;
	packet.alt = alt;
	packet.vol_bat = vol_bat;
	packet.gps_num = gps_num;
	packet.rc_flag = rc_flag;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_MSG, (const char *)&packet, MAVLINK_MSG_ID_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_MSG_LEN, MAVLINK_MSG_ID_DATA_MSG_CRC);
#endif
}

/**
 * @brief Send a data_msg message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_data_msg_send_struct(mavlink_channel_t chan, const mavlink_data_msg_t* data_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_data_msg_send(chan, data_msg->lat, data_msg->lon, data_msg->alt, data_msg->vol_bat, data_msg->gps_num, data_msg->rc_flag);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_MSG, (const char *)data_msg, MAVLINK_MSG_ID_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_MSG_LEN, MAVLINK_MSG_ID_DATA_MSG_CRC);
#endif
}

#if MAVLINK_MSG_ID_DATA_MSG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_data_msg_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int32_t lat, int32_t lon, int16_t alt, int16_t vol_bat, uint8_t gps_num, uint8_t rc_flag)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_int32_t(buf, 0, lat);
	_mav_put_int32_t(buf, 4, lon);
	_mav_put_int16_t(buf, 8, alt);
	_mav_put_int16_t(buf, 10, vol_bat);
	_mav_put_uint8_t(buf, 12, gps_num);
	_mav_put_uint8_t(buf, 13, rc_flag);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_MSG, buf, MAVLINK_MSG_ID_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_MSG_LEN, MAVLINK_MSG_ID_DATA_MSG_CRC);
#else
	mavlink_data_msg_t *packet = (mavlink_data_msg_t *)msgbuf;
	packet->lat = lat;
	packet->lon = lon;
	packet->alt = alt;
	packet->vol_bat = vol_bat;
	packet->gps_num = gps_num;
	packet->rc_flag = rc_flag;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_MSG, (const char *)packet, MAVLINK_MSG_ID_DATA_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_MSG_LEN, MAVLINK_MSG_ID_DATA_MSG_CRC);
#endif
}
#endif

#endif

// MESSAGE DATA_MSG UNPACKING


/**
 * @brief Get field lat from data_msg message
 *
 * @return Latitude (WGS84), in degrees * 1E7
 */
static inline int32_t mavlink_msg_data_msg_get_lat(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field lon from data_msg message
 *
 * @return Longitude (WGS84), in degrees * 1E7
 */
static inline int32_t mavlink_msg_data_msg_get_lon(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field alt from data_msg message
 *
 * @return alt vertical_angle
 */
static inline int16_t mavlink_msg_data_msg_get_alt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field vol_bat from data_msg message
 *
 * @return The new base mode
 */
static inline int16_t mavlink_msg_data_msg_get_vol_bat(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field gps_num from data_msg message
 *
 * @return The new base mode
 */
static inline uint8_t mavlink_msg_data_msg_get_gps_num(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field rc_flag from data_msg message
 *
 * @return The new base mode
 */
static inline uint8_t mavlink_msg_data_msg_get_rc_flag(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Decode a data_msg message into a struct
 *
 * @param msg The message to decode
 * @param data_msg C-struct to decode the message contents into
 */
static inline void mavlink_msg_data_msg_decode(const mavlink_message_t* msg, mavlink_data_msg_t* data_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	data_msg->lat = mavlink_msg_data_msg_get_lat(msg);
	data_msg->lon = mavlink_msg_data_msg_get_lon(msg);
	data_msg->alt = mavlink_msg_data_msg_get_alt(msg);
	data_msg->vol_bat = mavlink_msg_data_msg_get_vol_bat(msg);
	data_msg->gps_num = mavlink_msg_data_msg_get_gps_num(msg);
	data_msg->rc_flag = mavlink_msg_data_msg_get_rc_flag(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DATA_MSG_LEN? msg->len : MAVLINK_MSG_ID_DATA_MSG_LEN;
        memset(data_msg, 0, MAVLINK_MSG_ID_DATA_MSG_LEN);
	memcpy(data_msg, _MAV_PAYLOAD(msg), len);
#endif
}

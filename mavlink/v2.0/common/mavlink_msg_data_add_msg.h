// MESSAGE DATA_ADD_MSG PACKING

#define MAVLINK_MSG_ID_DATA_ADD_MSG 72

MAVPACKED(
typedef struct __mavlink_data_add_msg_t {
 int16_t gps_vertical_vel; /*< The new base mode*/
 uint16_t head_angel; /*< The new base mode*/
 int8_t gps_level_vel; /*< The new base mode*/
 uint8_t gps_hdop; /*< The new base mode*/
 uint8_t rssi; /*< The new base mode*/
}) mavlink_data_add_msg_t;

#define MAVLINK_MSG_ID_DATA_ADD_MSG_LEN 7
#define MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN 7
#define MAVLINK_MSG_ID_72_LEN 7
#define MAVLINK_MSG_ID_72_MIN_LEN 7

#define MAVLINK_MSG_ID_DATA_ADD_MSG_CRC 175
#define MAVLINK_MSG_ID_72_CRC 175



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DATA_ADD_MSG { \
	72, \
	"DATA_ADD_MSG", \
	5, \
	{  { "gps_vertical_vel", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_data_add_msg_t, gps_vertical_vel) }, \
         { "head_angel", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_data_add_msg_t, head_angel) }, \
         { "gps_level_vel", NULL, MAVLINK_TYPE_INT8_T, 0, 4, offsetof(mavlink_data_add_msg_t, gps_level_vel) }, \
         { "gps_hdop", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_data_add_msg_t, gps_hdop) }, \
         { "rssi", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_data_add_msg_t, rssi) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DATA_ADD_MSG { \
	"DATA_ADD_MSG", \
	5, \
	{  { "gps_vertical_vel", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_data_add_msg_t, gps_vertical_vel) }, \
         { "head_angel", NULL, MAVLINK_TYPE_UINT16_T, 0, 2, offsetof(mavlink_data_add_msg_t, head_angel) }, \
         { "gps_level_vel", NULL, MAVLINK_TYPE_INT8_T, 0, 4, offsetof(mavlink_data_add_msg_t, gps_level_vel) }, \
         { "gps_hdop", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_data_add_msg_t, gps_hdop) }, \
         { "rssi", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_data_add_msg_t, rssi) }, \
         } \
}
#endif

/**
 * @brief Pack a data_add_msg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param gps_vertical_vel The new base mode
 * @param gps_level_vel The new base mode
 * @param gps_hdop The new base mode
 * @param head_angel The new base mode
 * @param rssi The new base mode
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_data_add_msg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       int16_t gps_vertical_vel, int8_t gps_level_vel, uint8_t gps_hdop, uint16_t head_angel, uint8_t rssi)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DATA_ADD_MSG_LEN];
	_mav_put_int16_t(buf, 0, gps_vertical_vel);
	_mav_put_uint16_t(buf, 2, head_angel);
	_mav_put_int8_t(buf, 4, gps_level_vel);
	_mav_put_uint8_t(buf, 5, gps_hdop);
	_mav_put_uint8_t(buf, 6, rssi);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN);
#else
	mavlink_data_add_msg_t packet;
	packet.gps_vertical_vel = gps_vertical_vel;
	packet.head_angel = head_angel;
	packet.gps_level_vel = gps_level_vel;
	packet.gps_hdop = gps_hdop;
	packet.rssi = rssi;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_DATA_ADD_MSG;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_CRC);
}

/**
 * @brief Pack a data_add_msg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param gps_vertical_vel The new base mode
 * @param gps_level_vel The new base mode
 * @param gps_hdop The new base mode
 * @param head_angel The new base mode
 * @param rssi The new base mode
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_data_add_msg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           int16_t gps_vertical_vel,int8_t gps_level_vel,uint8_t gps_hdop,uint16_t head_angel,uint8_t rssi)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DATA_ADD_MSG_LEN];
	_mav_put_int16_t(buf, 0, gps_vertical_vel);
	_mav_put_uint16_t(buf, 2, head_angel);
	_mav_put_int8_t(buf, 4, gps_level_vel);
	_mav_put_uint8_t(buf, 5, gps_hdop);
	_mav_put_uint8_t(buf, 6, rssi);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN);
#else
	mavlink_data_add_msg_t packet;
	packet.gps_vertical_vel = gps_vertical_vel;
	packet.head_angel = head_angel;
	packet.gps_level_vel = gps_level_vel;
	packet.gps_hdop = gps_hdop;
	packet.rssi = rssi;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_DATA_ADD_MSG;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_CRC);
}

/**
 * @brief Encode a data_add_msg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param data_add_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_data_add_msg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_data_add_msg_t* data_add_msg)
{
	return mavlink_msg_data_add_msg_pack(system_id, component_id, msg, data_add_msg->gps_vertical_vel, data_add_msg->gps_level_vel, data_add_msg->gps_hdop, data_add_msg->head_angel, data_add_msg->rssi);
}

/**
 * @brief Encode a data_add_msg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param data_add_msg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_data_add_msg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_data_add_msg_t* data_add_msg)
{
	return mavlink_msg_data_add_msg_pack_chan(system_id, component_id, chan, msg, data_add_msg->gps_vertical_vel, data_add_msg->gps_level_vel, data_add_msg->gps_hdop, data_add_msg->head_angel, data_add_msg->rssi);
}

/**
 * @brief Send a data_add_msg message
 * @param chan MAVLink channel to send the message
 *
 * @param gps_vertical_vel The new base mode
 * @param gps_level_vel The new base mode
 * @param gps_hdop The new base mode
 * @param head_angel The new base mode
 * @param rssi The new base mode
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_data_add_msg_send(mavlink_channel_t chan, int16_t gps_vertical_vel, int8_t gps_level_vel, uint8_t gps_hdop, uint16_t head_angel, uint8_t rssi)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_DATA_ADD_MSG_LEN];
	_mav_put_int16_t(buf, 0, gps_vertical_vel);
	_mav_put_uint16_t(buf, 2, head_angel);
	_mav_put_int8_t(buf, 4, gps_level_vel);
	_mav_put_uint8_t(buf, 5, gps_hdop);
	_mav_put_uint8_t(buf, 6, rssi);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_ADD_MSG, buf, MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_CRC);
#else
	mavlink_data_add_msg_t packet;
	packet.gps_vertical_vel = gps_vertical_vel;
	packet.head_angel = head_angel;
	packet.gps_level_vel = gps_level_vel;
	packet.gps_hdop = gps_hdop;
	packet.rssi = rssi;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_ADD_MSG, (const char *)&packet, MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_CRC);
#endif
}

/**
 * @brief Send a data_add_msg message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_data_add_msg_send_struct(mavlink_channel_t chan, const mavlink_data_add_msg_t* data_add_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_data_add_msg_send(chan, data_add_msg->gps_vertical_vel, data_add_msg->gps_level_vel, data_add_msg->gps_hdop, data_add_msg->head_angel, data_add_msg->rssi);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_ADD_MSG, (const char *)data_add_msg, MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_CRC);
#endif
}

#if MAVLINK_MSG_ID_DATA_ADD_MSG_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_data_add_msg_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int16_t gps_vertical_vel, int8_t gps_level_vel, uint8_t gps_hdop, uint16_t head_angel, uint8_t rssi)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char *buf = (char *)msgbuf;
	_mav_put_int16_t(buf, 0, gps_vertical_vel);
	_mav_put_uint16_t(buf, 2, head_angel);
	_mav_put_int8_t(buf, 4, gps_level_vel);
	_mav_put_uint8_t(buf, 5, gps_hdop);
	_mav_put_uint8_t(buf, 6, rssi);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_ADD_MSG, buf, MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_CRC);
#else
	mavlink_data_add_msg_t *packet = (mavlink_data_add_msg_t *)msgbuf;
	packet->gps_vertical_vel = gps_vertical_vel;
	packet->head_angel = head_angel;
	packet->gps_level_vel = gps_level_vel;
	packet->gps_hdop = gps_hdop;
	packet->rssi = rssi;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DATA_ADD_MSG, (const char *)packet, MAVLINK_MSG_ID_DATA_ADD_MSG_MIN_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN, MAVLINK_MSG_ID_DATA_ADD_MSG_CRC);
#endif
}
#endif

#endif

// MESSAGE DATA_ADD_MSG UNPACKING


/**
 * @brief Get field gps_vertical_vel from data_add_msg message
 *
 * @return The new base mode
 */
static inline int16_t mavlink_msg_data_add_msg_get_gps_vertical_vel(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int16_t(msg,  0);
}

/**
 * @brief Get field gps_level_vel from data_add_msg message
 *
 * @return The new base mode
 */
static inline int8_t mavlink_msg_data_add_msg_get_gps_level_vel(const mavlink_message_t* msg)
{
	return _MAV_RETURN_int8_t(msg,  4);
}

/**
 * @brief Get field gps_hdop from data_add_msg message
 *
 * @return The new base mode
 */
static inline uint8_t mavlink_msg_data_add_msg_get_gps_hdop(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field head_angel from data_add_msg message
 *
 * @return The new base mode
 */
static inline uint16_t mavlink_msg_data_add_msg_get_head_angel(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint16_t(msg,  2);
}

/**
 * @brief Get field rssi from data_add_msg message
 *
 * @return The new base mode
 */
static inline uint8_t mavlink_msg_data_add_msg_get_rssi(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Decode a data_add_msg message into a struct
 *
 * @param msg The message to decode
 * @param data_add_msg C-struct to decode the message contents into
 */
static inline void mavlink_msg_data_add_msg_decode(const mavlink_message_t* msg, mavlink_data_add_msg_t* data_add_msg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	data_add_msg->gps_vertical_vel = mavlink_msg_data_add_msg_get_gps_vertical_vel(msg);
	data_add_msg->head_angel = mavlink_msg_data_add_msg_get_head_angel(msg);
	data_add_msg->gps_level_vel = mavlink_msg_data_add_msg_get_gps_level_vel(msg);
	data_add_msg->gps_hdop = mavlink_msg_data_add_msg_get_gps_hdop(msg);
	data_add_msg->rssi = mavlink_msg_data_add_msg_get_rssi(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DATA_ADD_MSG_LEN? msg->len : MAVLINK_MSG_ID_DATA_ADD_MSG_LEN;
        memset(data_add_msg, 0, MAVLINK_MSG_ID_DATA_ADD_MSG_LEN);
	memcpy(data_add_msg, _MAV_PAYLOAD(msg), len);
#endif
}

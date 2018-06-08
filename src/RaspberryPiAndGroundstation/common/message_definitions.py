"""
This file contains an array with all the messages we may need
"""
from struct import unpack, pack

format_character_byte_lengths = {
    "l": 4,
    "h": 2,
    "f": 4
}

messages = [
    {
        "name": "heartbeat",
        "type": 0,  # also known as type
        "binary_format_string": "h",
        "description": "heartbeat"
    },
    {
        "name": "telemetry",
        "type": 1,
        "binary_format_string": "hh",
        "description": "telemetry: battery_voltage, battery_temperature, ..."
    },
    {
        "name": "emergency_stop",
        "type": 2,
        "binary_format_string": "h",
        "description": "emergency_stop"
    },
    {
        "name": "set_motor_speed",
        "type": 3,
        "binary_format_string": "hf",
        "description": "set_motor_speed: motor_index, motor_speed"
    },
    {
        "name": "set_motor_steering",
        "type": 4,
        "binary_format_string": "hf",
        "description": "set_motor_steering: motor_index, motor_speed"
    }
]


def get_by_name(message_name):
    for a in messages:
        if (a["name"] == message_name):
            return a
    raise "{} not found in message definition list".format(message_name)


def get_type_from_name(message_name):
    return get_by_name(message_name)["type"]


#
# Identifier
#


def validate_identifier(identifier):
    for a in messages:
        if (a["type"] == identifier):
            return True
    return False


def retrieve_data_identifier(raw_data):
    if (len(raw_data) > 2):
        idenfitier = unpack('h', raw_data[0: 2])[0]
        if (validate_identifier(idenfitier)):
            return idenfitier
    return None


#
# Checksum
#


def calculate_checksum(byte_message):
    length = len(str(byte_message))
    return length


def create_format_string(message_format):
    """
    # h: first byte. used for determining the type of message sent/received
    # middle part: determined by the message format
    # l: last byte. used for error checking """
    return "h" + message_format + "l"


#
# Message encoding and decoding
#


def encode_message(message_name, data):
    current_message_def = get_by_name(message_name)
    format_string = create_format_string(current_message_def["binary_format_string"])
    checksum = calculate_checksum(data)
    formatted_message = pack(format_string, current_message_def["type"], *data, checksum)
    return formatted_message


def decode_message(raw_data, identifier=None):
    if (identifier is None):
        _identifier = retrieve_data_identifier(raw_data)
    else:
        _identifier = identifier
    message_definition_format = messages[_identifier]["binary_format_string"]
    return unpack('h' + message_definition_format + 'l', raw_data)

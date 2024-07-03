#####################################
# This is the main service that will be used to serve BLE requests
# For now it is the only one since the library used is bugged and cannot handle more than one service at a time
# As a temporary solution, we have a lot of characteristics that should ideally be placed in separate repositories
#####################################
import os
from bless import (
    GATTCharacteristicProperties,
    GATTAttributePermissions,
)
from gatt.service_dispatcher import AsyncInitMixin
import logging
import core.utils as utils
logger = logging.getLogger(name=__name__)


def write_wifi_credentials(value):
    logger.info(f"Received value: {value}")

    # connect to the wifi network
    pass


def get_local_ip():
    # Get the local ip address of the device
    ip = utils.get_local_ip()
    logger.info(f"Local ip: {ip}")
    return ip.encode()


class MatrixControl(AsyncInitMixin):
    server = None
    service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    characteristics = {

        # Discovery
        "9d0e35da-bc0f-473e-a32c-25d33eaae17a": {
            "name": "discovery",
            "properties":
                GATTCharacteristicProperties.read,
            "value": None,
            "permissions": GATTAttributePermissions.readable,
            "read_action": lambda: "Hello, World!".encode()
        },

        # Wi-fi
        "9d0e35da-bc0f-473e-a32c-25d33eaae17b": {
            "name": "wifi",
            "properties": GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.writeable,
            "write_action": write_wifi_credentials
        },

        # Matrix ip
        "9d0e35da-bc0f-473e-a32c-25d33eaae17c": {
            "name": "ip",
            "properties": GATTCharacteristicProperties.read,
            "value": None,
            "permissions": GATTAttributePermissions.readable,
            "read_action": get_local_ip
        },

    }

#####################################
# This is the main service that will be used to serve BLE requests
# For now it is the only one since the library used is bugged and cannot handle more than one service at a time
# As a temporary solution, we have a lot of characteristics that should ideally be placed in separate repositories
#####################################

from bless import (
    GATTCharacteristicProperties,
    GATTAttributePermissions,
)
from gatt.service_dispatcher import AsyncInitMixin
import logging

logger = logging.getLogger(name=__name__)


def write_wifi_credentials(value):
    logger.info(f"Received value: {value}")

    # connect to the wifi network



class MatrixControl(AsyncInitMixin):
    server = None
    service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    characteristics = {

        # Discovery
        "9d0e35da-bc0f-473e-a32c-25d33eaae17a": {
            "name": "active",
            "properties":
                GATTCharacteristicProperties.read,
            "value": None,
            "permissions": GATTAttributePermissions.readable,
            "read_action": lambda: "Hello, World!".encode()
        },

        # Wi-fi
        "9d0e35da-bc0f-473e-a32c-25d33eaae17b": {
            "name": "active",
            "properties": GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.writeable,
            "write_action": write_wifi_credentials
        },

    }

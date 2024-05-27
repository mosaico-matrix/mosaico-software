import sys
import logging
import asyncio
import threading
import socket

from typing import Any, Dict, Union

from bless import ( 
    BlessServer,
    BlessGATTCharacteristic,
    GATTCharacteristicProperties,
    GATTAttributePermissions,
)

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(name=__name__)

trigger: Union[asyncio.Event, threading.Event]
if sys.platform in ["darwin", "win32"]:
    trigger = threading.Event()
else:
    trigger = asyncio.Event()


def send_data_to_cpp(data):
    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('localhost', 10000)
    try:
        sock.connect(server_address)
        sock.sendall(data)
    finally:
        # Clean up
        sock.close()

def read_request(characteristic: BlessGATTCharacteristic, **kwargs) -> bytearray:
    logger.debug(f"Reading {characteristic.value}")
    return characteristic.value


def write_request(characteristic: BlessGATTCharacteristic, value: Any, **kwargs):
    characteristic.value = value
    logger.debug(f"Char value set to {characteristic.value}")
    send_data_to_cpp(value)
    if characteristic.value == b"\x0f":
        logger.debug("Nice")
        trigger.set()


async def run(loop):

    trigger.clear()

    # Instantiate the server
    gatt: Dict = {
        "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2": {
            "9d0e35da-bc0f-473e-a32c-25d33eaae17a": {
                "Properties": (
                        GATTCharacteristicProperties.read
                        | GATTCharacteristicProperties.write
                        | GATTCharacteristicProperties.indicate
                ),
                "Permissions": (
                        GATTAttributePermissions.readable
                        | GATTAttributePermissions.writeable
                ),
                "Value": None,
            }
        },
        # "5c339364-c7be-4f23-b666-a8ff73a6a86a": {
        #     "bfc0c92f-317d-4ba9-976b-cc11ce77b4ca": {
        #         "Properties": GATTCharacteristicProperties.read,
        #         "Permissions": GATTAttributePermissions.readable,
        #         "Value": bytearray(b"\x69"),
        #     }
        # },
    }

    my_service_name = "PixelForge"
    server = BlessServer(name=my_service_name, loop=loop)
    server.read_request_func = read_request
    server.write_request_func = write_request

    await server.add_gatt(gatt)
    await server.start()
    #logger.debug(server.get_characteristic("51FF12BB-3ED8-46E5-B4F9-D64E2FEC021B"))
    logger.debug("Advertising")
    logger.info(
        "Write '0xF' to the advertised characteristic: "
        + "51FF12BB-3ED8-46E5-B4F9-D64E2FEC021B"
    )
    if trigger.__module__ == "threading":
        trigger.wait()
    else:
        await trigger.wait()
    await asyncio.sleep(2)
    logger.debug("Updating")
    # server.get_characteristic("51FF12BB-3ED8-46E5-B4F9-D64E2FEC021B").value = bytearray(
    #     b"i"
    # )
    # server.update_value(
    #     "A07498CA-AD5B-474E-940D-16F1FBE7E8CD", "51FF12BB-3ED8-46E5-B4F9-D64E2FEC021B"
    # )
    await asyncio.sleep(5)
    await server.stop()


loop = asyncio.get_event_loop()
loop.run_until_complete(run(loop))
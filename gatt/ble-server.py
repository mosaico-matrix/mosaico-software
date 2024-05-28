import sys
import logging
import asyncio
import threading
import socket
from services.service_dispatcher import ServiceDispatcher
from services.runner_service import RunnerService

from typing import Any, Dict, Union

from bless import ( 
    BlessServer,
    BlessGATTCharacteristic,
    GATTCharacteristicProperties,
    GATTAttributePermissions,
)

logging.basicConfig(level=logging.CRITICAL)
logger = logging.getLogger(name=__name__)

trigger: Union[asyncio.Event, threading.Event]
if sys.platform in ["darwin", "win32"]:
    trigger = threading.Event()
else:
    trigger = asyncio.Event()


# Dispatch services
services: Dict = {
    "runner": {
        "uuid": "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2",
        "class": RunnerService(),
    },
}
service_dispatcher = ServiceDispatcher()
service_dispatcher.register_services(services)


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
    logger.debug(f"Reading {characteristic}")
    return characteristic.value


def write_request(characteristic: BlessGATTCharacteristic, value: Any, **kwargs):
    service_dispatcher.dispatch_write(characteristic.service_uuid, characteristic.uuid, value)
    return
    logger.debug(characteristic.uuid)
    characteristic.value = value
    logger.debug(f"Char value set to {characteristic.value}")
    #send_data_to_cpp(value)


async def run(loop):

    trigger.clear()

    # Instantiate the server
    gatt: Dict = {
        services["runner"]["uuid"]: {
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

    # Configure the server
    my_service_name = "PixelForge"
    server = BlessServer(name=my_service_name, loop=loop)
    server.read_request_func = read_request
    server.write_request_func = write_request
    await server.add_gatt(gatt)

    # Start server
    await server.start()
    logger.debug("Advertising")
    if trigger.__module__ == "threading":
        trigger.wait()
    else:
        await trigger.wait()
    await asyncio.sleep(5)
    await server.stop()


loop = asyncio.get_event_loop()
loop.run_until_complete(run(loop))
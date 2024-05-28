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
service_dispatcher = ServiceDispatcher()

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
    
    
    # Configure server
    server = BlessServer(name="PixelForge", loop=loop)
    server.read_request_func = read_request
    server.write_request_func = write_request


    # Dispatch services
    runner_service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    services: Dict = {
        "runner": {
            "uuid": runner_service_uuid,
            "class": await RunnerService.create(server, runner_service_uuid),
        },
    }
    service_dispatcher.register_services(services)
        
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

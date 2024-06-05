import sys
import logging
import asyncio
import threading


from services.matrix_service import MatrixService
from services.service_dispatcher import ServiceDispatcher
from db import init as init_db
from typing import Any, Dict, Union

from bless import (
    BlessServer,
    BlessGATTCharacteristic,
    GATTCharacteristicProperties,
    GATTAttributePermissions,
)

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(name=__name__)
service_dispatcher = ServiceDispatcher()

trigger: Union[asyncio.Event, threading.Event]
if sys.platform in ["darwin", "win32"]:
    trigger = threading.Event()
else:
    trigger = asyncio.Event()

def read_request(characteristic: BlessGATTCharacteristic, **kwargs) -> bytearray:
    return service_dispatcher.dispatch_read(characteristic.service_uuid, characteristic.uuid)


def write_request(characteristic: BlessGATTCharacteristic, value: Any, **kwargs):
    service_dispatcher.dispatch_write(characteristic.service_uuid, characteristic.uuid, value)

async def run(loop):

    trigger.clear()
    # Configure server
    server = BlessServer(name="Mosaico", loop=loop)
    server.read_request_func = read_request
    server.write_request_func = write_request

    # Enum all the possible services and initialize them
    services = [
        # We cannot initialize more than one service at a time since this lib is bugged :(
        #await RunnerService.create(server),
        await MatrixService.create(server)
    ]

    # Register services with the dispatcher in order to dispatch read and write requests to the appropriate service.
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

# Init database
init_db()

loop = asyncio.get_event_loop()
loop.run_until_complete(run(loop))

import sys
import logging
import asyncio
import threading
from aiocoap import Context, Message, resource, GET
from typing import Any, Union
from data.db import init as init_db

from bless import (
    BlessServer,
    BlessGATTCharacteristic,
)
from gatt.services.matrix_service import MatrixService
from gatt.services.service_dispatcher import ServiceDispatcher
from coap.services.widget_service import InstalledWidgetsService

# Logger
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(name=__name__)

# Service dispatcher
service_dispatcher = ServiceDispatcher()

# Trigger
trigger: Union[asyncio.Event, threading.Event]
if sys.platform in ["darwin", "win32"]:
    trigger = threading.Event()
else:
    trigger = asyncio.Event()


# GATT server callbacks
def read_request(characteristic: BlessGATTCharacteristic, **kwargs) -> bytearray:
    return service_dispatcher.dispatch_read(characteristic.service_uuid, characteristic.uuid)


def write_request(characteristic: BlessGATTCharacteristic, value: Any, **kwargs):
    service_dispatcher.dispatch_write(characteristic.service_uuid, characteristic.uuid, value)


async def run(loop):
    trigger.clear()

    # Configure GATT server
    gatt_server = BlessServer(name="Mosaico", loop=loop)
    gatt_server.read_request_func = read_request
    gatt_server.write_request_func = write_request
    await MatrixService.create(gatt_server)

    # Create CoAP context and add resources
    root = resource.Site()
    root.add_resource(['.well-known', 'core'],
                      resource.WKCResource(root.get_resources_as_linkheader))
    root.add_resource(['widgets'], InstalledWidgetsService())

    # Start CoAP server
    coap_context = await Context.create_server_context(root)

    # Start GATT server
    await gatt_server.start()
    logger.debug("Advertising")

    # Wait for trigger
    if trigger.__module__ == "threading":
        trigger.wait()
    else:
        await trigger.wait()
    await asyncio.sleep(5)
    await gatt_server.stop()
    await coap_context.shutdown()


# Init database
init_db()

loop = asyncio.get_event_loop()
loop.run_until_complete(run(loop))
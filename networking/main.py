import sys
import logging
import asyncio
import threading
from aiocoap import Context, resource
from typing import Any, Union

from coap.services.widget_configurations import WidgetConfigurations
from data.db import init as init_db
from bless import (
    BlessServer,
    BlessGATTCharacteristic,
)

from gatt.services.matrix_control import MatrixControl
from gatt.service_dispatcher import ServiceDispatcher
from coap.services.widgets import *

# Logger
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(name=__name__)

# GATT Service dispatcher
service_dispatcher = ServiceDispatcher()

# Mode of operation
mode = "default"

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
    if mode == "default":
        gatt_server = BlessServer(name="Mosaico", loop=loop)
        gatt_server.read_request_func = read_request
        gatt_server.write_request_func = write_request
        await MatrixControl.create(gatt_server)
        await gatt_server.start()
        logger.info("Advertising")

    # Create CoAP context and add resources
    root = resource.Site()
    root.add_resource(['widgets', 'installed'], InstalledWidgets())
    root.add_resource(['widgets', 'active'], ActiveWidget())
    root.add_resource(['widgets', 'configuration_form'], WidgetConfigurationForm())
    root.add_resource(['widgets', 'developed'], DevelopedWidgets())
    root.add_resource(['widget_configurations'], WidgetConfigurations())

    # Start CoAP server
    coap_context = await Context.create_server_context(root)

    # Wait for trigger
    if trigger.__module__ == "threading":
        trigger.wait()
    else:
        await trigger.wait()
    await asyncio.sleep(5)
    if mode == "default":
        await gatt_server.stop()
    await coap_context.shutdown()

# Init database
init_db()

# Get the mode of operation
if len(sys.argv) > 1:
    mode = sys.argv[1]

# Log the mode of operation
if mode == "web" or mode == "simulation":
    logger.warning("Networking module started in light mode")
    logger.warning("The GATT server will not be started")
else:
    logger.warning("The system is running in default mode")
    logger.warning("The GATT server will be started")

# Create a new event loop
loop = asyncio.new_event_loop()
asyncio.set_event_loop(loop)
loop.run_until_complete(run(loop))

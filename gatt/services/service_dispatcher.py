from bless import BlessServer
from typing import Dict
import logging


class ServiceDispatcher:
    services: Dict

    def __init__(self):
        self.services = {}

    def register_services(self, services):
        for service in services:
            self.services[service.service_uuid] = service

    def dispatch_read(self, service_uuid, characteristic_uuid):
        if service_uuid in self.services:
            characteristic = self.services[service_uuid].characteristics[characteristic_uuid]
            return characteristic["value"]
        else:
            raise ValueError(f"No service registered for UUID: {service_uuid}")

    def dispatch_write(self, service_uuid, characteristic_uuid, data):
        if service_uuid in self.services:
            characteristic = self.services[service_uuid].characteristics[characteristic_uuid]
            characteristic["write_action"](data, characteristic_uuid)
        else:
            raise ValueError(f"No service registered for UUID: {service_uuid}")


# Used as a mixin to initialize services asynchronously and register all the characteristics with the server
class AsyncInitMixin:
    logger = logging.getLogger(name=__name__)

    async def _async_init(self):
        # Register the service
        await self.server.add_new_service(self.service_uuid)
        logging.debug(f"Registered service: {self.service_uuid}")

        # Register the characteristics
        for characteristic_key, characteristic in self.characteristics.items():
            self.logger.debug(f"Registering characteristic: {characteristic_key}")
            await self.server.add_new_characteristic(
                self.service_uuid,
                characteristic_key,
                characteristic["properties"],
                characteristic["value"],
                characteristic["permissions"]
            )

    @classmethod
    async def create(cls, server: BlessServer):
        instance = cls(server)
        await instance._async_init()
        return instance

    def __init__(self, server: BlessServer):
        self.server = server

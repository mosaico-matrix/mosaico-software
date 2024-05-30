from bless import BlessServer
from typing import Dict

class ServiceDispatcher:
    services: Dict

    def __init__(self):
        self.services = {}

    def register_services(self, services):
        for service in services:
            self.services[service.service_uuid] = service

    def dispatch_read(self, uuid):
        if uuid in self.services:
            return self.services[uuid].read()
        else:
            raise ValueError(f"No service registered for UUID: {uuid}")

    def dispatch_write(self, service_uuid, characteristic_uuid, data):
        if service_uuid in self.services:
            characteristic = self.services[service_uuid].characteristics[characteristic_uuid]
            characteristic["write_action"](data, characteristic_uuid)
        else:
            raise ValueError(f"No service registered for UUID: {service_uuid}")




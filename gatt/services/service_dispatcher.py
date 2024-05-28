from typing import Dict

class ServiceDispatcher:

    def __init__(self):
        self.services = {}

    def register_services(self, gatt: Dict):
        for service_name, service in gatt.items():
            self.services[service["uuid"]] = service["class"]


    def dispatch_read(self, uuid):

        """Dispatch a read request to the appropriate service based on the UUID."""
        if uuid in self.services:
            return self.services[uuid].read()
        else:
            raise ValueError(f"No service registered for UUID: {uuid}")

    def dispatch_write(self, service_uuid, characteristic_uuid, data):

        """Dispatch a write request to the appropriate service based on the UUID."""
        if service_uuid in self.services:
            self.services[service_uuid].write(data, characteristic_uuid)
        else:
            raise ValueError(f"No service registered for UUID: {service_uuid}")
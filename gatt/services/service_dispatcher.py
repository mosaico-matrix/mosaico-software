from typing import Dict
from bless import BlessServer

class ServiceDispatcher:
    
    # This class is responsible for dispatching read and write requests to the appropriate service based on the UUID.
    # We mantain a dictionary of services, where the key is the UUID of the service and the value is the service object.
    # Whenever a read or write request is received, we check if the UUID is in the dictionary and dispatch the request to the appropriate service.
    services: Dict

    def __init__(self):
        self.services = {}

    # We receive an array of services and register them in the services dictionary
    def register_services(self, services):
        for service in services:
            self.services[service.service_uuid] = service


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


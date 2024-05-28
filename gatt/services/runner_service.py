from bless import (
    GATTCharacteristicProperties,
    BlessServer,
    GATTAttributePermissions,
)
#from service_dispatcher import AsyncInitMixin


# Used as a mixin to initialize services asynchronously and register all the characteristics with the server
class AsyncInitMixin:
    async def _async_init(self):
        # Register the service
        await self.server.add_new_service(self.service_uuid)

        # Register the characteristics
        for characteristic in self.characteristics:
            await self.server.add_new_characteristic(
                self.service_uuid,
                self.characteristics[characteristic]["uuid"],
                self.characteristics[characteristic]["properties"],
                self.characteristics[characteristic]["value"],
                self.characteristics[characteristic]["permissions"]
            )

    @classmethod
    async def create(cls, server: BlessServer):
        instance = cls(server)
        await instance._async_init()
        return instance

    def __init__(self, server: BlessServer):
        self.server = server

class RunnerService(AsyncInitMixin):
    server = None
    service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    characteristics = {
        "active": {
            "uuid": "9d0e35da-bc0f-473e-a32c-25d33eaae17a",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable

        }
    }

    def write(self, data, characteristic_uuid):
        print(f"RunnerService received data: {data} to characteristic: {characteristic_uuid}")

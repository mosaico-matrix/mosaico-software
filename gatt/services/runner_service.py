from bless import (
    GATTCharacteristicProperties,
    BlessServer,
    GATTAttributePermissions,
)



# Used as a mixin to initialize services asynchronously and register all the characteristics with the server
class AsyncInitMixin:
    async def _async_init(self):
        # Register the service
        await self.server.add_new_service(self.service_uuid)

        # Register the characteristics
        for characteristic_key, characteristic in self.characteristics.items():
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

def write_active_service(data, characteristic_uuid):
    print(f"RunnerService received data: {data} to characteristic: {characteristic_uuid}")

class RunnerService(AsyncInitMixin):
    server = None
    service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    characteristics = {
        "9d0e35da-bc0f-473e-a32c-25d33eaae17a": {
            "name": "active",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
            "write_action": write_active_service,
        }
    }

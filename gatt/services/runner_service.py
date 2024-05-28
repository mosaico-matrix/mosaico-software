from bless import ( 
    GATTCharacteristicProperties,
    BlessServer,
    GATTAttributePermissions,
)


class RunnerService:

    server = None
    service_uuid = None

    
    # gatt: Dict = {
    #     "kd0e3zda-bc0f-475e-a32c-25d23eaae17a": {
    #         "9d0e35da-bc0f-473e-a32c-25d33eaae17a": {
    #             "Properties": (
    #                     GATTCharacteristicProperties.read
    #                     | GATTCharacteristicProperties.write
    #                     | GATTCharacteristicProperties.indicate
    #             ),
    #             "Permissions": ( 
    #                     GATTAttributePermissions.readable
    #                     | GATTAttributePermissions.writeable
    #             ),
    #             "Value": None,
    #         }
    #     },
    #     # "5c339364-c7be-4f23-b666-a8ff73a6a86a": {
    #     #     "bfc0c92f-317d-4ba9-976b-cc11ce77b4ca": {
    #     #         "Properties": GATTCharacteristicProperties.read,
    #     #         "Permissions": GATTAttributePermissions.readable,
    #     #         "Value": bytearray(b"\x69"),
    #     #     }
    #     # },
    # }
        
    
    characteristics = {
        "active": "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    }
    
    
    async def _async_init(self):
            
        # Register the service
        await self.server.add_new_service(self.service_uuid)
        
        # Register the characteristics
        await self.server.add_new_characteristic(
            self.service_uuid,
            self.characteristics["active"],
            GATTCharacteristicProperties.read | GATTCharacteristicProperties.write,
            None,
            GATTAttributePermissions.readable | GATTAttributePermissions.writeable
        )

    def __init__(self, server: BlessServer, service_uuid: str):
        self.server = server
        self.service_uuid = service_uuid

    @classmethod
    async def create(cls, server: BlessServer, service_uuid: str):
        instance = cls(server, service_uuid)
        await instance._async_init()
        return instance


    def write(self, data, characteristic_uuid):
        print(f"RunnerService received data: {data} to characteristic: {characteristic_uuid}")
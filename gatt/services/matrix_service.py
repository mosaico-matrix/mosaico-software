#####################################
# This is the main service that will be used to serve BLE requests
# For now it is the only one since the library used is bugged and cannot handle more than one service at a time
# As a temporary solution, we have a lot of characteristics that should ideally be placed in separate services
#####################################

from bless import (
    GATTCharacteristicProperties,
    BlessServer,
    GATTAttributePermissions,
)

from .service_dispatcher import AsyncInitMixin

def write_active_service(data, characteristic_uuid):
    print(f"MatrixService received data: {data} to characteristic: {characteristic_uuid}")


class MatrixService(AsyncInitMixin):
    server = None
    service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    characteristics = {


        # Runners
        "9d0e35da-bc0f-473e-a32c-25d33eaae17a": {
            "name": "active",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
            "write_action": write_active_service,
        },
        "9d0e35da-bc0f-473e-a32c-25d33eaae17b": {
            "name": "active",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
            "write_action": write_active_service,
        },


        # Matrix
        "": {
        }

    }

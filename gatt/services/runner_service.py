from bless import (
    GATTCharacteristicProperties,
    BlessServer,
    GATTAttributePermissions,
)

from .service_dispatcher import AsyncInitMixin


def write_active_service(data, characteristic_uuid):
    print(f"RunnerService received data: {data} to characteristic: {characteristic_uuid}")


def get_installed_runners():
    return {
        "runner1": {
            "name": "runner1",
            "uuid": "runner1-uuid",
            "status": "idle",
            "last_run": "2021-07-01 12:00:00",
        },
        "runner2": {
            "name": "runner2",
            "uuid": "runner2-uuid",
            "status": "idle",
            "last_run": "2021-07-01 12:00:00",
        },
    }


class RunnerService(AsyncInitMixin):
    server = None
    service_uuid = "17ac3acb-b759-48c8-b5a1-5b180682bc72"
    characteristics = {
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
        "9d0e35da-bc0f-473e-a32c-25d33eaae17c": {
            "name": "active",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
            "write_action": write_active_service,
        },
        "9d0e35da-bc0f-473e-a32c-25d33eaae17d": {
            "name": "active",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
            "write_action": write_active_service,
        },


    }

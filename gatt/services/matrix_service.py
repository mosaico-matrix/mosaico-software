#####################################
# This is the main service that will be used to serve BLE requests
# For now it is the only one since the library used is bugged and cannot handle more than one service at a time
# As a temporary solution, we have a lot of characteristics that should ideally be placed in separate services
#####################################

from bless import (
    GATTCharacteristicProperties,
    GATTAttributePermissions,
)
from .service_dispatcher import AsyncInitMixin
import logging
from .interop import call_matrix

logger = logging.getLogger(name=__name__)


def preview_widget(data):
    """
    This function is called when the active widget is written to.
    It will call the matrix to preview the widget with provided <widget_id,config_id>
    """
    response = call_matrix("CMD1", data)
    logger.debug(f"active service write: {data} and response: {response}")


def get_active_widget():
    logger.debug(f"active service read")
    return b"active service read"


def install_widget(data):
    """
    This function is called when user wants to install a new widget from the app store.
    Simply an <id> is passed so that we can download the widget from the app store and update local files and db
    """
    widget_id = data['widget_id']


def get_installed_widgets():
    logger.debug(f"installed service read")
    return b"installed service read"


class MatrixService(AsyncInitMixin):
    server = None
    service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    characteristics = {


        # Widgets
        "9d0e35da-bc0f-473e-a32c-25d33eaae17a": {
            "name": "active",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
            "write_action": preview_widget,
            "read_action": get_active_widget,
        },
        "9d0e35da-bc0f-473e-a32c-25d33eaae17b": {
            "name": "installed",
            "properties":
                GATTCharacteristicProperties.read
                | GATTCharacteristicProperties.write,
            "value": None,
            "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
            "write_action": preview_widget,
            "read_action": get_installed_widgets(),
        }

        # Slideshows
        # "9d0e35da-bc0f-473e-a32c-25d33eaae17b": {
        #     "name": "active",
        #     "properties":
        #         GATTCharacteristicProperties.read
        #         | GATTCharacteristicProperties.write,
        #     "value": None,
        #     "permissions": GATTAttributePermissions.readable | GATTAttributePermissions.writeable,
        #     "write_action": write_active_service,
        #     "read_action": read_active_service,
        # },


        # Matrix
        # "": {
        # }

    }

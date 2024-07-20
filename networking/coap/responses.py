import json
import logging
import aiocoap
from aiocoap import ContentFormat


logger = logging.getLogger('mosaico_networking')
def base_response(data, message, code):
    # Create a dictionary with the data and the message
    response = {
        "data": data,
        "message": message,
    }

    return aiocoap.Message(
        payload=json.dumps(response).encode("ASCII"),
        code=code,
        content_format=ContentFormat.JSON
    )


def error_response(message=""):
    logger.error("COAP ERROR: {}".format(message))
    return base_response(None, message, aiocoap.BAD_REQUEST)


def success_response(data, message=""):
    logger.debug("COAP SUCCESS: {}".format(message))
    return base_response(data, message, aiocoap.CHANGED)

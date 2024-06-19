import json
import aiocoap
from aiocoap import ContentFormat


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
    return base_response(None, message, aiocoap.BAD_REQUEST)


def success_response(data, message=""):
    return base_response(data, message, aiocoap.CHANGED)

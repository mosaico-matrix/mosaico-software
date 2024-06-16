import aiocoap.resource as resource
from aiocoap.numbers.contentformat import ContentFormat
import aiocoap
import logging
import json
from data import db
from data.repositories.widgets import *
from rest.services.widgets import get_widget
from coap.responses import *
from interop import call_matrix
#
# logger = logging.getLogger(name="coap.repositories.installed_widgets")
#
#
# class ActiveWidget(resource.Resource):
#
#     async def render_get(self, request):
#         pass
#
#     async def render_post(self, request):
#         """
#         Set the active widget
#         """
#         logger.debug("Received POST request to active_widget")
#
#         # Deserialize the request
#         payload = json.loads(request.payload.decode())
#         widget_id = payload["widget_id"]
#         config_id = payload["config_id"]
#
#         # Set widget on matrix
#         call_matrix("LOAD_WIDGET", {"widget_path": get_widget_path(widget_id)})
#
#         return success_response(None, "Widget set successfully")

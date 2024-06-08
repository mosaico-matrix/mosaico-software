import aiocoap.resource as resource
from aiocoap.numbers.contentformat import ContentFormat
import aiocoap
import logging
import json
from data import db
from data.repositories.widgets import *
from rest.services.widgets import get_widget
from coap.responses import *

logger = logging.getLogger(name="coap.repositories.installed_widgets")


class InstalledWidgets(resource.Resource):

    async def render_get(self, request):
        logger.debug("Received GET request to installed_widgets")
        widgets = get_installed_widgets()
        return success_response(widgets)


    async def render_post(self, request):
        logger.debug("Received POST request to installed_widgets")

        # Get widget_id from request
        widget_id = json.loads(request.payload.decode())["id"]

        # Check if already installed
        widget = get_widget_by_id(widget_id)
        if widget:
            logger.warning(f"Widget with id: {widget_id} is already installed, skipping")

            # Do not return error, just log and return success
            #return error_response(None, "Widget already installed")
        else:
            # Get the widget from the app store
            logger.debug(f"Installing widget with id: {widget_id}")
            widget = get_widget(widget_id)

            # Save the widget to the db
            add_widget(widget["id"], widget["name"], widget["user"]["username"])

        return success_response(None, "Widget installed successfully")

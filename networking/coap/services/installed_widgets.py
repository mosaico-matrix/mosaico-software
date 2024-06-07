import aiocoap.resource as resource
from aiocoap.numbers.contentformat import ContentFormat
import aiocoap
import logging
import json
from data import db

logger = logging.getLogger(name="coap.services.installed_widgets")


class InstalledWidgets(resource.Resource):

    async def render_get(self, request):
        logger.debug("Received GET request to installed_widgets")
        return aiocoap.Message(payload="GG".encode("ASCII"))

    async def render_post(self, request):
        logger.debug("Received POST request to installed_widgets")

        # Get widget_id from request
        widget_id = json.loads(request.payload.decode())["id"]

        # Check if already installed
        widget = db.get_widget_by_id(widget_id)
        if widget:
            logger.warning(f"Widget with id: {widget_id} is already installed, skipping")
        else:
            # Get the widget from the app store
            logger.debug(f"Installing widget with id: {widget_id}")
            #widget = widget_service.get_widget(widget_id)

            # Save the widget to the db
            #db.add_widget(widget["id"], widget["name"])
            db.add_widget(widget_id, "Test Widget")

        return aiocoap.Message(payload="F".encode("ASCII"), code=aiocoap.CHANGED)

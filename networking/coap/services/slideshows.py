import json
import logging

from coap.dynamic_resource import DynamicResource
from coap.responses import success_response

logger = logging.getLogger('mosaico_networking')

class CreatedSlideshows(DynamicResource):

    async def render_post(self, request, args):
        """
        Create a new slideshow
        """
        logger.info("Received POST request to created_slideshows")

        # Deserialize the request
        payload = json.loads(request.payload.decode())
        logger.warning(payload)

        return success_response(None, "Slideshow created successfully")



    # async def render_delete(self, request, args):
    #     """
    #     Delete selected widget from local db but keep the files and configurations
    #     """
    #     logger.info("Received DELETE request to installed_widgets")
    #
    #     # Get widget_id from request
    #     widget_id = args["widget_id"]
    #
    #     logger.info(f"Deleting widget with id: {widget_id}")
    #
    #     # Get widget
    #     widget = local_widgets.get_widget(widget_id)
    #     if widget is None:
    #         return error_response("Widget not found")
    #
    #     # Get widget folder path
    #     widget_path = configs.get_widget_path(widget["author"], widget["name"])
    #
    #     # Delete widget from db
    #     local_widgets.disable_widget(widget_id)
    #
    #     # Delete widget data
    #     if os.path.exists(widget_path):
    #         os.system(f"rm -rf {widget_path}")
    #         logger.info(f"Deleted widget data at: {widget_path}")
    #     else:
    #         logger.warning(f"Widget path: {widget_path} does not exist, skipping")
    #
    #     return success_response(None, "Widget deleted successfully")
    #
    # async def render_get(self, request, args):
    #     """
    #     Get all installed widgets from local db
    #     """
    #     logger.info("Received GET request to installed_widgets")
    #     widgets = local_widgets.get_installed_widgets()
    #     return success_response(widgets)
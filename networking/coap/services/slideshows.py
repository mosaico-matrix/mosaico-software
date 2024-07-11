import json
import logging
import aiocoap.resource as resource

import core.slideshow_manager
import data.repositories.slideshows
from coap.dynamic_resource import DynamicResource
from coap.responses import success_response, error_response

logger = logging.getLogger('mosaico_networking')



class ActiveSlideshow(resource.Resource):

    async def render_get(self, request):
        pass
        # """
        # Get the active slideshow
        # """
        # logger.info("Received GET request to active_slideshow")
        #
        # return success_response({
        #     "widget": helpers.get_active_widget(),
        #     "config": helpers.get_active_configuration(),
        # })

    async def render_post(self, request):
        """
        Set the active slideshow
        """
        logger.info("Received POST request to active_slideshow")

        # Deserialize the request
        payload = json.loads(request.payload.decode())
        slideshow_id = payload["slideshow_id"]

        # Stop previous slideshow
        #await core.slideshow_manager.stop_slideshow()

        # Set the active slideshow
        await core.slideshow_manager.set_active_slideshow(slideshow_id)
        return success_response(None, "Slideshow set successfully")


    # async def render_delete(self, request):
    #     """
    #     Unset the active widget
    #     """
    #     logger.info("Received DELETE request to active_widget")
    #
    #     # Unload the widget
    #     settings.set_active_widget_id(None)
    #     settings.set_active_config_id(None)
    #     call_matrix("UNLOAD_WIDGET", {})
    #
    #     return success_response(None, "Widget stopped")



class CreatedSlideshows(DynamicResource):

    async def render_post(self, request, args):
        """
        Create a new slideshow or update an existing one
        """
        logger.info("Received POST request to created_slideshows")

        # Deserialize the request
        payload = json.loads(request.payload.decode())

        # Check if editing an existing slideshow
        slideshow_id = payload["id"]
        editing = slideshow_id is not None
        if editing:
            slideshow = data.repositories.slideshows.get_slideshow(slideshow_id)
            if slideshow is None:
                return error_response("Slideshow to edit not found")
            slideshow = data.repositories.slideshows.update_slideshow(slideshow_id, payload["name"], payload["items"])
            logger.info(f"Updated slideshow with id: {slideshow_id}")
        else:

            # Check if a slideshow with the same name already exists
            existing_slideshow = data.repositories.slideshows.get_slideshow_by_name(payload["name"])
            if existing_slideshow is not None:
                return error_response("Slideshow with the same name already exists")

            slideshow = data.repositories.slideshows.add_slideshow(payload["name"], payload["items"])
            logger.info(f"Created slideshow with id: {slideshow['id']}")

        return success_response(slideshow, "Slideshow saved!")


    async def render_get(self, request, args):
        """
        Get all created slideshows
        """
        logger.info("Received GET request to created_slideshows")
        slideshows = data.repositories.slideshows.get_slideshows()
        return success_response(slideshows)



    async def render_delete(self, request, args):
        """
        Delete a slideshow
        """
        logger.info("Received DELETE request to created_slideshows")

        # Deserialize the request
        slideshow_id = args["slideshow_id"]

        # Check if the slideshow exists
        slideshow = data.repositories.slideshows.get_slideshow(slideshow_id)

        if slideshow is None:
            return error_response("Slideshow not found")

        data.repositories.slideshows.delete_slideshow(slideshow_id)
        logger.info(f"Deleted slideshow with id: {slideshow_id}")

        return success_response(None)

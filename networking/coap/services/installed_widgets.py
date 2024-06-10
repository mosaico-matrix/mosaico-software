import os

import aiocoap.resource as resource
from aiocoap.numbers.contentformat import ContentFormat
import aiocoap
import logging
import json
from data import db
from data.repositories.widgets import *
from rest.services.widgets import get_widget
from coap.responses import *
import git
import configs

logger = logging.getLogger(name="coap.repositories.installed_widgets")


class InstalledWidgets(resource.Resource):

    async def render_get(self, request):
        """
        Get all installed widgets from local db
        """
        logger.debug("Received GET request to installed_widgets")
        widgets = get_installed_widgets()
        return success_response(widgets)

    async def render_post(self, request):

        """
        Install a widget from the app store to the local db
        """
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
            logger.info(f"Installing widget with id: {widget_id}")
            widget = get_widget(widget_id)

            # Save the widget to the db
            add_widget(widget["id"], widget["name"], widget["user"]["username"])
            widget_path = get_widget_path(widget["id"])

            # Git clone the widget from provided repository
            repo_url = widget["repository_url"]

            # Remove if path already exists
            if os.path.exists(widget_path):
                logger.warning(f"Widget path: {widget_path} already exists, removing")
                os.system(f"rm -rf {widget_path}")

            # Create widget directory
            os.makedirs(widget_path)

            # Clone the repo with depth 1
            git.Repo.clone_from(repo_url, widget_path, depth=1)
            logger.info(f"Cloned widget from: {repo_url}")

        return success_response(None, "Widget installed successfully")

import base64
import os
import aiocoap.resource as resource
import coap.dynamic_resource
import rest.services.widgets as rest_widgets_api
import data.repositories.widgets as local_widgets
from data.repositories import settings
from data.repositories.widget_configurations import get_widget_configuration
from core.interop import call_matrix
from coap.responses import *
from core import configs, utils, helpers, slideshow_manager
import git

logger = logging.getLogger('mosaico_networking')

class InstalledWidgets(coap.dynamic_resource.DynamicResource):

    async def render_delete(self, request, args):
        """
        Delete selected widget from local db but keep the files and configurations
        """
        logger.info("Received DELETE request to installed_widgets")

        # Get widget_id from request
        widget_id = args["widget_id"]

        logger.info(f"Deleting widget with id: {widget_id}")

        # Get widget
        widget = local_widgets.get_widget(widget_id)
        if widget is None:
            return error_response("Widget not found")

        # Get widget folder path
        widget_path = configs.get_widget_path(widget["author"], widget["name"])

        # Delete widget from db
        local_widgets.disable_widget(widget_id)

        # Delete widget data
        if os.path.exists(widget_path):
            os.system(f"rm -rf {widget_path}")
            logger.info(f"Deleted widget data at: {widget_path}")
        else:
            logger.warning(f"Widget path: {widget_path} does not exist, skipping")

        return success_response(None, "Widget deleted successfully")

    async def render_get(self, request, args):
        """
        Get all installed widgets from local db
        """
        logger.info("Received GET request to installed_widgets")
        widgets = local_widgets.get_installed_widgets()
        return success_response(widgets)

    async def render_post(self, request, args):

        """
        Install a widget from the app store to the local db
        """
        logger.info("Received POST request to installed_widgets")

        # Get widget_store_id from args
        widget_store_id = args["widget_store_id"]

        # Get the widget from the app store
        logger.info(f"Installing widget with id: {widget_store_id}")

        # Always catch API exceptions to inform the user
        try:
            widget = rest_widgets_api.get_widget(widget_store_id)
        except Exception as e:
            logger.error(f"Error getting widget with id: {widget_store_id}")
            return error_response(f"Could not get widget from app store")

        # Get the widget path
        widget_path = configs.get_widget_path(widget["user"]["username"], widget["name"])

        # Remove if path already exists
        if os.path.exists(widget_path):
            logger.warning(f"Widget path: {widget_path} already exists, removing")
            os.system(f"rm -rf {widget_path}")

        # Create widget directory
        os.makedirs(widget_path)

        # Git clone the widget from provided repository
        repo_url = widget["repository_url"]
        try:
            git.Repo.clone_from(repo_url, widget_path, depth=1)
            logger.info(f"Cloned widget from: {repo_url}")
        except Exception as e:
            logger.error(f"Error cloning widget from: {repo_url}")
            return error_response(f"Could not clone widget from repository")

        # Read the metadata from the widget folder
        try:
            metadata = read_widget_metadata(widget_path)
        except Exception as e:
            return error_response("Failed to read widget metadata")

        # Check if already installed
        conflict_widget = local_widgets.get_widget_by_store_id(widget_store_id)

        if conflict_widget:
            logger.warning(f"Widget with id: {widget_store_id} is already installed, enabling")

            # Re-enable the widget (this was previously uninstalled)
            local_widgets.enable_widget(conflict_widget["id"])
        else:

            # Save the widget to the db
            local_widgets.add_widget(widget["id"], widget["name"], widget["user"]["username"], metadata)

        return success_response(None)


class ActiveWidget(resource.Resource):

    async def render_get(self, request):

        """
        Get the active widget
        """
        logger.info("Received GET request to active_widget")

        return success_response({
            "widget": helpers.get_active_widget(),
            "config": helpers.get_active_configuration(),
        })

    async def render_post(self, request):
        """
        Set the active widget
        """
        logger.info("Received POST request to active_widget")

        # Deserialize the request
        payload = json.loads(request.payload.decode())
        widget_id = payload["widget_id"]
        config_id = payload["config_id"]

        # Set the active widget
        await slideshow_manager.set_active_widget(widget_id, config_id)

        return success_response(None, "Widget set successfully")


    async def render_delete(self, request):
        """
        Unset the active widget
        """
        logger.info("Received DELETE request to active_widget")

        # Unload the widget
        settings.set_active_widget_id(None)
        settings.set_active_config_id(None)
        call_matrix("UNLOAD_WIDGET", {})

        return success_response(None, "Widget stopped")


class WidgetConfigurationForm(coap.dynamic_resource.DynamicResource):
    """
    The purpose of this service is to provide the widget configuration form .json from the file system
    It is composed by only the get method which returns the json file used to build the configuration form
    """

    async def render_get(self, request, args):
        """
        Get the widget configuration form from the file system
        """

        # Get widget
        widget_id = args["widget_id"]
        widget = local_widgets.get_widget(widget_id)
        if widget is None:
            return error_response("Widget not found")

        # Get the widget installation path
        widget_path = configs.get_widget_path(widget["author"], widget["name"])

        # Get the configuration form
        form_path = f"{widget_path}/config-form.json"

        # Return the configuration form
        try:
            with open(form_path, "r") as f:
                return success_response(json.load(f))
        except Exception as e:
            return error_response("Could not get configuration form for widget")


class DevelopedWidgets(resource.Resource):
    """
    This service allows developers to create a custom widget package and upload it directly to the system
    This skips the app store and the download from the git repository
    """
    local_dev_username = "local_dev"

    async def render_post(self, request):
        """
        Upload a custom widget package in .tar.gz format which contains a folder named "widget" with the widget files
        Extract the package and save the widget to the database
        """
        logger.info("Received POST request to developed_widgets")

        # The request is of the kind widget_name, file_base64
        payload = request.payload.decode()

        # Split the payload
        payload_split = payload.split(",")
        if len(payload_split) != 2:
            return error_response("Invalid payload format")

        # Get the widget_name and file_base64
        widget_name = payload_split[0]
        file_base64 = payload_split[1]

        # Create output path
        output_path = configs.get_widget_path(self.local_dev_username, widget_name)

        # Check if widget with same name already exists for local user
        widget_conflict = local_widgets.get_widget_by_name_author(widget_name, self.local_dev_username)
        if widget_conflict:
            logger.warning(f"Widget with name: {widget_name} already exists for local user, overwriting files")

        # Remove previous files
        if os.path.exists(output_path):
            os.system(f"rm -rf {output_path}")

        # Create the directory if it does not exist
        try:
            os.makedirs(output_path)
        except Exception as e:
            return error_response("Failed to create widget directory")

        try:
            utils.extract_archive_from_base64_bytes(file_base64, output_path)
        except Exception as e:
            return error_response("Failed to save widget to the disk")

        # Read the metadata from the widget folder
        try:
            metadata = read_widget_metadata(output_path)
        except Exception as e:
            return error_response("Failed to read widget metadata")

        # Add the widget to the database
        if widget_conflict is None:
            local_widgets.add_widget(None, widget_name, self.local_dev_username, metadata)

        # Retrieve the widget from the database
        widget = local_widgets.get_widget_by_name_author(widget_name, self.local_dev_username)

        return success_response(widget, "Widget uploaded successfully")




def read_widget_metadata(widget_path):
    """
    Read the widget metadata from the widget folder as a string
    """
    metadata_path = f"{widget_path}/mosaico.json"
    with open(metadata_path, "r") as f:
        return f.read()
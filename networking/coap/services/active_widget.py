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


class ActiveWidget(resource.Resource):


    async def render_get(self, request):
        pass


    async def render_post(self, request):
        pass

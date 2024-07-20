import logging
import os
import aiocoap.resource as resource

logger = logging.getLogger(name="coap.services.matrix_control")


class MatrixControl(resource.Resource):

    async def render_get(self, request):
        pass

    async def render_post(self, request):
        pass


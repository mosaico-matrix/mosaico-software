from aiocoap import Message, Code
from aiocoap.resource import Resource, PathCapable, error
import logging

logger = logging.getLogger(name="coap.dynamic_resource")
class DynamicResource(Resource, PathCapable):
    def __init__(self):
        super().__init__()

    async def render(self, request):

        # Split the dynamic part in case it has multiple values
        return await self.handle_dynamic_request(request, request.opt.uri_path)

    async def handle_dynamic_request(self, request, args):
        # This method can be customized to handle different dynamic values
        if request.code == Code.GET:
            return await self.render_get(request, args)
        elif request.code == Code.POST:
            return await self.render_post(request, args)
        elif request.code == Code.PUT:
            return await self.render_put(request, args)
        elif request.code == Code.DELETE:
            return await self.render_delete(request, args)
        else:
            raise error.UnsupportedMethod()

    async def render_get(self, request, args):
        pass

    async def render_post(self, request, args):
        pass

    async def render_put(self, request, args):
        pass

    async def render_delete(self, request, args):
        pass
import aiocoap.resource as resource
from aiocoap.numbers.contentformat import ContentFormat
import aiocoap


class InstalledWidgetsService(resource.Resource):

    def __init__(self):
        super().__init__()
        self.set_content(b"MONO\n")

    def set_content(self, content):
        self.content = content
        while len(self.content) <= 5024:
            self.content = self.content + b"0123456789\n"

    async def render_get(self, request):
        return aiocoap.Message(payload=self.content)

    async def render_put(self, request):
        print('PUT payload: %s' % request.payload)
        self.set_content(request.payload)
        return aiocoap.Message(code=aiocoap.CHANGED, payload=self.content)
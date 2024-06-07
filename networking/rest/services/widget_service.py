from ..client import RestClient
import json


class WidgetService:
    def __init__(self, client: RestClient):
        self.client = client

    def get_widget(self, widget_id):
        response = self.client.get(f"widgets/{widget_id}")
        return response

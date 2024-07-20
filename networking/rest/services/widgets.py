from rest.client import get
import json


def get_widget(widget_id):
    response = get(f"widgets/{widget_id}")
    return response

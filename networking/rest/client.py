import requests
import logging
from .exceptions import APIClientError, APIClientResponseError, APIClientConnectionError

class RestClient:
    def __init__(self):
        self.base_url = "http://localhost:8000/api"
        self.headers = {
            "Content-Type": "application/json"
        }
        self.timeout = 10
        logging.basicConfig(level=logging.DEBUG)

    def _make_request(self, method, endpoint, **kwargs):
        url = f"{self.base_url}/{endpoint}"
        try:
            response = requests.request(method, url, headers=self.headers, timeout=self.timeout, **kwargs)
            response.raise_for_status()
        except requests.exceptions.HTTPError as http_err:
            logging.error(f"HTTP error occurred: {http_err}")
            raise APIClientResponseError(response) from http_err
        except requests.exceptions.ConnectionError as conn_err:
            logging.error(f"Connection error occurred: {conn_err}")
            raise APIClientConnectionError from conn_err
        except requests.exceptions.Timeout as timeout_err:
            logging.error(f"Timeout error occurred: {timeout_err}")
            raise APIClientError(f"Timeout error: {timeout_err}") from timeout_err
        except requests.exceptions.RequestException as req_err:
            logging.error(f"An error occurred: {req_err}")
            raise APIClientError(f"Request error: {req_err}") from req_err

        body = response.json()
        return body['data']

    def get(self, endpoint, params=None):
        return self._make_request('GET', endpoint, params=params)

    def post(self, endpoint, data=None, json=None):
        return self._make_request('POST', endpoint, data=data, json=json)

    def put(self, endpoint, data=None, json=None):
        return self._make_request('PUT', endpoint, data=data, json=json)

    def delete(self, endpoint):
        return self._make_request('DELETE', endpoint)

    def patch(self, endpoint, data=None, json=None):
        return self._make_request('PATCH', endpoint, data=data, json=json)

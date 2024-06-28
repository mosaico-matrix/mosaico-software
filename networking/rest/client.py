import requests
import logging
from .exceptions import APIClientError, APIClientResponseError, APIClientConnectionError

base_url = "http://localhost:8000/api"
headers = {
    "Content-Type": "application/json"
}
timeout = 10
logging.basicConfig(level=logging.DEBUG)


def _make_request(method, endpoint, **kwargs):
    url = f"{base_url}/{endpoint}"
    try:
        response = requests.request(method, url, headers=headers, timeout=timeout, **kwargs)
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


def get(endpoint, params=None):
    return _make_request('GET', endpoint, params=params)


def post(endpoint, data=None, json=None):
    return _make_request('POST', endpoint, data=data, json=json)


def put(endpoint, data=None, json=None):
    return _make_request('PUT', endpoint, data=data, json=json)


def delete(endpoint):
    return _make_request('DELETE', endpoint)


def patch(endpoint, data=None, json=None):
    return _make_request('PATCH', endpoint, data=data, json=json)

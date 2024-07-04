import logging
logger = logging.getLogger('mosaico_networking')
class APIClientError(Exception):

    def __init__(self, message):
        self.message = message
        super().__init__(self.message)

    def __str__(self):
        return self.message

class APIClientResponseError(APIClientError):
    """Raised when a response contains an HTTP error status"""
    def __init__(self, response):
        self.response = response
        self.status_code = response.status_code
        self.message = response.text
        super().__init__(f"HTTP {self.status_code}: {self.message}")

class APIClientConnectionError(APIClientError):
    """Raised when a connection error occurs"""
    pass
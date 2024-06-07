import logging
logger = logging.getLogger(name=__name__)

class APIClientError(Exception):
    """General API Client Error"""
    logger.error("An error occurred")

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
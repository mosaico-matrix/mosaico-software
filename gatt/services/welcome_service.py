from .service_dispatcher import AsyncInitMixin

# Note: this service is only used for discovery purposes and does not have any characteristics
class WelcomeService(AsyncInitMixin):
    server = None
    service_uuid = "d34fdcd0-83dd-4abe-9c16-1230e89ad2f2"
    characteristics = {}

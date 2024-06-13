import json
import socket
import logging

logger = logging.getLogger(__name__)


def call_matrix(command, data: dict) -> dict:

    # Connect to the server
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('localhost', 10000)
    sock.connect(server_address)

    # Convert data to json
    data = json.dumps(data)
    logger.debug(f"Sending {command} with data {data}")

    # Send the message
    message = f"{command} {data}"
    sock.sendall(message.encode())
    response = sock.recv(1024)  # 1KB
    sock.close()

    # Convert the response to a dictionary if actual data is returned
    response = response.decode()
    if response:
        logger.debug(f"Received response {response}")
        response = json.loads(response)
        return response
    else:
        return {}

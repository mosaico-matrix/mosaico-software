import socket


def call_matrix(command, data):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('localhost', 10000)
    sock.connect(server_address)
    message = f"{command} {data}"
    sock.sendall(message.encode())
    response = sock.recv(1024) # 1KB
    sock.close()
    return response.decode()

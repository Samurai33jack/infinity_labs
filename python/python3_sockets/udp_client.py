import socket
import time


def ping_server(host='127.0.0.1', port=42842):
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        while True:
            s.sendto(b'Ping', (host, port))
            data, addr = s.recvfrom(1024)
            print(f"client received: {data.decode()}")
            time.sleep(4)


if __name__ == "__main__":
    ping_server()

import socket


HOST = "127.0.0.1"
PORT = 42707


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as test_socket:
    test_socket.connect((HOST, PORT))
    test_socket.sendall(b"ping")
    data = test_socket.recv(1024)
    data_text = data.decode(encoding="utf-8")
    print(f"client received {data_text}")


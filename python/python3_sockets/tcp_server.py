import socket


HOST = "127.0.0.1"
PORT = 42842

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as test_socket:
    test_socket.bind((HOST, PORT))
    test_socket.listen()
    conn, addr = test_socket.accept()
    with conn:

        while True:
            data = conn.recv(1024)
            data_text = data.decode(encoding="utf-8")
            if not data:
                break
            print(f"server received {data_text}")
            conn.sendall(b"pong")


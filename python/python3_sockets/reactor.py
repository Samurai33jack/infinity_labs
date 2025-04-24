import socket
import select


class ReactorServer:
    SOCK = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    SOCK.bind(('localhost', 42707))
    SOCK.listen()
    SOCK.setblocking(False)

    def __init__(self):
        self.readable_sockets = []
        self.writeable_sockets = []
        self.exception_sockets = []
        self.readable_sockets_callback = []
        self.writeable_sockets_callback = []
        self.exception_sockets_callback = []
        self.select_on = False

    def accept_connection(self, server_sock, mode):
        client_socket, client_address = server_sock.accept()
        print(f"Accepted connection from {client_address}")
        client_socket.setblocking(False)
        if "read" == mode:
            self.subscribe_socket(client_socket, "read", self.handle_readable)
        elif "write" == mode:
            self.subscribe_socket(client_socket, "write", self.handle_writable)

    def subscribe_socket(self, sock, mode, callback):
        if "read" == mode:
            self.readable_sockets.insert(sock.fileno(), sock)
            self.readable_sockets_callback.insert(sock.fileno(), callback)
        elif "write" == mode:
            self.writeable_sockets.insert(sock.fileno(), sock)
            self.writeable_sockets_callback.insert(sock.fileno(), callback)
        elif "exec" == mode:
            self.exception_sockets.insert(sock.fileno(), sock)
            self.exception_sockets_callback.insert(sock.fileno(), callback)

    def unsubscribe_socket(self, sock, mode):
        if "read" == mode:
            self.readable_sockets.pop(sock.fileno())
        elif "write" == mode:
            self.writeable_sockets.pop(sock.fileno())
        elif "exec" == mode:
            self.exception_sockets.pop(sock.fileno())

    def run(self):
        self.select_on = True
        self.loop()

    def stop(self):
        self.select_on = False

    def loop(self):
        while self.select_on:
            try:
                ready_to_read, ready_to_write, in_error = select.select(self.readable_sockets,
                                                                        self.writeable_sockets,
                                                                        self.exception_sockets)
                for sock in ready_to_read:
                    self.readable_sockets_callback[sock.fileno()](sock, "read")
                for sock in ready_to_write:
                    self.writeable_sockets_callback[sock.fileno()](sock, "write")
                for sock in in_error:
                    pass

            except KeyboardInterrupt:
                print('keyboard interrupted')

    def handle_exception(self, sock, mode):
        pass

    def handle_readable(self, sock, mode):
        try:
            data = sock.recv(4096)
            if data:
                print(f"received from client: {data.decode()}")
                sock.sendall(b"received data")
            else:
                print("client disconnected")
                self.unsubscribe_socket(sock, "read")
                sock.close()
        except ConnectionResetError:
            print("closed connection")
            self.unsubscribe_socket(sock, "read")
            sock.close()

    def handle_writable(self, sock):
        try:
            data = b"this is what we do, and we do it for you, so you should do it too, and pass it through you"
            sock.sendall(data)
            print(f"send to client: {data}")
        except ConnectionResetError:
            print("closed connection")
            self.unsubscribe_socket(sock, "write")
            sock.close()


if __name__ == "__main__":
    new_reactor = ReactorServer()
    new_reactor.subscribe_socket(new_reactor.SOCK, "read", new_reactor.accept_connection)

    new_reactor.run()

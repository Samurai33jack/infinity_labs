import socket, select

host = '127.0.0.1'
port = 10000

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect((host, port))
inout = [socket]

while 1:
    infds, outfds, errfds = select.select(inout, inout, [], 5)
    if len(infds) != 0:
        buf = socket.recv(1024)
        if len(buf) != 0:
            print('receive data:', buf)
    if len(outfds) != 0:
        socket.send(b"python select client from Debian.\n")

import socket

import keyboard

print("Creating server...")
s = socket.socket()
s.bind(('0.0.0.0', 3131))
s.listen(0)

client, addr = s.accept()
print("Client connected")

while True:
    content = client.recv(1024)
    msg = content.decode().split(";")
    for line in msg:
        print(line)

    if keyboard.is_pressed("q"):
        break

print("Closing connection")
client.close()


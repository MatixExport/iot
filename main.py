import socket
import csv
import time
import keyboard



print("Creating server...")
s = socket.socket()
s.bind(('0.0.0.0', 3131))
s.listen(0)

gate_states = [
    {
        "symbol":"w",
        "state":"up",
        "per_second_change": 3.33
    },
    {
        "symbol": "s",
        "state": "down",
        "per_second_change": - 5.13
    },
    {
        "symbol": "space",
        "state": "unknown",
        "per_second_change": 0
    }
]

with open("data.csv", "w", newline='') as csv_file:
    writer = csv.writer(csv_file)

    client, addr = s.accept()
    print("Client connected")
    current_state_value = 0
    current_state = gate_states[2]
    last_recorded_time = time.time()
    while True:
        for state in gate_states:
            if keyboard.is_pressed(state['symbol']):
                current_state = state
                break

        content = client.recv(1024)
        msg = content.decode().split(";")
        msg = msg[:-1]

        for line in msg:
            time_change = time.time() - last_recorded_time
            last_recorded_time = time.time()
            current_state_value += (current_state['per_second_change']) * time_change

            print(current_state_value)

            data_arr = line.split(",")
            data_arr.append("     ")
            data_arr.append(str(current_state_value))

            data_arr.append(str(time.time()))

            # if len(data_arr) == 11:
            writer.writerow(data_arr)
        if keyboard.is_pressed("q"):
            break

    print("Closing connection")
    client.close()


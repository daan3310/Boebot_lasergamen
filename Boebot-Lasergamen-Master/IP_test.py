import socket
import time

def send_string(String, send_ip, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Create a UDP socket

    try:
        # Send the message
        # Note: No need to connect for UDP, directly send to address
        sock.sendto(String.encode(), (send_ip, port))
        print(f"Sent message: {String}")
    finally:
        sock.close()  # Note the parentheses to actually call the close method

while True:
    send = "Dit is een test"
    ip = '192.168.0.101'
    port = 12345  # Should be an integer
    send_string(send, ip, port)
    print("\nBericht verstuurd!")
    time.sleep(5)

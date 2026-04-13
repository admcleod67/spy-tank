#!/usr/bin/env python3
import socket
import argparse
import sys
import time


DEFAULT_IP = "10.10.1.1"
CONTROL_PORT = 8150


class SpyTank:
    LEFT = 1
    RIGHT = 2
    CAMERA = 3

    FORWARD = 1
    BACK = 2
    STOP = 0

    def __init__(self, ip=DEFAULT_IP, port=CONTROL_PORT, timeout=2.0):
        self.ip = ip
        self.port = port
        self.timeout = timeout
        self.sock = None

    # ----------------------------
    # Connection handling
    # ----------------------------
    def connect(self):
        if self.sock:
            return

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(self.timeout)
        self.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)

        print(f"Connecting to {self.ip}:{self.port} …")
        self.sock.connect((self.ip, self.port))

        # Initial handshake from original code: send "t1"
        self.sock.sendall(b"t1")
        print("Connected and handshake sent.")

    def close(self):
        if self.sock:
            try:
                self.sock.close()
            except Exception:
                pass
            self.sock = None

    # ----------------------------
    # Protocol commands
    # ----------------------------
    def send_command(self, motor, direction):
        if not self.sock:
            raise RuntimeError("Not connected")

        msg = f"{motor}{direction}".encode("ascii")
        self.sock.sendall(msg)
        print(f"Sent: {msg!r}")

    # Convenience wrappers
    def left(self, direction):
        self.send_command(self.LEFT, direction)

    def right(self, direction):
        self.send_command(self.RIGHT, direction)

    def camera(self, direction):
        self.send_command(self.CAMERA, direction)


# ------------------------------------------------------------
# Command-line interface
# ------------------------------------------------------------
def main():
    parser = argparse.ArgumentParser(
        description="Instant Spy Tank command-line controller"
    )

    parser.add_argument("--ip", default=DEFAULT_IP, help="Tank IP address")
    parser.add_argument("--port", type=int, default=CONTROL_PORT, help="Control port")

    sub = parser.add_subparsers(dest="cmd", required=True)

    # Motor commands
    move = sub.add_parser("move", help="Move a motor")
    move.add_argument("motor", choices=["left", "right", "camera"])
    move.add_argument("direction", choices=["forward", "back", "stop"])

    # Simple test pattern
    sub.add_parser("test", help="Run a simple movement test")

    args = parser.parse_args()

    tank = SpyTank(ip=args.ip, port=args.port)
    tank.connect()

    if args.cmd == "move":
        motor_map = {"left": 1, "right": 2, "camera": 3}
        dir_map = {"forward": 1, "back": 2, "stop": 0}

        tank.send_command(motor_map[args.motor], dir_map[args.direction])

    elif args.cmd == "test":
        print("Running test pattern…")
        tank.left(1)
        tank.right(1)
        time.sleep(1)
        tank.left(0)
        tank.right(0)
        print("Test complete.")

    tank.close()


if __name__ == "__main__":
    main()

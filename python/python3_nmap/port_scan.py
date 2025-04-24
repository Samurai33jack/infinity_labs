'''
scans all relevent ports(1-1024) and prints open ports
'''

from scapy.all import *
from scapy.layers.inet import IP, TCP


def port_scan(target_ip=None):
    # src_port = RandShort()
    # for port in range(1, 1024):
    try:
        scan_response, _ = sr(IP(dst=target_ip) / TCP(dport=range(1, 1024), flags="S"), timeout=10, verbose=0)
        for response in scan_response:
            if response.answer[TCP].flags == "SA":
                print(f"{response.query[TCP].dport} is open")
    except Exception as e:
        print(e)


def main():
    ip = input("Enter IP Address: ")
    port_scan(ip)
    print("All done")


if __name__ == "__main__":
    main()

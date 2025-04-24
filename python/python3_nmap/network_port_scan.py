'''
scans all devices and checks if given port is open
'''

from scapy.all import srp
from scapy.layers.l2 import Ether, ARP
from scapy.layers.inet import IP, TCP
from scapy.sendrecv import sr


def check_open_port_in_network(target_ip=None, target_port=None):
    target_ip = target_ip + "/24"
    print(target_ip)
    arp = ARP(pdst=target_ip)
    ether = Ether(dst="ff:ff:ff:ff:ff:ff")
    packet = ether / arp
    answered, _ = srp(packet, timeout=5, verbose=0)

    devices_ip_in_network = []
    for ans in answered:
        devices_ip_in_network.append(ans.query[ARP].pdst)
    counter = 0
    scan_response, _ = sr(IP(dst=devices_ip_in_network) / TCP(dport=target_port, flags="S"),
                          timeout=5, verbose=0, retry=-2)

    for response in scan_response:
        if response.answer[TCP].flags == "SA":
            print(f"Target {response.query[IP].dst} is open on port {target_port}")
            counter += 1

    print(f"counter is {counter}")


def main():
    # ip = input("Enter IP Address: ")
    # port = input("Enter Port Number: ")
    ip = "10.1.0.120"
    port = "80"
    check_open_port_in_network(ip, int(port))
    print("All done")


if __name__ == "__main__":
    main()

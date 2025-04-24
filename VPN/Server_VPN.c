/***************************************************
* Simple VPN server                    
* Author: Jakob Shnider, Cr7                       
* Date of writing: 10/09/24                                
* Last Updated: 10/09/24                                    
* Reviewed by:                                    
***************************************************/

#include <stdio.h>    /* printf, perror */
#include <string.h>   /* memset, strncpy */
#include <stdlib.h>   /* system, exit */
#include <unistd.h>   /* open, read, write, close */
#include <arpa/inet.h>/* inet_ntoa, ntohs, INADDR_ANY */
#include <fcntl.h>    /* open, close, fcntl */
#include <sys/ioctl.h>/* ioctl */
#include <linux/if.h> /* struct ifreq, IFF_TUN, IFF_NO_PI */
#include <linux/if_tun.h> /* TUNSETIFF */
#include <signal.h>   /* signal, SIGINT, SIGTERM */

#define BUFFER_SIZE 4096
#define SERVER_PORT 4242

int OpenTun() 
{
    struct ifreq ifr;
    int fd;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0) 
    {
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

    strncpy(ifr.ifr_name, "tun0", IFNAMSIZ);

    if (ioctl(fd, TUNSETIFF, (void *)&ifr) < 0) 
    {
        close(fd);
        return -1;
    }

    return fd;
}

int SetUpRouting() 
{
    system("ip addr add 142.142.0.142/24 dev tun0");
    system("ip link set tun0 up");
    system("sysctl -w net.ipv4.ip_forward=1");
    system("iptables -t nat -A POSTROUTING -s 142.142.0.0/16 ! -d 142.142.0.0/16 -j MASQUERADE");
    system("iptables -A FORWARD -s 142.142.0.0/16 -m state --state RELATED,ESTABLISHED -j ACCEPT");
    system("iptables -A FORWARD -d 142.142.0.0/16 -j ACCEPT");

    return 0;
}

int OpenSocket() 
{
    int sock_fd = 0;
    struct sockaddr_in server_addr;

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
    {
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

static void SetNonBlocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) 
    {
        exit(1);
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) 
    {
        exit(1);
    }
}

int CleanUp() 
{
    system("ip route del default via 142.142.0.142 dev tun0");
    system("ip link set tun0 down");
    system("ip addr flush dev tun0");
    system("sysctl -w net.ipv4.ip_forward=0");
    system("iptables -t nat -D POSTROUTING -s 142.142.0.0/16 ! -d 142.142.0.0/16 -j MASQUERADE");
    system("iptables -D FORWARD -s 142.142.0.0/16 -m state --state RELATED,ESTABLISHED -j ACCEPT");
    system("iptables -D FORWARD -d 142.142.0.0/16 -j ACCEPT");

    return 0;
}

void SignalHandler(int signum) 
{
    CleanUp();
    exit(0);
}

int VPNHandler() 
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    int tun_fd = 0;
    int sock_fd = 0;
    int recv_amount = 0;
    int read_amount = 0;

    tun_fd = OpenTun();

    if (tun_fd < 0) 
    {
    
        return 1;
    }

    if (SetUpRouting()) 
    {
        close(tun_fd);
        return 1;
    }

    sock_fd = OpenSocket();

    if (sock_fd < 0) 
    {
        close(tun_fd);
        return 1;
    }

    SetNonBlocking(tun_fd);
    SetNonBlocking(sock_fd);

    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    while (42) 
    {
        recv_amount = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        
        if (recv_amount) 
        {
            write(tun_fd, buffer, recv_amount);
        }

        read_amount = read(tun_fd, buffer, BUFFER_SIZE);

        if (read_amount) 
        {
            sendto(sock_fd, buffer, read_amount, 0, (struct sockaddr *)&client_addr, client_len);
        }
    }

    CleanUp();

    close(tun_fd);
    close(sock_fd);

    return 0;
}

int main() 
{
    return VPNHandler();
}

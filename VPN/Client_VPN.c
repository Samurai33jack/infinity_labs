/***************************************************
* Simple VPN client                    
* Author: Jakob Shnider, Cr7                       
* Date of writing: 10/09/24                                
* Last Updated: 10/09/24                                    
* Reviewed by:                                    
***************************************************/

#include <stdio.h>    /* printf, perror */
#include <string.h>   /* memset, strncpy */
#include <stdlib.h>   /* system, exit */
#include <unistd.h>   /* open, read, write, close */
#include <arpa/inet.h>/* inet_addr, htons */
#include <fcntl.h>    /* open, close, fcntl */
#include <sys/ioctl.h>/* ioctl */
#include <linux/if.h> /* struct ifreq, IFF_TUN, IFF_NO_PI */
#include <linux/if_tun.h> /* TUNSETIFF */
#include <signal.h>   /* signal, SIGINT, SIGTERM */

#define BUFFER_SIZE 4096
#define SERVER_PORT 4242

#define VPN_SERVER_IP "10.1.0.5"

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
    system("ip route del default via 142.142.0.42 dev tun0");
    system("ip link set tun0 down");
    system("ip addr flush dev tun0");
    system("echo 'nameserver 127.0.0.53' > /etc/resolv.conf");

    return 0;
}

void SignalHandler(int signum) 
{
    CleanUp();
    exit(0);
}

int OpenSocket() 
{
    int sockfd = 0;
    struct sockaddr_in server_addr;

    if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, 0))) 
    {
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(VPN_SERVER_IP);

    if (0 > connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) 
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int SetUpRouting() 
{
    system("ip addr add 142.142.0.42/24 dev tun0");
    system("ip link set tun0 up");
    system("ip route add default via 142.142.0.42 dev tun0");
    system("echo 'nameserver 8.8.8.8' > /etc/resolv.conf");

    return 0;
}

int VPNHandler() 
{
    char buffer[BUFFER_SIZE];

    int tun_fd = OpenTun();

    if (tun_fd < 0) 
    {
        return 1;
    }

    if (!SetUpRouting()) 
    {
        close(tun_fd);
        return 1;
    }

    int sockfd = OpenSocket();

    if (sockfd < 0) 
    {
        close(tun_fd);
        return 1;
    }

    SetNonBlocking(tun_fd);
    SetNonBlocking(sockfd);

    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    while (42) 
    {
        int read_amount = read(tun_fd, buffer, BUFFER_SIZE);

        if (read_amount > 0) 
        {
            send(sockfd, buffer, read_amount, 0);
        }

        int recv_amount = recv(sockfd, buffer, BUFFER_SIZE, 0);

        if (recv_amount > 0) 
        {

            write(tun_fd, buffer, recv_amount);
        }
    }

    close(tun_fd);
    close(sockfd);

    return 0;
}

int main() 
{
    return VPNHandler();
}

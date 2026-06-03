#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <fcntl.h>

static std::string read_first_line(const char *path) {
    char buf[256] = {};
    int fd = open(path, O_RDONLY);
    if (fd < 0) return "ERROR";
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    close(fd);
    if (n > 0) {
        while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) buf[--n] = '\0';
        buf[n] = '\0';
    }
    return buf;
}

// Check if a network interface is UP using ioctl (respects current net namespace).
static bool iface_is_up(const char *ifname) {
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return false;
    bool up = false;
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
        up = (ifr.ifr_flags & IFF_UP) != 0;
    }
    close(sock);
    return up;
}

// Count only network interfaces that are UP.
// In a fresh CLONE_NEWNET namespace, lo exists but is down, so count is 0.
static int count_net_ifaces() {
    FILE *f = fopen("/proc/net/dev", "r");
    if (!f) return -1;
    char line[256];
    int count = 0;
    int lineno = 0;
    while (fgets(line, sizeof(line), f)) {
        ++lineno;
        if (lineno <= 2) continue;
        size_t len = strlen(line);
        if (len <= 1) continue;

        // Extract interface name (text before ':')
        char *colon = strchr(line, ':');
        if (!colon) continue;
        const char *src = line;
        while (*src == ' ') ++src;
        size_t n = colon - src;
        if (n > 255) n = 255;
        char ifname[256] = {};
        memcpy(ifname, src, n);
        ifname[n] = '\0';

        if (iface_is_up(ifname)) ++count;
    }
    fclose(f);
    return count;
}

int main() {
    std::cout << "PID=" << getpid() << std::endl;
    std::cout << "PPID=" << getppid() << std::endl;
    std::cout << "UID=" << getuid() << std::endl;
    std::cout << "GID=" << getgid() << std::endl;
    std::cout << "PID1_COMM=" << read_first_line("/proc/1/comm") << std::endl;
    std::cout << "NET_IFACES=" << count_net_ifaces() << std::endl;
    return 0;
}

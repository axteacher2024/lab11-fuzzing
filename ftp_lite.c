#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
static void login(int c, const char *name) {
    char user[32];
    strcpy(user, name);
    dprintf(c, "331 %s uchun parol kiriting\r\n", user);
}

static void handle(int c) {
    char line[512];
    ssize_t n = recv(c, line, sizeof line - 1, 0);
    if (n <= 0) return;
    line[n] = 0;
    if (strncmp(line, "USER ", 5) == 0) {
        line[strcspn(line, "\r\n")] = 0;
        login(c, line + 5);
    } else {
        dprintf(c, "500 Noma'lum buyruq\r\n");
    }
}

#define PORT 2121

int main(void) {
    int s = socket(AF_INET, SOCK_STREAM, 0), one = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
    struct sockaddr_in a;
    memset(&a, 0, sizeof a);
    a.sin_family = AF_INET;
    a.sin_port = htons(PORT);
    a.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* faqat 127.0.0.1 */
    if (bind(s, (struct sockaddr *)&a, sizeof a) < 0 || listen(s, 16) < 0) {
        perror("bind/listen");
        return 1;
    }
    printf("ftp_lite: 127.0.0.1:%d tinglanmoqda\n", PORT);
    fflush(stdout);
    for (;;) {
        int c = accept(s, NULL, NULL);
        if (c < 0) continue;
        handle(c);
        close(c);
    }
}

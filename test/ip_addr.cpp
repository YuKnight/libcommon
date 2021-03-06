#include "network/ip_addr.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace libcommon;

void test_ip_addr();
void test_ip_addr2();
void test_connect();

void test_ip_addr()
{

    ip_addr_t t("www.baidu.com");
    char buff[64] = {0};
    if (t.get_ip_addr(buff) == 0) {

        printf("got ip address %s \n", buff);

        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            printf("got error socket ! \n");
            return ;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        addr.sin_addr.s_addr = inet_addr(buff);

        if (::connect(s, (:: sockaddr*)&addr, sizeof(sockaddr_in)) < 0) {
            printf("got error socket connect! [%s]\n", strerror(errno));
            close(s);
            return ;
        }

        printf("got succ connect !! \n");

        char ip_buf[64];
        int p = 0;
        if (t.get_socket_name(s, ip_buf, p) == 0) {

            printf("got succ get socket name ! [%s %d] \n", ip_buf, p);
        } else {
            printf("got fail get socket name! \n");
        }
        close(s);
    }

    memset(buff, 0, sizeof(buff));
    if (t.get_ip_addr(AF_INET, buff) == 0) {

        printf("got ip v4 address %s \n", buff);
    }

    memset(buff, 0, sizeof(buff));
    if (t.get_ip_addr(AF_INET6, buff) == 0) {

        printf("got ip v6 address %s \n", buff);
    }

    memset(buff, 0, sizeof(buff));
    struct sockaddr sa = {0};
    int sa_len = 0;
    if (t.get_ip_addr2(AF_INET, 80, &sa, sa_len) == 0) {
        printf("got ip address 2 success !\n");

        int s  = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            printf("got error socket ! \n");
            return ;
        }
        if (connect(s, &sa, sa_len) < 0) {
            printf("got error socket connect ! [%s] \n", strerror(errno));
            close(s);
            return ;
        }

        printf("got socket connect success! \n");
        close(s);
    }

    typedef std::vector<std::string> TypeIpList;
    TypeIpList ip_list;
    if (t.get_ip_addr(ip_list) == 0) {
        printf("got ip address list success! \n");

        TypeIpList::iterator iter = ip_list.begin();
        while (iter != ip_list.end()) {
            printf("ip list [%s] \n", iter->c_str());
            iter++;
        }

    }

}

void test_ip_bind_func(struct sockaddr* addr, int len)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("got socket error ! \n");
        return ;
    }

    int option = 1;
    if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)  ) < 0) {
        printf("set socket opt error! SO_REUSEADDR \n");
    }

    if (bind(s, addr, len) == 0) {
        printf("bind succ ! \n");
    } else {
        printf("%s == bind fail ! \n", __FUNCTION__);
    }

    close(s);
}

void test_ip_addr2()
{

   
}


void test_connect()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("got error socket ! \n");
        return ;
    }
    connect_addr_t t(s);
    const char* host = "www.baidu.com";
    if (t.connect_addr(host, 80)) {
        printf("connect baidu.com succ !\n");
    } else {
        printf("connect baidu.com fail !\n");
    }
    close(s);


    ip_addr_t ip(host);
    char buff[64] = {0};
    if (ip.get_ip_addr(buff) == 0) {

        printf("got ip address %s \n", buff);

        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            printf("got error socket ! \n");
            return ;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        addr.sin_addr.s_addr = inet_addr(buff);

        if (t.connect_addr((struct sockaddr*)&addr, sizeof(sockaddr_in)) < 0) {
            printf("got error socket connect! [%s]\n", strerror(errno));
            close(s);
            return ;
        }
        printf("got succ connect %s baidu.com !! \n", __FUNCTION__);
        close(s);
    }

    if (true) {
        // connect_ex with timeout
        int s = socket(AF_INET, SOCK_STREAM, 0);
        connect_addr_t t(s);

        // connect with timeout 1s
        int result = t.connect_addr_ex(host, 80, 1000);
        if (result == SOCK_CONNECTED) {
            printf("socket connect with timeout [connected] \n");
        } else if (result == SOCK_CONNECT_TIMEOUT) {
            printf("socket connect with timeout [timeout] \n");
        } else if (result == SOCK_CONNECT_ERR) {
            printf("socket connect with timeout [error] \n");
        }

        close(s);
    }

    if (true) {
        // connect_ex with timeout
        int s = socket(AF_INET, SOCK_STREAM, 0);
        connect_addr_t t(s);

        // connect with timeout 10s
        int result = t.connect_addr_ex(host, 80, 10 * 1000);
        if (result == SOCK_CONNECTED) {
            printf("socket connect with timeout [connected] \n");
        } else if (result == SOCK_CONNECT_TIMEOUT) {
            printf("socket connect with timeout [timeout] \n");
        } else if (result == SOCK_CONNECT_ERR) {
            printf("socket connect with timeout [error] \n");
        }

        close(s);
    }

}

int main()
{
    test_ip_addr();
    test_ip_addr2();
    test_connect();
    return 0;
}


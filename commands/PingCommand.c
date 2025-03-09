#include "PingCommand.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>


#define PING_PACKET_DATA_SIZE 64
#define PING_TIMEOUT          1000
#define PING_NUMBER           4
#define ERR_OK                0
#define ERR_FAILED            -1
#define ERR_TIMEOUT           1

// Функция помощи для команды ping
static void help() {
    printf("ping: ping ADDRESS\n      Sends ICMP echo requests to the specified ADDRESS.\n");
}

// Структура для пакета ping
typedef struct {
    struct icmp header;
    char data[PING_PACKET_DATA_SIZE];

} pingPacket;

// Структура для IP-пакета
typedef struct {
    struct ip ipHeader;
    pingPacket pingPacket;
} ipPacket;

// Функция для вычисления контрольной суммы
unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;

    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }

    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

// Подготовка пакета ICMP для отправки
static void prepareIcmpPacker(pingPacket *packet) {
    memset(packet, 0, sizeof(packet));
    srand(time(NULL));
    unsigned short randomId = rand();
    packet->header.icmp_hun.ih_idseq.icd_id = randomId; 
    packet->header.icmp_type = ICMP_ECHO;                
    packet->header.icmp_hun.ih_idseq.icd_seq = 0;

    memset(packet->data, 0xAA, sizeof(packet->data));
    packet->header.icmp_cksum = 0;

    packet->header.icmp_cksum = checksum(packet, sizeof(pingPacket));
}

// Получение текущего времени в миллисекундах
static unsigned long getCurTimeMs() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    unsigned long timeMs = time.tv_sec * 1000 + (time.tv_nsec / 1000000);
    return timeMs;
}

// Функция для выполнения пинга
int ping(const char* ip, const unsigned long timeout, unsigned long* replyTime) {
    // Проверка входных параметров на корректность
    if (ip == NULL || timeout == 0) {
        return ERR_FAILED;
    }

    pingPacket pingPacket;                                                      // Структура для пинг-пакета
    prepareIcmpPacker(&pingPacket);                                             // Подготовка пакета ICMP для отправки
    const unsigned short reply_id = pingPacket.header.icmp_hun.ih_idseq.icd_id; // ID ответа

    // Настройка структуры для адреса хоста
    struct sockaddr_in toAddr;
    toAddr.sin_family = AF_INET;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    // Разрешение имени хоста в IP-адрес
    if (getaddrinfo(ip, NULL, &hints, &res) != 0) {
        printf("Host resolution error: %s\n", ip);
        return ERR_FAILED;
    }

    struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
    toAddr.sin_addr = addr->sin_addr;

    freeaddrinfo(res); // Освобождаем память, выделенную getaddrinfo

    // Создание сокета для отправки ICMP-запросов
    const int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("socket() %s\n");
        return ERR_FAILED;
    }

    // Получаем текущее время для вычисления времени ответа
    const unsigned long start_time_ms = getCurTimeMs();
    const socklen_t sockLen = sizeof(struct sockaddr_in);

    // Отправка ICMP-запроса
    if (sendto(sock, &pingPacket, sizeof(pingPacket), 0, (struct sockaddr*)&toAddr, sockLen) <= 0) {
        close(sock);
        return ERR_FAILED;
    }

    int result = ERR_FAILED;                // Результат выполнения пинга
    struct timeval tv;
    tv.tv_sec = timeout / 1000;             // Устанавливаем таймаут
    tv.tv_usec = (timeout % 1000) * 1000;

    // Ожидание ответа на пинг
    for (;;) {
        fd_set rfd;
        FD_ZERO(&rfd);
        FD_SET(sock, &rfd);

        int n;
        do {
            n = select(sock + 1, &rfd, 0, 0, &tv); // Ожидаем ответа с указанным таймаутом
        } while (n < 0 && errno == EINTR);

        // Если таймаут истек, выходим из цикла
        if (n == 0) {
            result = ERR_TIMEOUT;
            break;
        }

        if (n < 0) {
            break;
        }

        // Проверяем, не истекло ли время ответа
        const unsigned long elapsed_time = (getCurTimeMs() - start_time_ms);
        if (elapsed_time > timeout) {
            result = ERR_TIMEOUT;
            break;
        } else {
            const int new_timeout = timeout - elapsed_time;
            tv.tv_sec = new_timeout / 1000;
            tv.tv_usec = (new_timeout % 1000) * 1000;
        }

        // Если получен ответ
        if (FD_ISSET(sock, &rfd)) {
            ipPacket ipPkt; // Структура для IP-пакета
            struct sockaddr_in from_addr;
            socklen_t socklen = sizeof(struct sockaddr_in);

            // Получаем IP-ответ
            if (recvfrom(sock, &ipPkt, sizeof(ipPacket), 0, (struct sockaddr*)&from_addr, &socklen) <= 0) {
                break;
            }

            // Проверяем, что полученный ответ соответствует отправленному запросу
            if (toAddr.sin_addr.s_addr == from_addr.sin_addr.s_addr
                    && reply_id == ipPkt.pingPacket.header.icmp_hun.ih_idseq.icd_id) {
                // Если ответ корректный, сохраняем время ответа и выходим
                if (replyTime != NULL) {
                    *replyTime = elapsed_time;
                }
                result = ERR_OK;
                break;
            }
        }
    }
    close(sock);    // Закрытие сокета
    return result;  // Возвращаем результат выполнения пинга
}


// Основная функция выполнения команды ping
static void exec(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        help();
        return;
    }
    if (argc < 2) {
        printf("Invalid parameters, use --help for more information\n");
        return;
    }

    const unsigned long timeout = PING_TIMEOUT;
    const char* const host = argv[1];
    unsigned long replyTime = 0;

    int sentPackets = 0;
    int receivedPackets = 0;
    unsigned long totalTime = 0;

    printf("PING %s (%s): %d bytes of data.\n", host, host, PING_PACKET_DATA_SIZE);

    // Пингование 4 раза (PING_NUMBER)
    for (int i = 0; i < PING_NUMBER; ++i) {
        sentPackets++; // Увеличиваем количество отправленных пакетов
        // Отправляем пинг и получаем результат
        const int result = ping(host, timeout, &replyTime);

        // Проверяем результат выполнения пинга
        if (result == ERR_FAILED) {
            printf("Host is not available.\n");
            break; // Выход из цикла, если хост не доступен
        } else {
            if (result == ERR_TIMEOUT) {
                printf("Request timeout.\n");
            } else {
                // Если пинг успешен, увеличиваем счетчик полученных пакетов
                receivedPackets++;
                totalTime += replyTime; // Добавляем время ответа к общему времени
                printf("Reply from %s: time=%lu ms\n", host, replyTime);
            }
        }
        sleep(1); // Задержка между пингами
    }

    // Вывод статистики пинга
    printf("\n--- %s ping statistics ---\n", host);
    printf("%d packets transmitted, %d received, %.1f%% packet loss\n",
           sentPackets, receivedPackets, 
           ((sentPackets - receivedPackets) / (double)sentPackets) * 100.0);

    // Если получены пакеты, выводим среднее время ответа
    if (receivedPackets > 0) {
        printf("Approximate round trip times: avg = %lu ms\n", totalTime / receivedPackets);
    }
}


// Структура для команды ping
Command cmd_ping = {
    .name = "ping",
    .exec = exec,
    .help = help
};
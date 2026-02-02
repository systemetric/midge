#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "MQTTClient.h"

#define BUF_SIZE 4096

#define ADDRESS "mqtt://localhost:1883"
#define QOS 0

static int _EXIT = 0;
static int _DISCONN = 0;
static int _READER = 0;

static const char *_USAGE = "usage: %s [-r] [-a address] <client id> <topic>\n";

int msgarrvd(void *context, char *topic_name, int topic_len,
             MQTTClient_message *message) {
    if (_READER) {
        printf("%s", (char *)message->payload);
    }

    MQTTClient_free(topic_name);
    MQTTClient_freeMessage(&message);
    return 1;
};

void connlost(void *context, char *cause) {
    printf("conn lost, %s\n", cause);
    _EXIT = 1;
    _DISCONN = 1;
}

int reader_loop(MQTTClient *client, char *topic) {
    int rc;

    if ((rc = MQTTClient_subscribe(*client, topic, QOS)) !=
        MQTTCLIENT_SUCCESS) {
        printf("sub failed, %d\n", rc);
        return 1;
    }

    do {
        sleep(1);
    } while (!_EXIT);

    return 0;
}

int writer_loop(MQTTClient *client, char *topic) {
    int rc;

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    void *buf = (void *)malloc(BUF_SIZE);
    if (!buf) {
        perror("malloc");
        return 1;
    }

    while (!_EXIT) {
        if ((rc = read(STDIN_FILENO, buf, BUF_SIZE)) == 0) {
            break;
        }

        pubmsg.payload = buf;
        pubmsg.payloadlen = rc;
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        if ((rc = MQTTClient_publishMessage(*client, topic, &pubmsg, &token)) !=
            MQTTCLIENT_SUCCESS) {
            printf("pub failed, %d\n", rc);
            free(buf);
            return 0;
        }
    }

    free(buf);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf(_USAGE, argv[0]);
        return 1;
    }

    char *address = ADDRESS;

    int opt;
    while ((opt = getopt(argc, argv, "ra:")) != -1) {
        switch (opt) {
            case 'r':
                _READER = 1;
                break;
            case 'a':
                address = optarg;
                break;
            default:
                printf(_USAGE, argv[0]);
                return 1;
        }
    }

    // must be two arguments after options
    if (argc - optind != 2) {
        printf(_USAGE, argv[0]);
        return 1;
    }

    int rc;

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    if ((rc = MQTTClient_create(&client, address, argv[optind],
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) !=
        MQTTCLIENT_SUCCESS) {
        printf("create failed, %d\n", rc);
        return 1;
    }

    if ((rc = MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd,
                                      NULL)) != MQTTCLIENT_SUCCESS) {
        printf("callback set failed, %d\n", rc);
        return 1;
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("conn failed, %d\n", rc);
        return 1;
    }

    if (_READER) {
        rc = reader_loop(&client, argv[optind + 1]);
    } else {
        rc = writer_loop(&client, argv[optind + 1]);
    }

    // either reader or writer failed, exit early
    if (rc != 0) {
        return rc;
    }

    if (!_DISCONN &&
        (rc = MQTTClient_disconnect(client, 1000)) != MQTTCLIENT_SUCCESS) {
        printf("disconnect failed, %d", rc);
        return 1;
    }

    MQTTClient_destroy(&client);

    return rc;
}

// Adapted by Martín del Río for LDPL from https://github.com/gkbrk/SirLogsalot by Gokberk Yaltirakli

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include "ldpl-types.h"

ldpl_text IRCBOT_SERVER = "";      //195.154.200.232
ldpl_text IRCBOT_PORT = "6667";    //6667
ldpl_text IRCBOT_NICK = "";        //SirLogsalot
ldpl_text IRCBOT_CHANNELS = "";    //#WatchPeopleCode,#WatchCodeTest
ldpl_text IRCBOT_TO;
ldpl_text IRCBOT_MESSAGE;
ldpl_text IRCBOT_CHANNEL;
ldpl_text IRCBOT_FROM;
int socket_desc;

int read_line(int sock, char *buffer) {
    size_t length = 0;

    while (1) {
        char data;
        int result = recv(sock, &data, 1, 0);

        if ((result <= 0) || (data == EOF)){
            perror("Connection closed");
            exit(1);
        }

        buffer[length] = data;
        length++;
        
        if (length >= 2 && buffer[length-2] == '\r' && buffer[length-1] == '\n') {
            buffer[length-2] = '\0';
            return length;
        }
    }
}

char *get_config(char *name) {
    char *value = (char*)malloc(1024);
    FILE *configfile = fopen("config.txt", "r");
    value[0] = '\0';

    if (configfile != NULL) {
        while (1) {
            char configname[1024];
            char tempvalue[1024];

            int status = fscanf(configfile, " %1023[^= ] = %s ", configname, tempvalue); //Parse key=value
            
            if (status == EOF){
                break;
            }
            
            if (strcmp(configname, name) == 0){
                strncpy(value, tempvalue, strlen(tempvalue)+1);
                break;
            }
        }
        fclose(configfile);
    }
    return value;
}

char *get_prefix(char *line) {
    char *prefix = (char*)malloc(512);
    char clone[512];

    strncpy(clone, line, strlen(line) + 1);
    if (line[0] == ':') {
        char *splitted = strtok(clone, " ");
        if (splitted != NULL) {
            strncpy(prefix, splitted+1, strlen(splitted)+1);
        } else {
            prefix[0] = '\0';
        }
    } else {
        prefix[0] = '\0';
    }
    return prefix;
}

char *get_username(char *line) {
    char *username = (char*)malloc(512);
    char clone[512];

    strncpy(clone, line, strlen(line) + 1);
    if (strchr(clone, '!') != NULL) {
        char *splitted = strtok(clone, "!");
        if (splitted != NULL) {
            strncpy(username, splitted+1, strlen(splitted)+1);
        } else {
            username[0] = '\0';
        }
    } else {
        username[0] = '\0';
    }
    return username;
}

char *get_command(char line[]){
    char *command = (char*)malloc(512);
    char clone[512];
    strncpy(clone, line, strlen(line)+1);
    char *splitted = strtok(clone, " ");
    if (splitted != NULL){
        if (splitted[0] == ':'){
            splitted = strtok(NULL, " ");
        }
        if (splitted != NULL){
            strncpy(command, splitted, strlen(splitted)+1);
        }else{
            command[0] = '\0';
        }
    }else{
        command[0] = '\0';
    }
    return command;
}

char *get_last_argument(char line[]){
    char *argument = (char*)malloc(512);
    char clone[512];
    strncpy(clone, line, strlen(line)+1);
    char *splitted = strstr(clone, " :");
    if (splitted != NULL){
        strncpy(argument, splitted+2, strlen(splitted)+1);
    }else{
        argument[0] = '\0';
    }
    return argument;
}

char *get_argument(char line[], int argno){
    char *argument = (char*)malloc(512);
    char clone[512];
    strncpy(clone, line, strlen(line)+1);
    
    int current_arg = 0;
    char *splitted = strtok(clone, " ");
    while (splitted != NULL){
        if (splitted[0] != ':'){
            current_arg++;
        }
        if (current_arg == argno+1){
            strncpy(argument, splitted, strlen(splitted)+1);
            return argument;
        }
        splitted = strtok(NULL, " ");
    }
    
    if (current_arg != argno){
        argument[0] = '\0';
    }
    return argument;
}

void set_nick(int sock, char nick[]){
    char nick_packet[512];
    sprintf(nick_packet, "NICK %s\r\n", nick);
    send(sock, nick_packet, strlen(nick_packet), 0);
}

void send_user_packet(int sock, char nick[]){
    char user_packet[512];
    sprintf(user_packet, "USER %s 0 * :%s\r\n", nick, nick);
    send(sock, user_packet, strlen(user_packet), 0);
}

void join_channel(int sock, char channel[]){
    char join_packet[512];
    sprintf(join_packet, "JOIN %s\r\n", channel);
    send(sock, join_packet, strlen(join_packet), 0);
}

void send_pong(int sock, char argument[]){
    char pong_packet[512];
    sprintf(pong_packet, "PONG :%s\r\n", argument);
    send(sock, pong_packet, strlen(pong_packet), 0);
}

void IRCBOT_SEND_MESSAGE(){
    int sock = socket_desc;
    char message_packet[512];
    sprintf(message_packet, "PRIVMSG %s :%s\r\n", (char*)IRCBOT_TO.str_rep().c_str(), (char*)IRCBOT_MESSAGE.str_rep().c_str());
    send(sock, message_packet, strlen(message_packet), 0);
}

void IRCBOT_ON_MESSAGE();
void IRCBOT_ON_PART();
void IRCBOT_ON_JOIN();

int IRCBOT_CREATE_BOT() {
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1){
       perror("Could not create socket");
       exit(1);
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(IRCBOT_SERVER.str_rep().c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(IRCBOT_PORT.str_rep().c_str()));

    if (connect(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0){
        perror(("Could not connect to " + IRCBOT_SERVER + ":" + IRCBOT_PORT).str_rep().c_str());
        exit(1);
    }

    set_nick(socket_desc, (char*)IRCBOT_NICK.str_rep().c_str());
    send_user_packet(socket_desc, (char*)IRCBOT_NICK.str_rep().c_str());
    join_channel(socket_desc, (char*)IRCBOT_CHANNELS.str_rep().c_str());

    //Wait until connected
    while (1){
        char line[512];
        read_line(socket_desc, line);
        std::string command = get_command(line);
        std::string prefix = get_prefix(line);
        std::string username = get_username(line);
        std::string argument = get_last_argument(line);
        if (command == "JOIN"){
            IRCBOT_CHANNEL = get_argument(line, 1);
            IRCBOT_FROM = username;
            IRCBOT_ON_JOIN();
        }
        else if (command == "PRIVMSG"){
            IRCBOT_CHANNEL = get_argument(line, 1);
            IRCBOT_FROM = username;
            IRCBOT_MESSAGE = argument;
            IRCBOT_ON_MESSAGE();
        }
        else if (command == "PING"){
            send_pong(socket_desc, (char*)argument.c_str());
        }
        else if (command == "PART"){
            IRCBOT_CHANNEL = get_argument(line, 1);
            IRCBOT_FROM = username;
            IRCBOT_ON_PART();
        }
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345



int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[100];
    char ip_servidor[] = "177.34.168.237";

    // Criação do socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    // Configuração do endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip_servidor);

    // Conexão ao servidor
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao conectar ao servidor");
        exit(1);
    }

    printf("Digite a mensagem a ser enviada: ");
    fgets(message, 100, stdin);

    // Envio da mensagem para o servidor
    if (send(sockfd, message, strlen(message), 0) == -1) {
        perror("Erro ao enviar a mensagem");
        exit(1);
    }

    printf("Mensagem enviada com sucesso.\n");

    close(sockfd);

    return 0;
}

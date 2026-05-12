#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void) {

        int porta;

        porta = 4334;

        // 1. Cria o socket
        int servidor = socket(AF_INET, SOCK_STREAM, 0);
        if (servidor < 0) {
            perror("Erro ao criar socket");
            return 1;
        }

        // 2. Define endereço e porta
        struct sockaddr_in endereco;
        endereco.sin_family = AF_INET; // IPV4
        endereco.sin_addr.s_addr = INADDR_ANY; // Escuta qualquer interface (Wifi, Ethernet, Loopback)
        endereco.sin_port = htons(porta); // Converte a porta para ordem de bytes

        // 3. Bind
        if (bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco)) < 0) {
            perror("Erro no bind");
            return 1;
        }

        // 4. Listen
        listen(servidor, 5);
        printf("Servidor escutando na porta: %d.\n", porta);

        while (1){

            // 5. Accept — espera uma conexão
            int cliente = accept(servidor, NULL, NULL);

            char html[4096];
            FILE *f = fopen("index.html", "r");
            fread(html, 1, sizeof(html), f);
            fclose(f);

            // 6. Lê o que o cliente mandou
            char buffer[1024];
            read(cliente, buffer, sizeof(buffer));
            printf("Recebi:\n%s\n", buffer);

            char resposta[5000];
            sprintf(resposta,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "%s", html);

            write(cliente, resposta, strlen(resposta));

            close(cliente);
        }

        // 7. Fecha
        close(servidor);


}
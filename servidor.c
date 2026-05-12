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

            // lê a requisição
            char buffer[1024] = {0};
            read(cliente, buffer, sizeof(buffer));

            // extrai a rota
            char metodo[10], rota[256];
            sscanf(buffer, "%s %s", metodo, rota);

          

            // monta o caminho
            char caminho[512];

            

            
            if (strcmp(rota, "/") == 0) {
                strcpy(caminho, "./public/index.html");
            } else {
                sprintf(caminho, "./public%s", rota);
            }

            printf("Rota: %s\n", rota);
            printf("Caminho: %s\n", caminho);

            // abre o arquivo
            char html[4096] = {0};

            FILE *f = fopen(caminho, "r");

            char *ext = strrchr(caminho, '.');

            char *content_type = "text/html"; // padrão
            if (ext != NULL) {
            if (strcmp(ext, ".css") == 0)  content_type = "text/css";
            if (strcmp(ext, ".js") == 0)   content_type = "application/javascript";
            if (strcmp(ext, ".png") == 0)  content_type = "image/png";
            if (strcmp(ext, ".jpg") == 0)  content_type = "image/jpeg";
            if (strcmp(ext, ".ico") == 0)  content_type = "image/x-icon";

}

            if (f == NULL) {

            char html_404[4096] = {0};
            FILE *err = fopen("./public/internal/erros/404.html", "r");

            if (err != NULL) {
                fread(html_404, 1, sizeof(html_404), err);
                fclose(err);
            } else {
                strcpy(html_404, "<h1>404 - Pagina nao encontrada</h1>");
            }

            char resposta_404[5000];
            sprintf(resposta_404,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "%s", html_404);
            write(cliente, resposta_404, strlen(resposta_404));
            close(cliente);
            continue;
            }


            fread(html, 1, sizeof(html), f);
            fclose(f);

            char resposta[5000];
            sprintf(resposta,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "\r\n"
            "%s", content_type, html);

            write(cliente, resposta, strlen(resposta));

            close(cliente);
        }

        // 7. Fecha
        close(servidor);


}
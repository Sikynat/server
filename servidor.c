#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void) {

    int init;
    
    printf("Iniciar o servidor na porta atual ou trocar? \n");
    printf("[0] Trocar [1] Inicar: ");
    scanf("%d", &init);

    if (init == 0){

        int porta;

        printf("Qual porta conectar: ");
        scanf("%d", &porta);

        FILE *fp = fopen("servidor.config", "w");

        if (fp == NULL) {
            perror("Erro ao abrir arquivo");
            return 1;
        }

        fprintf(fp, "Porta do servidor: %d", porta);

        fclose(fp);

        // 1. Cria o socket
        int servidor = socket(AF_INET, SOCK_STREAM, 0);
        if (servidor < 0) {
            perror("Erro ao criar socket");
            return 1;
        }

        
        int opcao = 1;
        setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(opcao));

        FILE *rp = fopen("servidor.config", "r");

        if (rp == NULL) {
            perror("Erro ao abrir arquivo");
            return 1;
        }


        fscanf(rp, "Porta do servidor: %d\n", &porta);
        
        fclose(rp);

        // 2. Define endereço e porta
        struct sockaddr_in endereco;
        endereco.sin_family = AF_INET;
        endereco.sin_addr.s_addr = INADDR_ANY;
        endereco.sin_port = htons(porta);

        // 3. Bind
        if (bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco)) < 0) {
            perror("Erro no bind");
            return 1;
        }

        // 4. Listen
        listen(servidor, 5);
        printf("Servidor escutando na porta %d...\n", porta);

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

} else {

    int porta;


        // 1. Cria o socket
        int servidor = socket(AF_INET, SOCK_STREAM, 0);
        if (servidor < 0) {
            perror("Erro ao criar socket");
            return 1;
        }

        
        int opcao = 1;
        setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(opcao));

        FILE *rp = fopen("servidor.config", "r");

        if (rp == NULL) {
            perror("Erro ao abrir arquivo");
            return 1;
        }


        fscanf(rp, "Porta do servidor: %d\n", &porta);
        
        fclose(rp);



        // 2. Define endereço e porta
        struct sockaddr_in endereco;
        endereco.sin_family = AF_INET;
        endereco.sin_addr.s_addr = INADDR_ANY;
        endereco.sin_port = htons(porta);

        // 3. Bind
        if (bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco)) < 0) {
            perror("Erro no bind");
            return 1;
        }

        // 4. Listen
        listen(servidor, 5);
        printf("Servidor escutando na porta %d...\n", porta);

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

}
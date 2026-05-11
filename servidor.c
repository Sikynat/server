#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    char versao[32];
    int porta;
} Config;

Config ler_config(void) {
    Config cfg = { .versao = "", .porta = -1 };
    FILE *fp = fopen("servidor.config", "r");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo de config");
        return cfg;
    }
    fscanf(fp, "Version: %31s\n", cfg.versao);
    fscanf(fp, "Porta do servidor: %d\n", &cfg.porta);
    fclose(fp);
    return cfg;
}

int salvar_porta_config(int porta) {
    FILE *fp = fopen("servidor.config", "w");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo de config");
        return -1;
    }
    fprintf(fp, "Version: 0.0.1\n");
    fprintf(fp, "Porta do servidor: %d\n", porta);
    fclose(fp);
    return 0;
}

void tratar_cliente(int cliente) {
    char html[4096];
    FILE *f = fopen("index.html", "r");
    if (f == NULL) {
        perror("Erro ao abrir index.html");
        close(cliente);
        return;
    }
    size_t lido = fread(html, 1, sizeof(html) - 1, f);
    html[lido] = '\0';
    fclose(f);

    char buffer[1024];
    read(cliente, buffer, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    printf("Recebi:\n%s\n", buffer);

    char resposta[5000];
    snprintf(resposta, sizeof(resposta),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "%s", html);

    write(cliente, resposta, strlen(resposta));
    close(cliente);
}

int iniciar_servidor(int porta, const char *versao) {
    int servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor < 0) {
        perror("Erro ao criar socket");
        return 1;
    }

    int opcao = 1;
    setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(opcao));

    struct sockaddr_in endereco;
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(porta);

    if (bind(servidor, (struct sockaddr *)&endereco, sizeof(endereco)) < 0) {
        perror("Erro no bind");
        close(servidor);
        return 1;
    }

    listen(servidor, 5);
    printf("Versão do servidor: %s\n", versao);
    printf("Servidor escutando na porta %d...\n", porta);


    while (1) {
        int cliente = accept(servidor, NULL, NULL);
        if (cliente < 0) {
            perror("Erro no accept");
            continue;
        }
        tratar_cliente(cliente);
    }

    close(servidor);
    return 0;
}

int main(void) {
    int init;

    printf("Iniciar o servidor na porta atual ou trocar?\n");
    printf("[0] Trocar [1] Iniciar: ");
    scanf("%d", &init);

    if (init == 0) {
        int porta;
        printf("Qual porta conectar: ");
        scanf("%d", &porta);
        if (salvar_porta_config(porta) < 0)
            return 1;
    }

    Config cfg = ler_config();
    if (cfg.porta <= 0)
        return 1;

    return iniciar_servidor(cfg.porta, cfg.versao);
}

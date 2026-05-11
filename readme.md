# http-server-c

Servidor HTTP simples escrito em C puro usando sockets TCP.

Projeto desenvolvido como parte do aprendizado de C — do zero, sem frameworks, sem bibliotecas externas. Só C, sockets POSIX e o protocolo HTTP na mão.

## O que faz

- Lê a porta de um arquivo de configuração (`servidor.config`)
- Abre um socket TCP e escuta conexões
- Aceita requisições HTTP do navegador
- Lê um arquivo HTML do disco e serve como resposta
- Loop contínuo — aceita múltiplas conexões sem reiniciar

## Como usar

### 1. Compile

```bash
gcc servidor.c -o servidor
```

### 2. Configure a porta

Crie o arquivo `servidor.config` com o conteúdo:

```
Porta do servidor: 8080
```

Ou rode o servidor uma vez com a opção de trocar a porta — ele gera o arquivo automaticamente.

### 3. Crie o HTML

Coloque um arquivo `index.html` na mesma pasta do executável.

### 4. Rode

```bash
./servidor
```

Acesse `http://localhost:8080` no navegador.

## Estrutura

```
.
├── servidor.c        # código do servidor
├── servidor.config   # porta configurada
└── index.html        # página servida
```

## Como funciona

```
socket()   → cria o ponto de conexão TCP
bind()     → associa à porta configurada
listen()   → começa a escutar
accept()   → aceita uma conexão do navegador
read()     → lê a requisição HTTP
fopen()    → abre o index.html
write()    → envia a resposta HTTP com o HTML
close()    → fecha a conexão e volta pro accept()
```

O protocolo HTTP é implementado manualmente — a resposta segue o formato:

```
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
\r\n
<conteúdo do arquivo>
```

## Próximos passos

- [ ] Parsear o caminho da requisição (`GET /pagina.html`)
- [ ] Servir arquivos diferentes por rota
- [ ] Resposta 404 para arquivos inexistentes
- [ ] Header `Content-Length`
- [ ] Refatorar com funções

## Motivação

Queria entender como servidores web funcionam por baixo — antes de usar o Nginx, o Apache ou qualquer framework. Esse projeto é a resposta para essa curiosidade.
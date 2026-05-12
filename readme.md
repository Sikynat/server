# http-server-c

Servidor HTTP simples escrito em C puro usando sockets TCP.

Projeto desenvolvido como parte do aprendizado de C — do zero, sem frameworks, sem bibliotecas externas. Só C, sockets POSIX e o protocolo HTTP na mão.

## O que faz

- Abre um socket TCP e escuta conexões na porta configurada
- Parseia a requisição HTTP e extrai a rota
- Serve arquivos estáticos da pasta `public/`
- Detecta o tipo do arquivo e responde com o `Content-Type` correto
- Responde `404 Not Found` para arquivos inexistentes
- Loop contínuo — aceita múltiplas conexões sem reiniciar

## Como usar

### 1. Compile

```bash
gcc servidor.c -o servidor
```

### 2. Crie os arquivos

Coloque seus arquivos HTML, CSS e JS dentro da pasta `public/`:

```
.
├── servidor.c
├── servidor
└── public/
    ├── index.html
    ├── sobre.html
    └── css/
        └── style.css
```

### 3. Rode

```bash
./servidor
```

Acesse `http://localhost:4334` no navegador.

## Como funciona

```
socket()   → cria o ponto de conexão TCP
bind()     → associa à porta 4334
listen()   → começa a escutar
accept()   → aceita uma conexão do navegador
read()     → lê a requisição HTTP
sscanf()   → extrai o método e a rota (GET /pagina.html)
fopen()    → abre o arquivo em public/
write()    → envia a resposta HTTP com o arquivo
close()    → fecha a conexão e volta pro accept()
```

## Content-Types suportados

| Extensão | Content-Type |
|----------|-------------|
| `.html` | text/html |
| `.css` | text/css |
| `.js` | application/javascript |
| `.png` | image/png |
| `.jpg` | image/jpeg |
| `.ico` | image/x-icon |

## Protocolo HTTP na mão

A resposta segue o formato obrigatório:

```
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
\r\n
<conteúdo do arquivo>
```

A linha em branco (`\r\n`) separa os headers do corpo — sem ela o navegador não sabe onde o HTML começa.

## Próximos passos

- [ ] Arquivo de configuração para porta dinâmica
- [ ] Header `Content-Length`
- [ ] Refatorar com funções
- [ ] Suporte a arquivos binários (imagens)

## Motivação

Queria entender como servidores web funcionam por baixo — antes de usar o Nginx, o Apache ou qualquer framework. Esse projeto é a resposta para essa curiosidade.
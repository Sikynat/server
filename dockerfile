FROM gcc:latest

WORKDIR /app

COPY servidor.c .
COPY public/ ./public/

RUN gcc -o servidor servidor.c

EXPOSE 4334

CMD ["./servidor"]
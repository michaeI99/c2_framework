CC = gcc
CFLAGS = -Wall -Wextra -I/opt/homebrew/opt/openssl@3/include -I/usr/local/include
LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -L/usr/local/lib
LIBS = -lcurl -lcrypto

all: agent

agent: src/main.o src/http.o src/crypto.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o agent src/main.o src/http.o src/crypto.o $(LIBS)

src/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/http.o: src/http.c src/http.h
	$(CC) $(CFLAGS) -c src/http.c -o src/http.o

src/crypto.o: src/crypto.c src/crypto.h
	$(CC) $(CFLAGS) -c src/crypto.c -o src/crypto.o

clean:
	rm -f src/*.o agent

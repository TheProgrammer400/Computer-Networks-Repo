# Basics of Server-Client

## Breakdown of server.c Code

```C
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
```

- `unistd.h` → Used for system calls like `close()`, `read()`, `write()`.
- `arpa/inet.h` → Provides networking functions (IP addresses, sockets).

***

```C
#define PORT 8080
#define BUFFER_SIZE 1024
```

- `PORT 8080` → The port where this server listens for connections (similar to phone number).
- `BUFFER_SIZE 1024` → Max size of message the server can handle at once.

***

```C
void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
```

- This function prints an error message and exits the program.

***

```C
int serverFd, clientFd;
struct sockaddr_in serverAddr, clientAddr;
socklen_t clientLen = sizeof(clientAddr);
char buffer[BUFFER_SIZE];
```

- `serverFd` → File descriptor (like an ID for the server socket).
- `clientFd` → File descriptor for the accepted client.
- `socklen_t` → Size of a socket address.
    - It is a **data type** used to store the **size of a socket address structure**.
    - Used in functions like `accept()` and `getsockname()` where you need to **pass or receive** the length of the address.
    - The size of a socket address **can vary on different systems**.
    - Instead of using `int` or `size_t`, `socklen_t` is a **special type** that makes sure it always works properly, no matter the platform.
- `sockaddr_in` → A structure storing server & client address info.
- `clientLen` → Stores size of `clientAddr` (needed for `accept()`).
- `buffer` → Used to store received messages.

***

### 1. Create The Server Socket

```C
serverFd = socket(AF_INET, SOCK_STREAM, 0);

if (serverFd == -1) {
    error("Socket failed");
}
```

- `socket(AF_INET, SOCK_STREAM, 0)`; → Creates a TCP socket.
- `AF_INET` → "Address Family: Internet"
    - This means the socket will use **IPv4** (like using a normal phone instead of a walkie-talkie).
    - If we wanted IPv6, we'd use `AF_INET6`.
- `SOCK_STREAM` → "Type of Communication"
    - This makes a **TCP socket**.
    - If we wanted a **UDP socket**, we'd have used `SOCK_DGRAM`.
- If `socket()` fails, `serverFd` will be `-1`, so we print an error.
- Significance of `0`:
    - Represents **protocol to be used** with the socket.
    - `0` tell system to **automatically pick the default protocol** for the given socket type.
    - Since we used `SOCK_STREAM`, the system **automatically selects TCP**.
    - If we had used `SOCK_DGRAM`, it would've used **UDP**.

***

### 2. Bind the Socket to an Address and Port

```C
serverAddr.sin_family = AF_INET;
serverAddr.sin_addr.s_addr = INADDR_ANY;
serverAddr.sin_port = htons(PORT);

if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
    error("Bind failed");
}
```

- `sin_family` → This tells the system **what kind of network addresses** this socket will use. (Here IPv4).
- `sin_addr.s_addr = INADDR_ANY` → Accepts connections on any IP.
- `sin_port = htons(PORT)` → Converts port number to network format.
- `bind()` → Assigns the IP & port to the socket.

***

### 3. Listen for Incoming Connections

```C
if (listen(serverFd, 5) == -1){
    error("Listen failed");
}

printf("Server is listening on port %d...\n", PORT);
```

- `listen(serverFd, 5)` → Server listens for incoming connections (max 5 clients in queue).

***

### 4. Accept a Client Connection

```C
clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);

if (clientFd == -1){
    error("Accept failed");
}
```

- `accept()` → Waits for a client to connect and returns a new socket (`clientFd`).
- If it fails, `clientFd` will be `-1`, so we show an error.

***

### 5. Receive Data from Client and Echo it Back

```C
int bytesRead = read(clientFd, buffer, BUFFER_SIZE);

if (bytesRead > 0) {
    buffer[bytesRead] = '\0';
    printf("Received: %s", buffer);
    write(clientFd, buffer, bytesRead);
}
```

- `read(clientFd, buffer, BUFFER_SIZE)` → Reads data from client into `buffer`.
- `buffer[bytesRead] = '\0'` → Adds a null character to make it a string.
- `write(clientFd, buffer, bytesRead)` → Sends the same message back to the client.

***

### 6. Close the Connection

```C
close(clientFd);
close(serverFd);
```

- `close(clientFd)` → Closes the client socket.
- `close(serverFd)` → Closes the server socket.

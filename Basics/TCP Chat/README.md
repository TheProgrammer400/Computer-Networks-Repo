# Breakdown Of TCP Chat server.c Code

```C
recv(clientFd, buffer, BUFFER_SIZE, 0);
```

`recv` stands for **receive** and used to **receive data from a socket**.

## What Happends in This Line?

- It **waits** untill data is received from the other side.
- Stores data received in `buffer`.
- If successful, it returns the **total no. of bytes received**.
- If the connection is closed, it returns `0`.
- If error occurs, it returns `-1`.

## Why Do We Need `serverFd` & `clientFd` in `server.c`, But Only `clientFd` in `client.c`?

- Server (`server.c`):
    - The **server** needs to **listen for connection** and **accept clients**.
    - `serverFd` (**Server file descriptor**)
        - This is the **main socket** that listens for connection.
        - It is **bound** to IP and **Port**.
        - It is used **only for accepting connections**, not for actual communication.
    - `cliendFd` (**Client file descriptor**)
        - Once a client connects, the server creates a **new socket** (`clientFd`).
        - This handles communication with that **specific client**.

- Client (`client.c`);
    - The client **doesn't listen for connections**—it only **connects** to the server.
    - **No need for `serverFd` or `clientFd` **because**:
        - The client **does not accept connections**, it just **connects**.
        - It only needs **one socket** (`clientFd`) to send and receive data.
        - The server's address (`serverAddr`) is used to **connect**, but the client doesn't neded its own separate `clientAddr`.

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
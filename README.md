# Socket File Transfer Program

## Description

This program allows for the transfer of files from a server directory to a client directory over a socket connection. It handles various error cases and provides feedback during the transfer process.

## Setup

### Compilation

Compile both client and server files using gcc:

```
gcc -o client client.c
gcc -o server server.c
```

### Running the Server

Navigate to the server directory and execute the server program:

```
cd <path>/server
./server
```

### Running the Client

```
./client <filename1> <filename2> <filename3> ...
```

## Transfer Process

The client sends filenames one by one to the server. The server verifies files for issues (permissions, existence, etc.). If no issues, it transfers data in chunks of 1024 bytes until the file is fully sent. Error messages are generated for the following cases:

1. File not found in the server directory.
2. Input is a directory instead of a file.
3. Read permissions are lacking for server files.
4. Write permissions are lacking for client files.
5. Attempt to overwrite an existing directory in the client.

## Termination

The server continues running after file transfers until manually stopped with `ctrl + c`. The client finishes execution upon completing file transfers.

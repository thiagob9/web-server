# web-server in c

## Description

This is a static (at the moment) web-server,  wrote entirely in C language, that supports:
1. HTML files
2. CSS files
3. Javascript files
4. JSON files 

> Warning: At the moment, this server is compatible only with Linux and only supports GET requests. 


## Installation
After you clone this repository, you will need a C/C++ compiler, like gcc or clang, and you will compile server.c.

```bash
    gcc server.c -o server
    -- or --
    clang server.c -o server 
```

After this, you will execute the file server
```
    ./server
```
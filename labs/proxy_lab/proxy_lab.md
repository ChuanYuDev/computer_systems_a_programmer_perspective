# Proxy Lab

## Table of contents
- [Introduction](#introduction)

- [Duration](#duration)

- [Chapter](#chapter)

- [Solution](#solution)
    - [Part I: implementing a sequential web proxy](#part-i-implementing-a-sequential-web-proxy)
    - [Part II: dealing with multiple concurrent requests](#part-ii-dealing-with-multiple-concurrent-requests)
    - [Part III: caching web objects](#part-iii-caching-web-objects)
    - [TO DO](#to-do)

## Introduction
- Students implement a concurrent caching Web proxy that sits between their browser and the rest of the World Wide Web
- This lab exposes students to the interesting world of network programming, and ties together many of the concepts from the course, such as byte ordering, caching, process control, signals, signal handling, file I/O, concurrency, and synchronization

## Duration
- 2025/5/21 - 2025/6/4

## Chapter
- Chapter 10: System-Level I/O
- Chapter 11: Network Programming
- Chapter 12: Concurrent Programming

## Solution
### Part I: implementing a sequential web proxy
- Basic workflow

    ![](./images/basic_workflow.png)
    1. Listen for incoming connections
        - Read the entirety of the request from the client and parse the request
        - Varify the HTTP request

    2. Establish connection to web server and request object the client specified
    3. Read the server's response
    4. Forward it to the client

- Error-handling functions modification
    - Temporarily remove `exit()`
    1. Original function
    1. Add error message, return original rc
    1. Application level, based on rc, do some actions

- Result:

    ```
    *** Basic ***
    Starting tiny on 15021
    Starting proxy on 24858
    1: home.html
    Fetching ./tiny/home.html into ./.proxy using the proxy
    Fetching ./tiny/home.html into ./.noproxy directly from Tiny
    Comparing the two files
    Success: Files are identical.
    2: csapp.c
    Fetching ./tiny/csapp.c into ./.proxy using the proxy
    Fetching ./tiny/csapp.c into ./.noproxy directly from Tiny
    Comparing the two files
    Success: Files are identical.
    3: tiny.c
    Fetching ./tiny/tiny.c into ./.proxy using the proxy
    Fetching ./tiny/tiny.c into ./.noproxy directly from Tiny
    Comparing the two files
    Success: Files are identical.
    4: godzilla.jpg
    Fetching ./tiny/godzilla.jpg into ./.proxy using the proxy
    Fetching ./tiny/godzilla.jpg into ./.noproxy directly from Tiny
    Comparing the two files
    Success: Files are identical.
    5: tiny
    Fetching ./tiny/tiny into ./.proxy using the proxy
    Fetching ./tiny/tiny into ./.noproxy directly from Tiny
    Comparing the two files
    Success: Files are identical.
    Killing tiny and proxy
    basicScore: 40/40
    ```

### Part II: dealing with multiple concurrent requests
- Use prethreading to deal with multiple concurrent requests

- Result:

    ```
    *** Concurrency ***
    Starting tiny on port 28477
    Starting proxy on port 17211
    Starting the blocking NOP server on port 11448
    Trying to fetch a file from the blocking nop-server
    Fetching ./tiny/home.html into ./.noproxy directly from Tiny
    Fetching ./tiny/home.html into ./.proxy using the proxy
    Checking whether the proxy fetch succeeded
    Success: Was able to fetch tiny/home.html from the proxy.
    Killing tiny, proxy, and nop-server
    concurrencyScore: 15/15
    ```

### Part III: caching web objects

- Features:
    - The maximum amount of data your proxy will ever use is:

        ```
        MAX_CACHE_SIZE + T * MAX_OBJECT_SIZE
        ```
        - This means the maximum cache size is still `MAX_CACHE_SIZE`
        - At the same time, there are `T` actice connections and they accumulate `MAX_OBJECT_SIZE` bytes data in the buffer

    - First readers-writers problem
    - Use `{hostname}:{port}{path}` as key
    - Cache structure:

        ```c
        typedef struct
        {
            cache_t *next;
        } cache_t;
        ```

    - Object structure

        ```c
        typedef struct
        {
            char *
            object_t *next;
        } object_t;
        ```

- Logic:
    - Multiple readers can access the cache (cache unchanged)
    - After read, return pointer to the item

    - If pointer == NULL (No cache)
        - `malloc` `MAX_OBJECT_SIZE` buffer `object_buf`
        - Connect to server

        - Read `MAXBUF` object into `buf`
            - Serve object to client

            - If `read_bytes` + `object_size` > `MAX_OBJECT_SIZE`
                - Discard buffer (`free`)
                - Abort cache process

            - If `read_bytes` + `object_size` <= `MAX_OBJECT_SIZE`
                - Append `buf` to `object_buf`

                    ```
                    memcpy(object_buf + object size, buf, read_bytes)
                    ```
                
                - Repeat read until EOF

        - If cache size + object size <= `MAX_CACHE_SIZE`
            - Store object into cache
        
        - If cache size + object size > `MAX_CACHE_SIZE`
            - Evict some the least used objects until size <= `MAX_CACHE_SIZE`
            - Store object into cache
    
    - If point != NULL (In cache)
        - Get cached object
        - Change cache organization to implement LRU
        - Serve object to client

### TO DO:
- Error handling
    - If `P(v)` error, what should I do?
    - If `Pthread_detach` error, what should I do?
    - Terminate thread?

- Proxy ignore SIGPIPE signal (page 964)?
    - `write` return EPIPE errors?

- Prematurely close, `read` turn -1 with `errno` set to `ECONNRESET`
    - Proxy should not terminate due to this error

- Tiny error: Rio_writen error: Connection reset by peer?
    - Only appear once?

- `line:netp:doit:parserequest` (`doit` in tiny.c) note meaning?
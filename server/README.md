# Server app

## Building
Use `make build`

## Run unittests (future)
Use `make tests`. Require `boost` >= `1.59`

## How to test manually/connect
Run `server.out`, it's should produce output like

    ./server.out
    Server runs on localhost:44444

Where `44444` is a local port. You can connect to server with any TCP client, e.g. netcat. For netcat, command is `nc localhost 44444`. Let's type & try.

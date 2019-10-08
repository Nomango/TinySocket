# TinySocket

A tiny sample of windows socket

## Usage

### How to build a server

**1. Include header files of TinySocket**

```cpp
#include <tinysocket.h>
using namespace tinysocket;
```

**2. Create a server object and start to listen a specific port**

```cpp
Server server;
server.Listen(Protocol::TCP, Address::LocalHost(1234));
```

Protocol is an enumeration type, which contains `UDP` and `TCP` two protocols.

**3. Accept a connection from clients and then read messages**

```cpp
Connection conn = server.Accept();      // Accept a connection
std::string received = conn.Receive();  // Read a message from the client
```

**4. Send responses to clients**

```cpp
conn.Send("Hello!");
```

### How to build a client

**1. Include header files of TinySocket**

```cpp
#include <tinysocket.h>
using namespace tinysocket;
```

**2. Create a client object and connect to the server**

```cpp
Client client;
client.Connect(Protocol::TCP, Address::LocalHost(1234));
```

**3. Send messages to server and receive responses**

```cpp
client.Send("Hello world");
std::string received = client.Receive();
```

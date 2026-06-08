# Presentation Script: Q5 - TCP Multi-Node Concurrency (Digital Library System)

## Introduction
Welcome to the final component of my summative portfolio presentation. Thus far, we have discussed single-system threads and code logic, but here we extrapolate upwards into Network Architectures. The objective of Project 5 was to design and construct a fully distributed concurrent Digital Library authentication and reservation architecture spanning over a TCP Client-Server model.

The challenge was quite distinct: we needed to boot a central server on a local port and enable it to asynchronously service requests from multiple disparate custom network terminal clients simultaneously. Each client logs in via specific hardcoded credentials and competes to access digital resources—all while maintaining absolute synchronization integrity on the shared central backend.

## Key Concepts: Network Sockets & Server Threading
For data to transport between isolated terminals, we utilized POSIX networking logic based around standard `socket` instantiations. 
For the server, our application creates an endpoint bounded to TCP port `8080`, initializing its connection status to `LISTEN`.

Crucially, standard servers sit in an endless loop blocking at the `accept()` function. When a new client initiates a connection, the central server receives a unique, isolated File Descriptor addressing this particular client. If our server processed client payloads purely sequentially, Client B would freeze waiting for Client A to finish interacting. 

To achieve a true concurrent architecture, I blended my multithreading skills directly into the TCP infrastructure. Upon completing the `accept()` handshake, my server dynamically instantiates a new unique thread parameter dedicated solely to handling the inbound network packets for that unique user space via `recv()` logic. This allows a hundred clients to comfortably connect simultaneously, maintaining independent conversational states with the server process.

## Code Specifics: Authentication & Race Conditions
The interactive protocol defines two core functionalities: Auth and Reserve. 
When a client application starts, it prompts for an authentication ID. The buffer is shipped across the socket. The dedicated server thread evaluates string boundaries, verifying the ID against the accepted list—specifically strings like `ALU123` or `ALU456`. If the buffer check passes, it responds with an approval byte, allowing the client interface into the library reservation phase. 

The reservation protocol introduces a massive potential structural data hazard: the library database state. Imagine Client A essentially fires a request `RESERVE:1`, and simultaneously Client B requests exactly `RESERVE:1`. Because unique client-spawning thread limits are processing concurrently, they might both theoretically witness that Book 1 is available and issue a success packet. 

To protect the Central Shared Resources, the database lookup requires strict Thread Synchronization limits natively in C. My server wraps the entire transactional check with a database-level Mutex. Whichever Thread accesses the reservation logic first secures the Mutex, claims Book 1 thereby modifying its availability byte, and releases the Mutex. The subsequent trailing Thread is safely permitted entrance, witnesses the newly updated array state, correctly assesses that Book 1 is missing, and explicitly sends back a `RES_REJECTED` string, entirely preventing a reservation conflict. 

## Conclusion
In summary, the TCP Digital Library System project perfectly unites POSIX socket programming with complex threaded concurrency state management. Running the models locally clearly matched the required sample logs, verifying my ability to write servers capable of isolating infinite disparate connections whilst managing safe backend memory configurations. 

This completes my summative portfolio walkthrough covering Reverse Engineering, x86 Assembly parsing, Python-C Interoperability, Threading, and Systems Networking. Thank you so much for listening—I am very happy to answer any questions.

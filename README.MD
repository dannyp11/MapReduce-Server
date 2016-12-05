Author: Dat Pham

A simple model of computational offloading where a single client offloads some computation
to a server which in turn distributes the load over 3 backend servers. The server facing the 
client then collects the results from thebackend and communicates the same to the client in 
the required format. This is anexample of how a cloud­computing service such Amazon Web Services 
might implement MapReduce to speed up a large computation task offloaded by the client.

![Alt text](https://github.com/dannyp11/MapReduce-Server/blob/master/networkIllustration.png?raw=true "Illustration of the network")

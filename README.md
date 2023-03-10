# Router Dataplane

## 📡 What is this?

This is a C program that implements the IPv4 protocol present in the [forwarding plane](https://en.wikipedia.org/wiki/Forwarding_plane) of a router.

It does this using standard C libraries for working with sockets, such as `<sys/socket.h>`. I have worked with sockets previously during a lab.

I worked on this during April 2022 and in doing so I learned a great deal about routing.

## 💻 How to run it?

1.  Clone this repository.
2.  Run the automated testing script using `sudo python3 topo.py` (I did not make the script myself).
3.  Enjoy!

## 📶 What does it do?

The code I wrote is in `router.c`.

What it does is following the next algorithm:

```
1.  Extract the Ethernet header.
2.  Parse the next header as it were for IPv4.
3.  Check the checksum and the TTL.
4.  Update TTL.
5.  Search the IP inside the routing table.
6.  Update checksum.
7.  Rewrite addresses inside the ARP table.
8.  Send the new package on the corresponding interface.
```

## 🏋️ Was this easy or hard?

Harder than it looks.

Firstly, figuring the steps in the algorithm was a challenge on its own, as many resources only explain them theoretically, without getting too much into the specifics.

Secondly, not having a good way to know whether you made some progress or not is very, very tough. Remember that writing a program like this leaves you with very little room to debug, other than running the routers manually and testing their functionalities using tcpdump, which can be a long and daunting task.

![image](https://user-images.githubusercontent.com/74200913/224244718-b450ba63-96d1-49c4-9d37-c056d5a332bd.png)

However, what's important is that, eventually, I did make it.

## 🤔 Did you know?

Sending and updating the checksum in IPv4 in the entire world consumes more energy per day, every day, than all the flights combined.

So, instead of focusing on reducing the number of flights, one would argue that we should try to move towards an IPv6-powered Internet instead. 🤓

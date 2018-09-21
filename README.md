# MITM binary null app for SSLProxy

Using
-----

Apply the patch from sslproxy.diff

Edit the /usr/include/commoncpp/socket.h and make Socket::readData & Socket::writeData public.

Compile & install SSLProxy

Create the MITM entry in sslproxy.conf:

```
User nobody
Group nobody
ProxySpec bssl 127.0.0.1 8443 up:8080
```

Run SSLProxy

Create some network redirect, for ex.:

```
iptables -t nat -A OUTPUT -p tcp --dport 443 -m owner ! --uid-owner nobody -j DNAT --to 127.0.0.1:8443
```

Compile & run mitmapp as

```
./bmitm 8080
```

Run the sample app, for ex.:

```
curl --compressed -L -k https://google.com
```

Enjoy !

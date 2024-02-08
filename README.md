# DNS_Serv

to run the program:

"make" or "make all"

run the executable:

./dns_server

in split window run the command:

dig @127.0.0.1 -p 8080 <domainname>

example:

dig @127.0.0.1 -p 8080 facebook.com

Resolved reply:

; <<>> DiG 9.10.6 <<>> @127.0.0.1 -p 8080 instagram.com
; (1 server found)
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 65327
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 512
;; QUESTION SECTION:
;instagram.com.                 IN      A

;; ANSWER SECTION:
instagram.com.          60      IN      A       157.240.15.174

;; Query time: 196 msec
;; SERVER: 127.0.0.1#8080(127.0.0.1)
;; WHEN: Thu Feb 08 14:20:59 +04 2024
;; MSG SIZE  rcvd: 58

blacklisted reply:

 <<>> DiG 9.10.6 <<>> @127.0.0.1 -p 8080 facebook.com
; (1 server found)
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: REFUSED, id: 21006
;; flags: qr rd ad; QUERY: 1, ANSWER: 0, AUTHORITY: 0, ADDITIONAL: 1
;; WARNING: recursion requested but not available

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 4096
;; QUESTION SECTION:
;facebook.com.                  IN      A

;; Query time: 0 msec
;; SERVER: 127.0.0.1#8080(127.0.0.1)
;; WHEN: Thu Feb 08 14:19:44 +04 2024
;; MSG SIZE  rcvd: 41

fee free to add more domains to the blacklist, please keep them in one line. Max limit is 10
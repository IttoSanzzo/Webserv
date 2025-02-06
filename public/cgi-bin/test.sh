#!/bin/bash
printf "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection: close\r\n\r\n"
printf "<html><head><title>Hello World</title></head><body><h1>Hello World</h1></body></html>"

# exit 1
import sys

print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection: close\r\n\r\n");
print("<html><head><title>Hello World</title></head><body><h1>Hello " + sys.argv[3] + " World</h1></body></html>");
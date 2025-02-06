import sys

if (sys.argv[1] == ""):
	print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection: close\r\nSet-Cookie: name=Chariot\r\n\r\n");
	print("<html><head><title>Hello World</title></head><body><h1>Missing Cookie</h1></body></html>");
	sys.exit()

print("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nConnection: close\r\n\r\n");
name = sys.argv[1][5:]
print("<html><head><title>Hello World</title></head><body><h1>Hello " + name + "</h1></body></html>");
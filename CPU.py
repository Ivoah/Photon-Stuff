import psutil
import socketserver

class HTTPServer(socketserver.BaseRequestHandler):
    def handle(self):
        cpu = psutil.cpu_percent()
        print(int(cpu/100*127))
        self.request.sendall(bytes([int(cpu/100*127)]))

socketserver.TCPServer.allow_reuse_address = True
server = socketserver.TCPServer(('0.0.0.0', 1337), HTTPServer)
try:
    server.serve_forever()
except KeyboardInterrupt as e:
    print('\nQuitting:', repr(e))
    server.server_close()

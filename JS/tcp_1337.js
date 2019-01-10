const net = require('net');

const server = net.createServer(stream => {
  stream.setEncoding('utf8');

  stream.write('hello\r\n');

  stream.addListener('data', data => {
    this.write(data);
  });

  stream.addListener('end', () => {
    this.write('goodbye\r\n');
    this.end();
  });
});

server.listen(1337, 'localhost');

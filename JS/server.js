// Загружаем модуль http
const http = require('http');

// Создаем web-сервер с обработчиком запросов
const server = http.createServer((req, res) => {
  console.log('Начало обработки запроса');
  // Передаем код ответа и http-заголовки
  res.writeHead(200, {
    'Content-Type': 'text/plain; charset=UTF-8'
  });
  res.end('Hello world!');
});

// Запускаем web-сервер
server.listen(2002, '127.0.0.1', () => {
  console.log('Сервер запущен http://127.0.0.1:2002/');
});

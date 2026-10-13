FROM gcc:12-bullseye

WORKDIR /app

COPY . /app

RUN g++ -std=c++17 -pthread -O3 main.cpp -o cache_server

EXPOSE 8080

CMD ["./cache_server"]

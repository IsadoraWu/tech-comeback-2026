FROM gcc:14

WORKDIR /app

COPY Makefile ./
COPY header/ ./header/
COPY src/ ./src/

RUN make server

EXPOSE 12345

CMD ["./bin/server"]
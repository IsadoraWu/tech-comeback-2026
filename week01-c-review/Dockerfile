FROM gcc:14
WORKDIR /app
COPY main.c linked_list.c linked_list.h ./
RUN gcc -Wall -Wextra -g main.c linked_list.c -o linked_list
CMD ["./linked_list"]
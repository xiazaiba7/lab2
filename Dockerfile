FROM gcc:10.2
WORKDIR /app/
COPY lab2.cpp ./
RUN g++ lab2.cpp -o lab2
RUN chmod +x lab2

FROM debian:latest

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libpcap-dev \
    iproute2 \
    iputils-ping \
    net-tools \
    sudo

COPY . /ft_malcolm
WORKDIR /ft_malcolm

RUN make -C libft
RUN make

CMD ["bash"]

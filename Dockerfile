FROM scapicryptobiu/libscapi:latest

RUN apt-get update
RUN apt-get install liblog4cpp5-dev libevent-dev -y

WORKDIR /root

RUN wget http://www.shoup.net/ntl/ntl-9.10.0.tar.gz
RUN tar xf ntl-9.10.0.tar.gz
WORKDIR /root/ntl-9.10.0/src
RUN ./configure WIZARD=off
RUN make

WORKDIR /root

COPY . HyperMPC

# compile ACP
RUN cp -r /root/HyperMPC/ACP /root/ACP
WORKDIR /root/ACP/comm_client
RUN cmake .
RUN make

WORKDIR ../cct_proxy
RUN cmake .
RUN make

WORKDIR /root/HyperMPC
RUN cmake .
RUN make
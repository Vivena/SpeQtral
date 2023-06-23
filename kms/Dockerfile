# Use an official image of Ubuntu as the parent image
FROM ubuntu:latest

# Set the working directory to /app
WORKDIR /app

# Update the package lists and install necessary packages
RUN apt-get update && \
    apt-get install -y cmake git build-essential libssl-dev && \
    apt-get install -y libyaml-cpp-dev && \
    apt-get install libjsoncpp-dev


# Clone the oatpp repository and build it
RUN git clone https://github.com/oatpp/oatpp.git && \
    cd oatpp && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make install

# Clone the oatpp-swagger repository and build it
RUN git clone https://github.com/oatpp/oatpp-swagger &&\
    cd oatpp-swagger &&\
    mkdir build &&\
    cd build &&\
    cmake .. && \
    make install

# Copy your oatpp application code into container
COPY . /app/

# Define environment variables for port numbers
ENV PORT1 8000
ENV PORT2 8001
ENV PORT3 8002

# Expose three ports based on environment variables
EXPOSE $PORT1 $PORT2 $PORT3

# Start the oatpp server on all three specified ports
CMD ["./my-oatpp-app-exe", "--server-port", "$PORT1,$PORT2,$PORT3"]


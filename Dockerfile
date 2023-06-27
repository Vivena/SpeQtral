FROM ubuntu:latest

# Update the apt package index
RUN apt-get update

# Install Java 8 JDK
RUN apt-get install -y openjdk-8-jdk

# Install Maven
RUN apt-get install -y maven

# Install Screen
RUN apt-get install -y screen

# Install Git
RUN apt-get install -y git

# Install Curl
RUN apt-get install -y curl

# Install jq
RUN apt-get install -y jq

# Define environment variables for Java and Maven
ENV JAVA_HOME /usr/lib/jvm/java-8-openjdk-amd64
ENV MAVEN_HOME /usr/share/maven

# Add Java and Maven to PATH
ENV PATH $PATH:$JAVA_HOME/bin:$MAVEN_HOME/bin

# Set the working directory to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
RUN git clone https://github.com/Open-QKD-Network/qkd-net.git

RUN cd qkd-net/kms && ./scripts/build

RUN rm -rf ~/.qkd 

# Copy the build-and-run script from ./scripts folder to the container at /app
COPY ./scripts/build-and-run.sh /app

WORKDIR /app/qkd-net/kms

# Define the entry point script
ENTRYPOINT ["/bin/bash","./scripts/run"]

# Define the default command to run when the container starts
# CMD ["/kms/scripts/build-and-run.sh"]


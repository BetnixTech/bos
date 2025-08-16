# Use a lightweight base image, Alpine is a small Linux distribution
FROM alpine:latest

# Update packages and install compilers and interpreters
# The --no-cache flag keeps the image small
RUN apk update && apk add --no-cache \
    g++ \
    gcc \
    openjdk17-jre \
    rustc \
    nasm \
    git \
    python3 \
    py3-pip \
    bash \
    ld-linux.so.2 \
    musl-dev \
    libc6-compat

# Install Python libraries
RUN pip install flask psutil

# Create a directory for your application inside the container
WORKDIR /app

# Copy all your project files from your computer into the container
COPY . /app

# Expose the port that your Flask app will run on
EXPOSE 5000

# Set the command to run your server when the container starts
CMD ["python3", "server.py"]

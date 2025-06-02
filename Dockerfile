# Use Windows base image compatible with Unreal
FROM mcr.microsoft.com/windows/servercore:ltsc2022

LABEL maintainer="Hexademic"
LABEL version="5.3.2"
LABEL description="Custom Unreal Engine 5.3.2 environment for Hexademic Consciousness Engine builds"

SHELL ["powershell", "-Command"]

# Set working directory
WORKDIR C:\\Unreal

# Copy Unreal Engine installation from host machine into image
# IMPORTANT: You must copy the engine directory manually
# This line assumes you already have the engine copied during docker build
COPY UE_5.3 C:/Unreal/UE_5.3

# Set environment variables
ENV UE_DIR="C:\\Unreal\\UE_5.3"
ENV PATH="${UE_DIR}\\Engine\\Binaries\\Win64;${PATH}"

# Optional: Preinstall build dependencies (MSVC, Python, etc.)
# Install Chocolatey and additional packages
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; \
    iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1')); \
    choco install -y python3 git 7zip

# Health check (optional)
HEALTHCHECK CMD [ "cmd", "/c", "echo Engine at %UE_DIR% && dir %UE_DIR%" ]

# Default shell
CMD ["powershell.exe"]

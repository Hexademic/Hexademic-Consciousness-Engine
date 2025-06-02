# Windows base (adjust for your environment)
FROM mcr.microsoft.com/windows/servercore:ltsc2022

LABEL maintainer="Hexademic"
LABEL version="5.3.2"
LABEL description="Custom Unreal Engine 5.3.2 container for Hexademic builds"

# Set PowerShell as the default shell
SHELL ["powershell", "-Command"]

# Create working directory
WORKDIR C:\Unreal

# (Manually copy your installed UE_5.3 directory here before building)
COPY UE_5.3 C:/Unreal/UE_5.3

# Add Engine binaries to PATH
ENV UE_DIR="C:\\Unreal\\UE_5.3"
ENV PATH="${UE_DIR}\\Engine\\Binaries\\Win64;${PATH}"

# Install useful tools
RUN iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1')) ; \
    choco install -y python git

CMD ["powershell"]

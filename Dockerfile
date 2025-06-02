# Use Epic Games' Windows UE base image (ensure EULA accepted)
FROM ghcr.io/epicgames/unreal-engine:5.3.2-windows

# Set working directory inside container
WORKDIR /workspace

# Copy Unreal project files into container
COPY . .

# Optional: define build command
# Replace with your actual Unreal project name
RUN "C:\Program Files\Epic Games\UE_5.3\Engine\Build\BatchFiles\Build.bat" MyProjectEditor Win64 Development

CMD ["powershell"]

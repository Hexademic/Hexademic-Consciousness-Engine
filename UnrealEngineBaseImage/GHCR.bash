# Build image
docker build -t ghcr.io/hexademic/unreal-engine:5.3.2-custom .

# Login
echo $YOUR_PAT | docker login ghcr.io -u yourusername --password-stdin

# Push
docker push ghcr.io/hexademic/unreal-engine:5.3.2-custom

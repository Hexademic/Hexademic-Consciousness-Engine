jobs:
  build:
    runs-on: windows-latest

    container:
      image: ghcr.io/hexademic/unreal-engine:5.3.2-custom
      options: --isolation=process  # optional for Windows

    steps:
      - uses: actions/checkout@v4

      - name: Build project inside Unreal container
        shell: powershell
        run: |
          Build.bat HexademicEditor Win64 Development `
            -Project="D:\a\Hexademic-Consciousness-Engine\Hexademic-Consciousness-Engine\Hexademic.uproject" `
            -WaitMutex -FromMsBuild

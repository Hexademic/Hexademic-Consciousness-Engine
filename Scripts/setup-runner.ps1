# Location: Scripts/setup-runner.ps1
# PowerShell script to set up self-hosted runner for Hexademic Consciousness Engine

param(
    [string]$GitHubToken = "",
    [string]$RunnerName = "hexademic-builder-01",
    [string]$UEInstallPath = "C:\Program Files\Epic Games\UE_5.3"
)

Write-Host "🚀 Setting up GitHub Actions Runner for Hexademic Consciousness Engine" -ForegroundColor Green

# Check if running as administrator
if (-NOT ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Error "❌ This script must be run as Administrator!"
    exit 1
}

# Validate parameters
if ([string]::IsNullOrEmpty($GitHubToken)) {
    Write-Error "❌ GitHub token is required! Use -GitHubToken parameter"
    exit 1
}

# Create runner directory
$runnerPath = "C:\actions-runner"
Write-Host "📁 Creating runner directory at $runnerPath"
if (!(Test-Path $runnerPath)) {
    New-Item -ItemType Directory -Path $runnerPath -Force
}
Set-Location $runnerPath

# Download latest runner
Write-Host "⬇️ Downloading GitHub Actions Runner..."
$latestUrl = "https://github.com/actions/runner/releases/download/v2.311.0/actions-runner-win-x64-2.311.0.zip"
$zipFile = "actions-runner-win-x64.zip"

try {
    Invoke-WebRequest -Uri $latestUrl -OutFile $zipFile
    Write-Host "✅ Downloaded runner package"
} catch {
    Write-Error "❌ Failed to download runner: $($_.Exception.Message)"
    exit 1
}

# Extract runner
Write-Host "📦 Extracting runner..."
try {
    Add-Type -AssemblyName System.IO.Compression.FileSystem
    [System.IO.Compression.ZipFile]::ExtractToDirectory("$runnerPath\$zipFile", $runnerPath)
    Remove-Item $zipFile
    Write-Host "✅ Runner extracted"
} catch {
    Write-Error "❌ Failed to extract runner: $($_.Exception.Message)"
    exit 1
}

# Configure runner
Write-Host "⚙️ Configuring runner..."
try {
    $configArgs = @(
        "--url", "https://github.com/Hexademic/Hexademic-Consciousness-Engine",
        "--token", $GitHubToken,
        "--name", $RunnerName,
        "--labels", "unreal-engine,ue5,hexademic",
        "--work", "_work",
        "--replace"
    )
    
    & .\config.cmd @configArgs
    Write-Host "✅ Runner configured"
} catch {
    Write-Error "❌ Failed to configure runner: $($_.Exception.Message)"
    exit 1
}

# Verify Unreal Engine installation
Write-Host "🎮 Verifying Unreal Engine installation..."
if (Test-Path "$UEInstallPath\Engine\Build\BatchFiles\Build.bat") {
    Write-Host "✅ Unreal Engine found at $UEInstallPath"
} else {
    Write-Warning "⚠️ Unreal Engine not found at $UEInstallPath"
    Write-Host "Please ensure UE 5.3 is installed before running builds"
}

# Install as service
Write-Host "🔧 Installing runner as Windows service..."
try {
    & .\svc.cmd install
    & .\svc.cmd start
    Write-Host "✅ Runner service installed and started"
} catch {
    Write-Error "❌ Failed to install service: $($_.Exception.Message)"
    exit 1
}

# Verify service status
$serviceStatus = & .\svc.cmd status
Write-Host "Service Status: $serviceStatus"

Write-Host ""
Write-Host "🎉 Setup completed successfully!" -ForegroundColor Green
Write-Host "Runner '$RunnerName' is now registered and running"
Write-Host "Location: $runnerPath"
Write-Host ""
Write-Host "Next steps:"
Write-Host "1. Verify the runner appears in your GitHub repository settings"
Write-Host "2. Test with a simple workflow run"
Write-Host "3. Monitor logs at: $runnerPath\_diag\"

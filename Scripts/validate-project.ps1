# Location: Scripts/validate-project.ps1
# Standalone project validation script for Hexademic Consciousness Engine

param(
    [string]$ProjectPath = (Get-Location).Path,
    [switch]$Detailed = $false
)

Write-Host "🔍 Validating Hexademic Consciousness Engine Project" -ForegroundColor Cyan
Write-Host "Project Path: $ProjectPath" -ForegroundColor Gray
Write-Host ""

# Change to project directory
Set-Location $ProjectPath

# Initialize validation results
$validationResults = @{
    ProjectFile = $null
    HasSource = $false
    HasContent = $false
    HasConfig = $false
    EngineVersion = $null
    Modules = @()
    Issues = @()
    Warnings = @()
}

# Function to add validation issue
function Add-Issue {
    param([string]$Message, [string]$Type = "Error")
    
    if ($Type -eq "Warning") {
        $validationResults.Warnings += $Message
        Write-Host "⚠️ $Message" -ForegroundColor Yellow
    } else {
        $validationResults.Issues += $Message
        Write-Host "❌ $Message" -ForegroundColor Red
    }
}

# Function to log success
function Log-Success {
    param([string]$Message)
    Write-Host "✅ $Message" -ForegroundColor Green
}

# 1. Find .uproject file
Write-Host "📋 Step 1: Finding project file..." -ForegroundColor Blue
$projectFiles = Get-ChildItem -Name "*.uproject" -Recurse

if ($projectFiles.Count -eq 0) {
    Add-Issue "No .uproject file found in $ProjectPath"
} elseif ($projectFiles.Count -gt 1) {
    $fileList = $projectFiles -join ', '
    Add-Issue "Multiple .uproject files found: $fileList" "Warning"
    $validationResults.ProjectFile = $projectFiles[0]
    Log-Success "Using first found: $($projectFiles[0])"
} else {
    $validationResults.ProjectFile = $projectFiles[0]
    Log-Success "Found project file: $($projectFiles[0])"
}

# 2. Validate directory structure
Write-Host ""
Write-Host "📂 Step 2: Validating directory structure..." -ForegroundColor Blue

$requiredDirs = @{
    "Source" = "Contains C++ source code and modules"
    "Content" = "Contains game assets and blueprints"
    "Config" = "Contains configuration files"
}

foreach ($dir in $requiredDirs.Keys) {
    if (Test-Path $dir) {
        $validationResults.$("Has$dir") = $true
        Log-Success "$dir directory found - $($requiredDirs[$dir])"
        
        if ($Detailed -and $dir -eq "Source") {
            $sourceFiles = Get-ChildItem -Path $dir -Recurse -Include "*.cpp", "*.h" | Measure-Object
            Write-Host "  - Found $($sourceFiles.Count) C++ files" -ForegroundColor Gray
        }
    } else {
        if ($dir -eq "Source") {
            Add-Issue "$dir directory missing - $($requiredDirs[$dir])"
        } else {
            Add-Issue "$dir directory missing - $($requiredDirs[$dir])" "Warning"
        }
    }
}

# 3. Validate project file content
if ($validationResults.ProjectFile) {
    Write-Host ""
    Write-Host "📄 Step 3: Validating project file content..." -ForegroundColor Blue
    
    try {
        $projectContent = Get-Content $validationResults.ProjectFile -Raw | ConvertFrom-Json
        Log-Success "Project file is valid JSON"
        
        # Engine version
        if ($projectContent.EngineAssociation) {
            $validationResults.EngineVersion = $projectContent.EngineAssociation
            Log-Success "Engine version: $($projectContent.EngineAssociation)"
            
            # Check version compatibility
            $supportedVersions = @("4.27", "5.0", "5.1", "5.2", "5.3", "5.4")
            if ($projectContent.EngineAssociation -notin $supportedVersions) {
                Add-Issue "Engine version $($projectContent.EngineAssociation) may not be fully supported" "Warning"
            }
        } else {
            Add-Issue "No engine association found in project file" "Warning"
        }
        
        # Modules
        if ($projectContent.Modules -and $projectContent.Modules.Count -gt 0) {
            $validationResults.Modules = $projectContent.Modules
            Log-Success "Found $($projectContent.Modules.Count) module(s):"
            
            foreach ($module in $projectContent.Modules) {
                Write-Host "  - $($module.Name) ($($module.Type))" -ForegroundColor Gray
                
                # Validate module source exists
                if ($validationResults.HasSource) {
                    $moduleDir = Join-Path "Source" $module.Name
                    if (!(Test-Path $moduleDir)) {
                        Add-Issue "Module directory missing: $moduleDir"
                    }
                }
            }
        } else {
            Add-Issue "No modules found - this might be a Blueprint-only project" "Warning"
        }
        
        # Additional project properties
        if ($Detailed) {
            Write-Host ""
            Write-Host "📊 Additional Project Details:" -ForegroundColor Blue
            
            if ($projectContent.Description) {
                Write-Host "  - Description: $($projectContent.Description)" -ForegroundColor Gray
            }
            
            if ($projectContent.Category) {
                Write-Host "  - Category: $($projectContent.Category)" -ForegroundColor Gray
            }
            
            if ($projectContent.Plugins) {
                Write-Host "  - Plugins: $($projectContent.Plugins.Count)" -ForegroundColor Gray
            }
        }
        
    } catch {
        Add-Issue "Project file JSON validation failed: $($_.Exception.Message)"
    }
}

# 4. Check for common Unreal Engine files
Write-Host ""
Write-Host "🔧 Step 4: Checking for additional UE files..." -ForegroundColor Blue

$ueFiles = @{
    "*.sln" = "Visual Studio solution file"
    "Binaries" = "Compiled binaries directory"
    "Intermediate" = "Intermediate build files directory"
    "Saved" = "Saved files directory"
}

foreach ($pattern in $ueFiles.Keys) {
    if ($pattern.Contains("*")) {
        $found = Get-ChildItem -Name $pattern | Measure-Object
        if ($found.Count -gt 0) {
            Log-Success "Found $($ueFiles[$pattern])"
        }
    } else {
        if (Test-Path $pattern) {
            Log-Success "Found $($ueFiles[$pattern])"
        }
    }
}

# 5. Generate summary report
Write-Host ""
Write-Host "📊 Validation Summary" -ForegroundColor Magenta
Write-Host "===================" -ForegroundColor Magenta

if ($validationResults.Issues.Count -eq 0) {
    Write-Host "✅ Project validation PASSED" -ForegroundColor Green
    $exitCode = 0
} else {
    Write-Host "❌ Project validation FAILED" -ForegroundColor Red
    $exitCode = 1
}

Write-Host ""
Write-Host "Issues: $($validationResults.Issues.Count)" -ForegroundColor $(if ($validationResults.Issues.Count -eq 0) { "Green" } else { "Red" })
Write-Host "Warnings: $($validationResults.Warnings.Count)" -ForegroundColor $(if ($validationResults.Warnings.Count -eq 0) { "Green" } else { "Yellow" })

if ($validationResults.Issues.Count -gt 0) {
    Write-Host ""
    Write-Host "Critical Issues:" -ForegroundColor Red
    foreach ($issue in $validationResults.Issues) {
        Write-Host "  • $issue" -ForegroundColor Red
    }
}

if ($validationResults.Warnings.Count -gt 0) {
    Write-Host ""
    Write-Host "Warnings:" -ForegroundColor Yellow
    foreach ($warning in $validationResults.Warnings) {
        Write-Host "  • $warning" -ForegroundColor Yellow
    }
}

# Export results to JSON (optional)
if ($Detailed) {
    $resultsFile = "validation-results.json"
    $validationResults | ConvertTo-Json -Depth 3 | Out-File -FilePath $resultsFile -Encoding UTF8
    Write-Host ""
    Write-Host "📄 Detailed results saved to: $resultsFile" -ForegroundColor Gray
}

Write-Host ""
Write-Host "🎮 Hexademic Consciousness Engine validation complete!" -ForegroundColor Cyan

exit $exitCode

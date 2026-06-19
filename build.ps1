param(
    [string]$LLVMDir = "C:\libs\build\lib\cmake\llvm",
    [string]$BuildDir = "build"
)

# Locate vswhere — ships with every VS 2017+ installation
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    Write-Error "vswhere.exe not found. Install Visual Studio 2017 or later."
    exit 1
}

# Find the latest VS installation that has the x64 C++ toolset
$vsPath = & $vswhere -latest -products * -requires Microsoft.VisualCpp.Tools.HostX64.TargetX64 -property installationPath
if (-not $vsPath) {
    Write-Error "No Visual Studio installation with C++ tools found."
    exit 1
}

$vcvarsall = Join-Path $vsPath "VC\Auxiliary\Build\vcvarsall.bat"
if (-not (Test-Path $vcvarsall)) {
    Write-Error "vcvarsall.bat not found at: $vcvarsall"
    exit 1
}

Write-Host "Found Visual Studio at: $vsPath"

# Import the VS x64 environment into this PowerShell session
Write-Host "Loading VS x64 build environment..."
$envLines = cmd /c "`"$vcvarsall`" x64 > nul 2>&1 && set"
foreach ($line in $envLines) {
    if ($line -match "^([^=]+)=(.*)$") {
        [System.Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], "Process")
    }
}

# Verify cl.exe is now on the PATH
$clCmd = Get-Command cl.exe -ErrorAction SilentlyContinue
if (-not $clCmd) {
    Write-Error "cl.exe not found after loading VS environment."
    exit 1
}
$clExe = $clCmd.Source
Write-Host "Using compiler: $clExe"

# Create build directory if it does not exist
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Configure
Write-Host "Configuring..."
cmake -G "Ninja" `
    -DLLVM_DIR="$LLVMDir" `
    -DCMAKE_C_COMPILER="$clExe" `
    -DCMAKE_CXX_COMPILER="$clExe" `
    -B "$BuildDir" .

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit $LASTEXITCODE
}

# Build
Write-Host "Building..."
cmake --build "$BuildDir"

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed."
    exit $LASTEXITCODE
}

Write-Host "Build complete."

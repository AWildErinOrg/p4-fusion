[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')]
    [string]$Configuration = 'Release',
    [string]$HelixApi = (Join-Path $PSScriptRoot 'vendor/helix-core-api/windows'),
    [string]$OpenSslRoot,
    [string]$ToolchainFile,
    [string]$BuildDirectory,
    [switch]$BuildTests,
    [switch]$Profiling
)

$ErrorActionPreference = 'Stop'
if (-not $BuildDirectory) {
    $BuildDirectory = Join-Path $PSScriptRoot "build/windows/$Configuration"
}

$cmakeArgs = @(
    '-S', $PSScriptRoot,
    '-B', $BuildDirectory,
    '-G', 'Visual Studio 17 2022',
    '-A', 'x64',
    "-DHELIX_API=$HelixApi",
    '-DBUILD_SHARED_LIBS=OFF',
    '-DUSE_BUNDLED_ZLIB=ON',
    '-DREGEX_BACKEND=builtin',
    '-DUSE_SSH=OFF',
    '-DUSE_HTTPS=OFF',
    '-DUSE_THREADS=ON',
    "-DBUILD_TESTS=$($BuildTests.IsPresent)",
    "-DMTR_ENABLED=$($Profiling.IsPresent)"
)
if ($OpenSslRoot) {
    $cmakeArgs += "-DOPENSSL_ROOT_DIR=$OpenSslRoot"
}
if ($ToolchainFile) {
    $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$ToolchainFile"
    $cmakeArgs += '-DVCPKG_TARGET_TRIPLET=x64-windows-static'
}

& cmake @cmakeArgs
if ($LASTEXITCODE -ne 0) {
    throw "CMake configuration failed (exit code $LASTEXITCODE)."
}

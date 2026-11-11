[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel')]
    [string]$Configuration = 'Release',
    [string]$BuildDirectory,
    [switch]$RunTests
)

$ErrorActionPreference = 'Stop'
if (-not $BuildDirectory) {
    $BuildDirectory = Join-Path $PSScriptRoot "build/windows/$Configuration"
}

& cmake --build $BuildDirectory --config $Configuration --parallel
if ($LASTEXITCODE -ne 0) {
    throw "Build failed (exit code $LASTEXITCODE)."
}
if ($RunTests) {
    # CTest otherwise succeeds even when no tests were configured.
    if (-not (Select-String -LiteralPath (Join-Path $BuildDirectory 'CMakeCache.txt') -Pattern '^BUILD_TESTS:BOOL=(ON|TRUE|1)$' -Quiet)) {
        throw 'Tests were not configured. Run generate_cache.ps1 with -BuildTests first.'
    }
    Push-Location -LiteralPath $BuildDirectory
    try {
        & ctest -C $Configuration --output-on-failure
        if ($LASTEXITCODE -ne 0) {
            throw "Tests failed (exit code $LASTEXITCODE)."
        }
    } finally {
        Pop-Location
    }
}

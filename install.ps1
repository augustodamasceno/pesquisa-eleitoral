# Pesquisa Eleitoral - Windows Install
# Copyright (c) 2026, Augusto Damasceno.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause
#
# See https://github.com/augustodamasceno/pesquisa-eleitoral
#

$ErrorActionPreference = "Stop"

$buildDir  = "$PSScriptRoot\build"
$vcpkgDir  = "$buildDir\vcpkg"

if (-not (Test-Path $buildDir)) {
    Write-Host "Creating build directory..." -ForegroundColor Cyan
    New-Item -ItemType Directory -Path $buildDir | Out-Null
} else {
    Write-Host "build directory already exists, skipping." -ForegroundColor Yellow
}


if (-not (Test-Path $vcpkgDir)) {
    Write-Host "Cloning vcpkg..." -ForegroundColor Cyan
    git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
} else {
    Write-Host "vcpkg directory already exists, skipping clone." -ForegroundColor Yellow
}

Push-Location $vcpkgDir
try {
    if (-not (Test-Path ".\vcpkg.exe")) {
        Write-Host "Bootstrapping vcpkg..." -ForegroundColor Cyan
        .\bootstrap-vcpkg.bat -disableMetrics
    } else {
        Write-Host "vcpkg already bootstrapped, skipping." -ForegroundColor Yellow
    }

    Write-Host "Installing sqlite3..." -ForegroundColor Cyan
    .\vcpkg install sqlite3

    Write-Host "Installing sqlitecpp..." -ForegroundColor Cyan
    .\vcpkg install sqlitecpp

    Write-Host "Integrating vcpkg with Visual Studio..." -ForegroundColor Cyan
    .\vcpkg integrate install
} finally {
    Pop-Location
}

Write-Host "Creating app data directory at $appDataDir..." -ForegroundColor Cyan
if (-not (Test-Path $appDataDir)) {
    New-Item -ItemType Directory -Path $appDataDir | Out-Null
} else {
    Write-Host "Directory already exists, skipping." -ForegroundColor Yellow
}

Write-Host "Generating seed database..." -ForegroundColor Cyan
Push-Location "$PSScriptRoot\database\seeds"
try {
    powershell -ExecutionPolicy Bypass -File .\generate.ps1
} finally {
    Pop-Location
}

$seedSource = "$PSScriptRoot\database\seeds\seed.db"
$dbDest     = "$appDataDir\database.db"
Write-Host "Copying seed database to $dbDest..." -ForegroundColor Cyan
Copy-Item -Path $seedSource -Destination $dbDest -Force

Write-Host "Configuring Qt UI build..." -ForegroundColor Cyan
$env:PATH = "$qtMingwBin;$env:PATH"

& $qtCmake -S $uiSrcDir -B $uiBuildDir `
    -G "MinGW Makefiles" `
    -DCMAKE_BUILD_TYPE=Release `
    "-DCMAKE_PREFIX_PATH=$qtMingwDir"
if ($LASTEXITCODE -ne 0) { Write-Error "Qt cmake configure failed." }

Write-Host "Building Qt UI..." -ForegroundColor Cyan
& $qtCmake --build $uiBuildDir --config Release --parallel
if ($LASTEXITCODE -ne 0) { Write-Error "Qt UI build failed." }


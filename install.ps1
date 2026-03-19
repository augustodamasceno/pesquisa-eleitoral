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

# Clone vcpkg  
if (-not (Test-Path $vcpkgDir)) {
    Write-Host "Cloning vcpkg..." -ForegroundColor Cyan
    git clone https://github.com/microsoft/vcpkg.git $vcpkgDir
} else {
    Write-Host "vcpkg directory already exists, skipping clone." -ForegroundColor Yellow
}

# vcpkg installations
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

# Create app data folder and copy initial database
$appDataDir = "$HOME\pesquisa-eleitoral"
Write-Host "Creating app data directory at $appDataDir..." -ForegroundColor Cyan
if (-not (Test-Path $appDataDir)) {
    New-Item -ItemType Directory -Path $appDataDir | Out-Null
} else {
    Write-Host "Directory already exists, skipping." -ForegroundColor Yellow
}

$seedSource = "$PSScriptRoot\database\seeds\seed.db"
$dbDest     = "$appDataDir\database.db"

# Generate seed database
Write-Host "Generating seed database..." -ForegroundColor Cyan
Push-Location "$PSScriptRoot\database\seeds"
try {
    powershell -ExecutionPolicy Bypass -File .\generate.ps1
} finally {
    Pop-Location
}

Write-Host "Copying seed database to $dbDest..." -ForegroundColor Cyan
Copy-Item -Path $seedSource -Destination $dbDest -Force

Write-Host ""
Write-Host "All dependencies installed successfully." -ForegroundColor Green

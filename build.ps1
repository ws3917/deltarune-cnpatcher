param(
    [string]$type = "build"   # build / clean
)

$ErrorActionPreference = "Stop"
$projectName = "outertale0"

# ===========================
# Clean Function
# ===========================
function Clean-All {
    Write-Host "====================================="
    Write-Host " Cleaning Project"
    Write-Host "====================================="

    # ---- CMake build ----
    if (Test-Path "build") {
        Write-Host "Removing build/..."
        Remove-Item "build" -Recurse -Force
    }

    # ---- Install output ----
    if (Test-Path "install") {
        Write-Host "Removing install/..."
        Remove-Item "install" -Recurse -Force
    }

    # ---- Dist packages ----
    if (Test-Path "dist") {
        Write-Host "Removing dist/..."
        Remove-Item "dist" -Recurse -Force
    }

    # ---- Android build cache ----
    if (Test-Path "android/build") {
        Write-Host "Removing android/build/..."
        Remove-Item "android/build" -Recurse -Force
    }

    if (Test-Path "android/.gradle") {
        Write-Host "Removing android/.gradle/..."
        Remove-Item "android/.gradle" -Recurse -Force
    }

    if (Test-Path "android/.cxx") {
        Write-Host "Removing android/.cxx/..."
        Remove-Item "android/.cxx" -Recurse -Force
    }

    Write-Host "Clean complete."
}

# ===========================
# Command Dispatch
# ===========================
if ($type -eq "clean") {
    Clean-All
    exit 0
}

# Prepare dist folder
if (Test-Path "dist") {
    Remove-Item "dist" -Recurse -Force
}
New-Item -ItemType Directory -Path "dist" | Out-Null

Write-Host "====================================="
Write-Host " Building: $type"
Write-Host "====================================="

# ---- CMake configure ----
Write-Host "[1/4] Configuring CMake..."
cmake --preset $type

# ---- CMake build ----
Write-Host "[2/4] Building..."
cmake --build "build/$type"

# ---- Install ----
Write-Host "[3/4] Installing..."
cmake --install "build/$type" --prefix "install/$type"

# ---- Zip Windows package ----
Write-Host "[4/4] Packaging Windows zip..."
$zipName = "dist/win-$type.zip"

if (Test-Path $zipName) {
    Remove-Item $zipName -Force
}

Compress-Archive -Path "install/$type/*" -DestinationPath $zipName

# ---- Android build ----
Write-Host "Building Android APK ($type)..."
if ($type.ToLower() -like "*debug*") {
    $gradleType = "debug"
}
elseif ($type.ToLower() -like "*release*") {
    $gradleType = "release"
}
else {
    throw "Unknown build type: $type (must contain 'debug' or 'release')"
}

pushd ".\android"

# ---- Gradle Build ----
if ($gradleType -eq "release") {
    .\gradlew.bat assembleRelease
}
else {
    .\gradlew.bat assembleDebug
}

popd

# ---- Copy APK ----
$apkPath = "build/outputs/apk/$gradleType/$projectName-$gradleType.apk"
$apkOut  = "dist/android-$gradleType.apk"

if (Test-Path "android/$apkPath") {
    Copy-Item "android/$apkPath" $apkOut -Force
    Write-Host "APK saved to $apkOut"
}
else {
    Write-Warning "APK not found: android/$apkPath"
}

Write-Host "Done: $type"

Write-Host "====================================="
Write-Host " All builds finished successfully."
Write-Host "====================================="

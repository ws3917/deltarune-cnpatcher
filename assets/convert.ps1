$inputFile = "icon.png"
$outputDir = "android_res"
$zipName = "android_icons.zip"

# 清理旧数据
if (Test-Path $outputDir) { Remove-Item -Recurse -Force $outputDir }
if (Test-Path $zipName) { Remove-Item -Force $zipName }

# 定义尺寸配置 (文件夹名称 = 尺寸)
$sizes = @{
    "mipmap-mdpi" = 48
    "mipmap-hdpi" = 72
    "mipmap-xhdpi" = 96
    "mipmap-xxhdpi" = 144
    "mipmap-xxxhdpi" = 192
}

Write-Host "正在处理图标..." -ForegroundColor Cyan

foreach ($name in $sizes.Keys) {
    $size = $sizes[$name]
    $targetPath = Join-Path $outputDir $name
    New-Item -ItemType Directory -Force -Path $targetPath | Out-Null
    
    # 调用 ImageMagick
    magick $inputFile -resize "${size}x${size}" "$targetPath\ic_launcher.png"
}

# 压缩
Write-Host "正在打包 ZIP..." -ForegroundColor Cyan
Compress-Archive -Path $outputDir -DestinationPath $zipName

Write-Host "✅ 成功! 输出文件: $zipName" -ForegroundColor Green
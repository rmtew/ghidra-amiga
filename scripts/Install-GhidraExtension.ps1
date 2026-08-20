[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateScript({ Test-Path $_ -PathType Leaf })]
    [string] $ArchivePath,

    [Parameter(Mandatory = $true)]
    [ValidateNotNullOrEmpty()]
    [string] $ExtensionName,

    [string] $GhidraUserDirectory = (Join-Path $env:APPDATA 'ghidra\ghidra_12.1.2_PUBLIC')
)

$ErrorActionPreference = 'Stop'
$extensionsDirectory = Join-Path $GhidraUserDirectory 'Extensions'
$destination = Join-Path $extensionsDirectory $ExtensionName
$staging = Join-Path ([System.IO.Path]::GetTempPath()) ("ghidra-extension-{0}" -f [guid]::NewGuid())
$payload = Join-Path $staging $ExtensionName
$backup = Join-Path ([System.IO.Path]::GetTempPath()) ("ghidra-extension-backup-{0}" -f [guid]::NewGuid())

try {
    Expand-Archive -LiteralPath $ArchivePath -DestinationPath $staging
    if (-not (Test-Path (Join-Path $payload 'Module.manifest') -PathType Leaf) -or
        -not (Test-Path (Join-Path $payload 'extension.properties') -PathType Leaf) -or
        -not (Test-Path (Join-Path $payload "lib\$ExtensionName.jar") -PathType Leaf)) {
        throw "Archive is not a complete '$ExtensionName' Ghidra extension."
    }

    New-Item -ItemType Directory -Force -Path $extensionsDirectory | Out-Null
    if (Test-Path $destination) {
        # This fails cleanly if Ghidra still has the extension JAR open.
        Move-Item -LiteralPath $destination -Destination $backup
    }
    Move-Item -LiteralPath $payload -Destination $destination
    if (Test-Path $backup) {
        Remove-Item -LiteralPath $backup -Recurse -Force
    }
    Write-Output "Installed '$ExtensionName' to '$destination'."
}
catch {
    if (-not (Test-Path $destination) -and (Test-Path $backup)) {
        Move-Item -LiteralPath $backup -Destination $destination
    }
    throw
}
finally {
    if (Test-Path $staging) {
        Remove-Item -LiteralPath $staging -Recurse -Force
    }
}

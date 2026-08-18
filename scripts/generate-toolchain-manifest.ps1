param(
    [string]$ToolchainsPath = (Join-Path $PSScriptRoot '..\toolchains'),
    [switch]$Verify
)

$root = (Resolve-Path $ToolchainsPath).Path
$manifest = Join-Path $root 'MANIFEST.sha256'

if ($Verify) {
    if (-not (Test-Path -LiteralPath $manifest -PathType Leaf)) {
        throw "Manifest does not exist: $manifest"
    }
    $failures = [System.Collections.Generic.List[string]]::new()
    $count = 0
    foreach ($line in Get-Content -LiteralPath $manifest) {
        if ($line -notmatch '^(?<hash>[0-9a-f]{64})  (?<relative>.+)$') {
            $failures.Add("Invalid manifest line: $line")
            continue
        }
        $file = Join-Path $root $Matches.relative.Replace('/', '\')
        if (-not (Test-Path -LiteralPath $file -PathType Leaf)) {
            $failures.Add("Missing: $($Matches.relative)")
            continue
        }
        $actual = (Get-FileHash -LiteralPath $file -Algorithm SHA256).Hash.ToLowerInvariant()
        if ($actual -ne $Matches.hash) {
            $failures.Add("Hash mismatch: $($Matches.relative)")
        }
        $count++
    }
    if ($failures.Count -ne 0) {
        throw ("Toolchain manifest verification failed:`n" + ($failures -join "`n"))
    }
    Write-Output "Verified $count entries in $manifest"
    return
}

$lines = Get-ChildItem $root -Recurse -File |
    Where-Object { $_.FullName -ne $manifest } |
    Sort-Object FullName |
    ForEach-Object {
        $relative = $_.FullName.Substring($root.Length + 1).Replace('\', '/')
        "$( (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash.ToLowerInvariant() )  $relative"
    }

[System.IO.File]::WriteAllLines($manifest, $lines, [System.Text.UTF8Encoding]::new($false))
Write-Output "Wrote $($lines.Count) entries to $manifest"

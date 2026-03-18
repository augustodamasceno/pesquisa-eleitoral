$ErrorActionPreference = 'Stop'

$dir     = $PSScriptRoot
$schema  = Join-Path $dir '..\schema\tables'
$seed    = Join-Path $dir 'seed.sql'
$db      = Join-Path $dir 'seed.db'

$tables = @('city', 'candidate', 'vote', 'result')

$lines = [System.Collections.Generic.List[string]]::new()

$lines.Add('PRAGMA journal_mode=WAL;')
$lines.Add('PRAGMA foreign_keys=ON;')
$lines.Add('')

foreach ($table in $tables) {
    $lines.Add("-- $table")
    $lines.Add((Get-Content (Join-Path $schema "$table.sql") -Raw).TrimEnd())
    $lines.Add('')
}

[System.IO.File]::WriteAllLines($seed, $lines, [System.Text.UTF8Encoding]::new($false))

& sqlite3 $db ((Get-Content $seed -Raw))

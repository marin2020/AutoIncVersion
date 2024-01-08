#Reads $name.rc
#Takes major and minor version from $name.GhVersion - they need to be in X.Y format
#replaces a.b.c.d or a,b,c,d with maojr,minor,build,patch respecting the delimiter (. or ,)
#where the "build" is the number of days since Jan 1 2000
# and patch is the number of seconds from the start of today divided by 2

Param(
    [Parameter(Position=1)]
    [string]$dir,
    [Parameter()]
    [string]$name
    )

if([string]::IsNullOrWhiteSpace($dir))
{
    throw '''dir'' argument not supplied'
}

if([string]::IsNullOrWhiteSpace($name))
{
    throw '''name'' argument not supplied'
}

Write-Output "Dir : $dir" 
Write-Output "File: $name" 

$sep = [System.IO.Path].DirectorySeparatorChar
$fileName = $dir + $sep + $name
$rcFileName = $fileName + ".rc"
$verFileName = $fileName + ".GhVersion"

$ver = (Get-Content $verFileName).Split('.');
$major = $ver[0];
$minor = $ver[1];


$build = (New-TimeSpan -Start (Get-Date -Date "2000-01-01") -End ([DateTime]::Now)).Days;
$patch =  [Math]::Ceiling((New-TimeSpan -Start ([DateTime]::Today) -End ([DateTime]::Now)).TotalSeconds / 2)
$build
$patch
$repl = $major + '${s1}'+ $minor +'${s2}'+$build +'${s3}'+ $patch
$repl

(Get-Content $rcFileName) `
    -replace '(?<major>\d+)(?<s1>[.|,]{1})(?<minor>\d+)(?<s2>[.|,]{1})(?<build>\d+)(?<s3>[.|,]{1})(?<patch>\d+)', $repl `
  |
Out-File $rcFileName

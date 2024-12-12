#e.g. .\_1-extpak.ps1 *psp-sn.bin
param([string]$specifier="*-sn.bin")


Get-ChildItem . -filter $specifier -name | % {
  Write-Host "python extract_snbin.py $($_)" -foregroundcolor cyan
  c:\Python33\python extract_snbin.py $_
  # cmd /c pause
}
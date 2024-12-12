#e.g. .\_2-extjis.ps1 "z__*psp*.opcodescript"
param([string]$specifier="*.opcodescript")


#Get-ChildItem . -filter "*psp*.opcodescript" -name | % {
#Get-ChildItem . -filter "*x360*.opcodescript" -name | % {
Get-ChildItem . -filter $specifier -name | % {
  Write-Host "python extjis_psp-cc.py $($_)" -foregroundcolor cyan
  c:\Python33\python extjis_psp-cc.py $_
  # cmd /c pause
}
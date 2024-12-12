#Get-ChildItem . -filter "*psp*-extr.txt" -name | % {
#Get-ChildItem . -filter "*x360*-extr.txt" -name | % {
Get-ChildItem . -filter "*-extr.txt" -name | % {
  $output = [System.IO.Path]::ChangeExtension($_, ".po")
  Write-Host "txt2po -o $($output) $($_)" -foregroundcolor cyan
  C:\Sites\djangostack\python\python C:\Sites\djangostack\python\scripts\txt2po -o $output $_
  # cmd /c pause
}
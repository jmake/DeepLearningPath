

if (Test-Path "Env1") { Remove-Item -Path "Env1" -Recurse -Force }

..\..\Python31000\python.exe -m venv Env1

.\Env1\Scripts\Activate.ps1

python.exe -m pip install open3d mediapipe opencv-python 



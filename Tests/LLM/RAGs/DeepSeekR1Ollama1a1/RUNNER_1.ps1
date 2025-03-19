<#---------------------------------------------------------------------------#>
<#---------------------------------------------------------------------------#>
function PIP_SETUP() 
{
    try{ deactivate } catch{ }

    $ENV="D:\z2025_1\AI\RAGs\Env3\Scripts\Activate.ps1"
    & $ENV 

    & $PYTHON_PATH --version 

    ##Invoke-WebRequest https://bootstrap.pypa.io/get-pip.py  -OutFile get-pip.py
    ##& $PYTHON_PATH get-pip.py 

    & $PYTHON_PATH -m pip install --upgrade pip
    & $PYTHON_PATH -m pip --version 

    & $PYTHON_PATH -m pip install -r requirements.txt
}


function OLLAMA_SETUP()
{
    ##.\OllamaSetup.exe /DIR="D:\z2025_1\AI\Ollama060"

    ollama.exe --version # 0.6.1 

    ollama.exe pull deepseek-r1:14b ## C:\Users\zvl_2\.ollama\models

}


function PIP_RUNNER() 
{

    & $PYTHON_PATH --version 

    streamlit.exe --version    

    streamlit.exe run Sources\app_2.py
}



<#---------------------------------------------------------------------------#>
<#---------------------------------------------------------------------------#>
#$PYTHON_PATH="D:\z2025_1\AI\Python3140\python.exe"

$PYTHON_PATH="python.exe"

#OLLAMA_SETUP 
PIP_SETUP 
PIP_RUNNER


<#---------------------------------------------------------------------------#>
<#---------------------------------------------------------------------------#>
<#
https://www.python.org/downloads/windows/

#>
<#---------------------------------------------------------------------------#>